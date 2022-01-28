#include <rplidar.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <math.h>

using namespace rp::standalone::rplidar;
using namespace std;

bool checkRPLIDARHealth(RPlidarDriver * drv);
void delay(_word_size_t ms);
u_result displayValue(RPlidarDriver * drv);
void fichier_close(void);
void fichier_open(void);