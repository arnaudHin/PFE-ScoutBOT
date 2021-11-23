/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/**
 * @file  proxy_robot.c
 * @author Debus Alexy
 * @version 0.2
 * @date 01/05/2020
 * @brief Postman_pilot entity source file
 *
 */

#include "../../pocket/com_Race/postman_pilot.h"

#include "../../util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "pthread.h"
#include "../map_raw.h"
#include <math.h>

//#include "../../pocket/com_Race/dispatcher_pilot.h"
#include "../../pocket/pilot.h"

#define FLAG_SIZE 1
#define SLAVE_ADDR_SIZE 1
#define MAX_DATA  1
#define CMD_SIZE  1

#define SENSORS_SIZE 4
#define ODOMETRY_SIZE 6
#define LIDAR_SIZE 2
#define SERVO_ANGLE_SIZE 2

#define SENSOR_SIZE 1
#define COORD_SIZE 2
#define LIDAR_DISTANCE_SIZE (LIDAR_SIZE * NB_SENSOR)

#define MAX_MSG_SIZE 1024
#define BYTE_MSG 8
#define MAX_READ_ATTEMPT 10
#define DEVICE_PORT "/dev/ttyRPMSG0"
#define RPMSG_BUFFER_SIZE	FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + ODOMETRY_SIZE + LIDAR_DISTANCE_SIZE + SERVO_ANGLE_SIZE + FLAG_SIZE
/************************************** STATIC FUNCTION *************************************************************/

static Pilot_message_s send_message_conversion(Pilot_message_s pilot_message_sent);
static Pilot_message_r receive_message_conversion(Pilot_message_r pilot_message_receive);

static void *postman_run(void *unused);
static void abort_postman_read_message();
static void wait_end_of_read_thread();

static void convert_to_str(char* msg,Pilot_message_s pilot_msg);

static void postman_dispatch(char * buffer_read, Pilot_message_r * pilot_message);

/**************************************END OF STATIC FUNCTION *******************************************************/

static pthread_t compute_thread;
static int serial_port = 0;
static bool exit_postman = false;

/*
 *  \brief Function dedicated to start the postman (Open the serial port)
 */
extern void postman_start()
{
	struct termios tty;
	memset(&tty,0,sizeof(tty));

	serial_port = open(DEVICE_PORT, O_RDWR);
	if(serial_port == -1){
		perror("Unable to open the serial port:");
		printf("%s\n",DEVICE_PORT);
		exit(errno);
	}else{
		TRACE("Serial port open successfully !\n");
	}

	if (tcgetattr(serial_port, &tty) != 0){
		perror("Error from existing configuration from tcgetattr");
	}

	/**************************************TTY CONFIGURATION ***********************************************************/

	tty.c_cflag |= (CLOCAL | CREAD);  // Ignore ctrl lines
	tty.c_cflag |= PARENB;            // No parity bit
	tty.c_cflag |= PARODD;			  // No odd number control
	tty.c_cflag &= ~CRTSCTS;          // DIsable HFC (Hardware FLow Control)
 	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;               // Standard 8 bits / byte

	tty.c_lflag &= ~ICANON;           // Set non-canonical mode
	tty.c_cc[VTIME] = 0.1;            // Set timeout to 50 milliseconds

	tty.c_lflag &= ~ECHO;             // Disable echo
	tty.c_lflag &= ~ECHOE;            // Disable erasure
	tty.c_lflag &= ~ECHONL;           // Disable new-line echo

	cfsetispeed(&tty, B115200);       // Set input  baudrate to 115200
	cfsetospeed(&tty, B115200);       // Set output baudrate to 115200

	/************************************** END OF TTY CONFIGURATION ***************************************************/

	if (tcsetattr(serial_port,TCSANOW, &tty) != 0){
			perror("Error from new configuration from tcsetattr");
		}
	exit_postman = false;

	int return_thread_creation = 0;
	return_thread_creation = pthread_create(&compute_thread, NULL, &postman_run, NULL);

	if (return_thread_creation != 0){
		perror("Error creating thread\n");
	}

}

/* \fn static void *Postman_run(void *unused)
 * \brief Function dedicated to send a message to the entity postman until the read buffer is empty
 */
static void *postman_run(void *unused){
	while(!exit_postman){
		dispatcher_pilot_set_msg(postman_read_message());
	}
	TRACE("Exit postman run");
	return 0;
}

/* \fn static void abort_postman_read_message(void)
 * \brief Function dedicated to stop to watch for an incoming message
 */
static void abort_postman_read_message(void)
{
	TRACE("Try abort read message\n");
	exit_postman = true;
}

/* \fn static void wait_end_of_read_thread(void)
 * \brief Function dedicated to wait until the end of the current task
 */
static void wait_end_of_read_thread(void)
{
	int return_end_of_compute_thread = 0;
	TRACE("Wait end of compute thread %d\n",(int)pthread_self());
	return_end_of_compute_thread = pthread_join(compute_thread, NULL);

	if (return_end_of_compute_thread != 0){
		perror("Error while trying to end read msg thread");
	}
}

