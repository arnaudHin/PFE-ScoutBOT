#include <rplidar.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <cmath>


using namespace rp::standalone::rplidar;
using namespace std;

typedef struct {
    double distance;
    double angle;
} polar;

typedef struct {
    double x;
    double y;
} cartesian;

bool checkRPLIDARHealth(RPlidarDriver * drv);
polar clearMyStructp(polar *stpolar);
double setMyStructp(polar *stpolar, double distance, double angle);
void delay(_word_size_t ms);
u_result displayValue(RPlidarDriver * drv);
void fichier_close(void);
void fichier_open(void);
