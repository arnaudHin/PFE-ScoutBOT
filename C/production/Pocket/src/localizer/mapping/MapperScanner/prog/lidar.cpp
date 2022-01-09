#include "lidar.hpp"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

ofstream myfile;
#define displayInformation 1
#define PI 3.14159265

bool checkRPLIDARHealth(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_health_t healthinfo;

    op_result = drv->getHealth(healthinfo);
    if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        #if displayInformation
        printf("RPLidar health status : %d\n", healthinfo.status);
        #endif
        
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want rplidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}


u_result displayValue(RPlidarDriver * drv)
{
    u_result ans;
    
    rplidar_response_measurement_node_hq_t nodes[8192];
    size_t   count = _countof(nodes);

    float valueA;
    float valueD;
    float valueQ;
    float x = 0;
    float y = 0;

    // fetech extactly one 0-360 degrees' scan
    ans = drv->grabScanDataHq(nodes, count);
    if (IS_OK(ans) || ans == RESULT_OPERATION_TIMEOUT) {
        drv->ascendScanData(nodes, count);

        int xBuffer[360];
        int yBuffer[360];


        float compt = count/360.0;
        float compt2 = 0.0;
        int compBuffer[360];

        for(int i = 0; i<360; i++){
            compt2 += compt;
            compBuffer[i] = (int) compt2;
        }

        int nb = 0;

        for (int posi = 0; posi < (int)count ; ++posi) {
            nb +=1;
            valueA = (nodes[posi].angle_z_q14* 90.f / (1 << 14));
            valueD = nodes[posi].dist_mm_q2/4.0f;
            valueQ = (nodes[posi].quality);

            //convert degree to radian
            valueA = (valueA*(PI/180.0));

            //create position (x,y)
            x = (valueD * cos(valueA));
            y = (valueD * sin(valueA));

            //one data to one degree
            for (int i = 0;i<360;i++){
                if(nb == compBuffer[i]){
                    xBuffer[i] = x;
                    yBuffer[i] = y;
                }
            }
        }

        //display data on file.txt
        for (int posi = 0; posi < 360 ; ++posi) {
            myfile << xBuffer[posi] << "," << yBuffer[posi] << ",";
        }
    } 
    else 
    {
        printf("error code: %x\n", ans);
    }
    return ans;
}

void delay(_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}

void fichier_open(void){
    myfile.open("position_gtk.txt",ios_base :: app );
}

void fichier_close(void){
    myfile.close();
}