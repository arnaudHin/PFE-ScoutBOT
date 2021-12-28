#!/bin/bash

echo -e "Compilation du programme LIDAR pour la STM32MP1\n\n"
make clean
./make_openST.sh
echo -e "Envoie des fichiers LIDAR à la MP1\n\n"
scp lidar.sh root@192.168.7.1:/usr/local/ROBOT
cd output
scp prog.elf root@192.168.7.1:/usr/local/ROBOT
echo -e "\n\nFichiers LIDAR envoyés !"