/*
 *  \brief Function dedicated to stop the postman (Close the serial port)
 */
void postman_stop(){
	abort_postman_read_message();
	wait_end_of_read_thread();
	close(serial_port);
	TRACE("Serial port closed\n");
}

/*
 * \brief Function dedicated to read message on the given serial port
 * \return Pilot_message the structure which contain a read message
 */
extern Pilot_message_r postman_read_message()
{
	Pilot_message_r pilot_message;
	char my_received_message[RPMSG_BUFFER_SIZE];

	char buffer_read[RPMSG_BUFFER_SIZE];
	memset(buffer_read,0,RPMSG_BUFFER_SIZE);

	int current_size = 0;
	int returnBytes = 0;

	if (serial_port == -1)
	{
		perror("Error : incorrect port");
		exit(EXIT_FAILURE);
	}else{
		TRACE("READ MESSAGE FROM M4\n");
		while(current_size<RPMSG_BUFFER_SIZE){
			TRACE("Postman pilot is receiving (On the A7) ... \r\n");
			//TRACE("on va read...\r\n");
			TRACE("serial_port = %d \r\n", serial_port);
			for(int z=0;z<RPMSG_BUFFER_SIZE;z++)
			{
				//TRACE("buffer_read numero %d = %c \r\n", z+1, buffer_read[z]);
			}
			TRACE("RPMSG_BUFFER_SIZE = %d \r\n", RPMSG_BUFFER_SIZE);
			returnBytes = read (serial_port, buffer_read, RPMSG_BUFFER_SIZE);
			//TRACE("read...\r\n");
			if (returnBytes==-1)
			{
				//TRACE("read KO\r\n");
				perror("Error : Impossible to read\r\n");
				exit(EXIT_FAILURE);
			}
			//TRACE("read OK\r\n");
			printf("Byte received from Race: ");
			for(int i=0;i<sizeof(buffer_read);i++){
						printf("%02X ",buffer_read[i]);
					}printf("\n");

			// returnBytes could be 0 at the end of the connection
			if (returnBytes!=0){
				for(int i=0;i<returnBytes;i++){
					my_received_message[current_size+i] = buffer_read[i];
				};

				current_size = current_size + returnBytes;
			}else{
				break;
			}
		}
	}
	TRACE("Postman_dispatch ...\n");

	postman_dispatch (my_received_message,&pilot_message);

	return pilot_message;

}

static void postman_dispatch (char * buffer_read, Pilot_message_r * pilot_message){

	pilot_message->start_flag = 0;
	pilot_message->order_r = 0;
	pilot_message->my_current_position.x = 0;
	pilot_message->my_current_position.y = 0;
	pilot_message->my_current_position.teta = 0;
	for(uint8_t num_sensor=0 ;num_sensor<NB_SENSOR ;num_sensor++)
	{
		pilot_message->my_lidar_state.distance[num_sensor] = 0;
	}
	pilot_message->my_lidar_state.servoAngle = 0;


	/*printf("Byte received from Race");
		for(int i=0;i<13;i++){
					printf("%02X ",buffer_read[i]);
				}printf("\n");
	 */

	// Get value of start_flag
	memcpy(&pilot_message->start_flag, &buffer_read[0], FLAG_SIZE);
	TRACE("Start flag  : %d \n",pilot_message->start_flag);

	// Get value of CMD
	memcpy(&pilot_message->order_r, &buffer_read[FLAG_SIZE], CMD_SIZE);
	TRACE("Order : %d \n",pilot_message->order_r);

	// Get value of sensors
	// memcpy(&pilot_message->my_sensors_state.bumper_state_fl, &buffer_read[FLAG_SIZE + CMD_SIZE], SENSOR_SIZE);
	// memcpy(&pilot_message->my_sensors_state.bumper_state_fr, &buffer_read[FLAG_SIZE + CMD_SIZE + SENSOR_SIZE], SENSOR_SIZE);
	// memcpy(&pilot_message->my_sensors_state.bumper_state_bl, &buffer_read[FLAG_SIZE + CMD_SIZE + 2*SENSOR_SIZE], SENSOR_SIZE);
	// memcpy(&pilot_message->my_sensors_state.bumper_state_br, &buffer_read[FLAG_SIZE + CMD_SIZE + 3*SENSOR_SIZE], SENSOR_SIZE);

	// TRACE("Sensor : fl:%d ; fr:%d ; bl:%d ; br:%d\n",
	// 		pilot_message->my_sensors_state.bumper_state_bl,
	// 		pilot_message->my_sensors_state.bumper_state_fr,
	// 		pilot_message->my_sensors_state.bumper_state_bl,
	// 		pilot_message->my_sensors_state.bumper_state_br);

	// Get value of odometry
	// memcpy(&pilot_message->my_current_position.x, &buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE], COORD_SIZE);
	// memcpy(&pilot_message->my_current_position.y, &buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + COORD_SIZE], COORD_SIZE);
	// memcpy(&pilot_message->my_current_position.teta, &buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + 2*COORD_SIZE], COORD_SIZE);
	// pilot_message->my_current_position.x = ntohs(pilot_message->my_current_position.x);
	// pilot_message->my_current_position.y = ntohs(pilot_message->my_current_position.y);
	// pilot_message->my_current_position.teta = ntohs(pilot_message->my_current_position.teta);

	// TRACE("X dans la frame : %02X\n",&buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE], COORD_SIZE);
	// TRACE("Position x : %d\n",pilot_message->my_current_position.x);
	// TRACE("Position y : %d\n",pilot_message->my_current_position.y);
	// TRACE("Angle teta : %d\n",pilot_message->my_current_position.teta);

	// Get value of lidar sensors
	// for(uint8_t num_sensor_2=0 ;num_sensor_2<NB_SENSOR ;num_sensor_2++)
	// {
	// 	memcpy(&pilot_message->my_lidar_state.distance[num_sensor_2], &buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + ODOMETRY_SIZE + LIDAR_SIZE*num_sensor_2], LIDAR_SIZE);
	// 	pilot_message->my_lidar_state.distance[num_sensor_2] = ntohs(pilot_message->my_lidar_state.distance[num_sensor_2]);

	// 	TRACE("Distance lidar %d: %u\n", (num_sensor_2+1), pilot_message->my_lidar_state.distance[num_sensor_2]);
	// }

	// Get value of servomotor's angle
	// memcpy(&pilot_message->my_lidar_state.servoAngle, &buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + ODOMETRY_SIZE + LIDAR_DISTANCE_SIZE], SERVO_ANGLE_SIZE);
	// pilot_message->my_lidar_state.servoAngle = ntohs(pilot_message->my_lidar_state.servoAngle);
	// TRACE("Angle servomoteur : %d\n", pilot_message->my_lidar_state.servoAngle);

	// Get value of stop_flag
	memcpy(&pilot_message->stop_flag, &buffer_read[FLAG_SIZE + CMD_SIZE + SENSORS_SIZE + ODOMETRY_SIZE + LIDAR_DISTANCE_SIZE + SERVO_ANGLE_SIZE], FLAG_SIZE);
}



