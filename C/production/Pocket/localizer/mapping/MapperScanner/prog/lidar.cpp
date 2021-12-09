#include "lidar.hpp"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

ofstream myfile;

#define displayInformation 1

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
    double dvalueA = 0;
    double dvalueD = 0;

    polar pol;

    int angleBuffer [2000];
    int distanceBuffer [2000];
    // fetech extactly one 0-360 degrees' scan
    ans = drv->grabScanDataHq(nodes, count);
    if (IS_OK(ans) || ans == RESULT_OPERATION_TIMEOUT) {
        drv->ascendScanData(nodes, count);
        for (int posi = 0; posi < (int)count ; ++posi) {

            valueA = (nodes[posi].angle_z_q14* 90.f / (1 << 14));
            valueD = nodes[posi].dist_mm_q2/4.0f;
            valueQ = (nodes[posi].quality);

            dvalueA = valueA;
            dvalueD = valueD;
    
            setMyStructp(&pol,dvalueD,dvalueA);
            pol.angle = pol.angle;
            pol.distance = pol.distance;

            // ConvertToCartesian(pol.distance,pol.angle);

            angleBuffer[posi] = pol.angle;
            distanceBuffer[posi] = pol.distance;
        }           

        for (int posi = 0; posi < 1500 ; ++posi) {
            myfile << "  " << angleBuffer[posi] << "     " << distanceBuffer[posi] << "\n";
        }
    } 
    else 
    {
        printf("error code: %x\n", ans);
    }
    return ans;
}



polar clearMyStructp(polar *stpolar)
{
    stpolar->distance = 0;
    stpolar->angle = 0;
    return *stpolar;
}

double setMyStructp(polar *stpolar, double distance, double angle)
{
    if (!stpolar)
        return -1;
    stpolar->distance = distance;
    stpolar->angle = angle;
    return 0;
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
    myfile.open("data_lidar.txt",ios_base :: app );
}

void fichier_close(void){
    myfile.close();
}






