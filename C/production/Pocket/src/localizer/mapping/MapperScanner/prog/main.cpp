#include <stdio.h>
#include <stdlib.h>
#include "lidar.hpp"

#define displayInformation 0

using namespace rp::standalone::rplidar;

int main(int argc, const char * argv[]) {
    const char * opt_com_path = NULL;
    _u32         opt_com_baudrate = 115200;
    u_result     op_result;
    
    fichier_open();

    if (argc < 2) {
        return -1;
    }
    opt_com_path = argv[1];

    if (argc>2) opt_com_baudrate = strtoul(argv[2], NULL, 10);

    // create the driver instance
    RPlidarDriver * drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

    if (!drv) {
        fprintf(stderr, "insufficent memory, exit\n");
        exit(-2);
    }

    rplidar_response_device_health_t healthinfo;
    rplidar_response_device_info_t devinfo;

    do {
        // try to connect
        if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate))) {
            fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
                , opt_com_path);
            break;
        }

        // retrieving the device info
        op_result = drv->getDeviceInfo(devinfo);

        if (IS_FAIL(op_result)) {
            if (op_result == RESULT_OPERATION_TIMEOUT) {
                // you can check the detailed failure reason
                fprintf(stderr, "Error, operation time out.\n");
            } else {
                fprintf(stderr, "Error, unexpected error, code: %x\n", op_result);
                // other unexpected result
            }
            break;
        }
        
        #if displayInformation 
        // print out the device serial number, firmware and hardware version number..
        printf("RPLIDAR S/N: ");
        for (int pos = 0; pos < 16 ;++pos) {
            printf("%02X", devinfo.serialnum[pos]);
        }


        printf("\n"
                "Version: " RPLIDAR_SDK_VERSION"\n"
                "Firmware Ver: %d.%02d\n"
                "Hardware Rev: %d\n"
                , devinfo.firmware_version>>8
                , devinfo.firmware_version & 0xFF
                , (int)devinfo.hardware_version);
        #endif

        // check the device health
        ////////////////////////////////////////
        op_result = drv->getHealth(healthinfo);
        if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
            #if displayInformation
            printf("RPLidar health status : ");
            #endif
            switch (healthinfo.status) {
            case RPLIDAR_STATUS_OK:
                #if displayInformation
                printf("OK.");
                #endif
                break;
            case RPLIDAR_STATUS_WARNING:
                #if displayInformation
                printf("Warning.");
                #endif
                break;
            case RPLIDAR_STATUS_ERROR:
                #if displayInformation
                printf("Error.");
                #endif
                break;
            }
            #if displayInformation
            printf(" (errorcode: %d)\n", healthinfo.error_code);
            #endif

        } else {
            fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
            break;
        }


        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want rplidar to be reboot by software
            // drv->reset();
            break;
        }

        drv->startMotor();

        // take only one 360 deg scan 
        ////////////////////////////////////////////////////////////////////////////////
        if (IS_FAIL(drv->startScan( 0,1 ))) // you can force rplidar to perform scan operation regardless whether the motor is rotating
        {
            fprintf(stderr, "Error, cannot start the scan operation.\n");
            break;
        }

        // for (int pos = 0; pos < 10 ; ++pos) {
        //     if(pos == 9){
                if (IS_FAIL(displayValue(drv))) {
                    fprintf(stderr, "Error, cannot grab scan data.\n");
                    break;
            //     }
            // }
        }

    } while(0);

    drv->stop();
    drv->stopMotor();
    fichier_close();
    RPlidarDriver::DisposeDriver(drv);
    return 0;
}
