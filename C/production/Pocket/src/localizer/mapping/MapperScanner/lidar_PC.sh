#!/bin/bash

echo -e "Lancement du programme LIDAR MAPPING"

make clean
make
cd output
rm -f position_gtk.txt
echo -e "\nExecution de prog.elf\n"
sudo ./prog.elf /dev/ttyUSB0
cd ../prog
echo -e "\nExecution de main.py\n"
python3.8 main.py