/**
 *  \fn void convert_int16_to_2_bytes(unsigned char * byte, uint16_t value)
 *
 *  \brief Function dedicated to convert an uin16_t to an array of bytes (size 2)
 *
 *  \param byte (unsigned char *) : byte where the data will be stored
 *
 *  \param value (uint16_t) : value to convert
 *
 */

static void convert_uint16_to_2_bytes(unsigned char * byte, uint16_t value){

	value = floor(value * MAP_RATIO -0.5);
	assert(value <= 65535); // 0xFF 0XFF
	byte[0] = (value >> 8) & 0xFF;
	byte[1] = value & 0xFF;
}


/*
 * \brief Function dedicated to send message on the given serial port
 * \param Pilot_message the structure which contain the message to send
 */
extern void postman_send_message(Pilot_message_s msg)
{
	/************** DEFAULT VALUE **************/


	unsigned char byte[FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA  + (4 * msg.max_data) + FLAG_SIZE];
	unsigned char byte_size[4 * msg.max_data];

	uint8_t cmd = msg.order_s;
	// 8 bits parameters

	byte[0] = 0x7A;
	byte[1] = 0xFF;
	byte[2] = cmd & 0xFF;
	byte[3] = msg.max_data & 0xFF;

	//MULTIPTS DATA
	int index = 4;

	if (msg.max_data>0){
		unsigned char buffer_x[COORD_SIZE];
		unsigned char buffer_y[COORD_SIZE];

		for (int i=0; i<msg.max_data ; i++){

			convert_uint16_to_2_bytes(buffer_x,(uint16_t)msg.multipts_data[i].x);
			byte[index++] = buffer_x[0];
			byte[index++] = buffer_x[1];
			convert_uint16_to_2_bytes(buffer_y,(uint16_t)msg.multipts_data[i].y);
			byte[index++] = buffer_y[0];
			byte[index++] = buffer_y[1];
		}
	}

	byte[FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA + (4 * msg.max_data)] = 0x7B;

	if (msg.max_data>0){
		printf("Byte sent to Race :\n");
		for(int i=0;i<FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA + (4 * msg.max_data) + FLAG_SIZE;i++){
					printf("%02X ",byte[i]);
				}printf("\n");
	}

	if (write(serial_port, &byte ,FLAG_SIZE + SLAVE_ADDR_SIZE + CMD_SIZE + MAX_DATA + (4 * msg.max_data) + FLAG_SIZE) ==-1){
		perror("Error : Information not send\r\n");
		exit(EXIT_FAILURE);
	}

}
