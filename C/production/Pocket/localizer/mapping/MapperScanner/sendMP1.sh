#!/bin/bash

echo -e "Envoie des fichiers à la MP1"
scp lidar.sh root@192.168.7.1:/usr/local/ROBOT

cd prog 
scp main.py root@192.168.7.1:/usr/local/ROBOT

cd ..
cd output

scp prog.elf root@192.168.7.1:/usr/local/ROBOT

echo -e "\nFichiers envoyés !"
