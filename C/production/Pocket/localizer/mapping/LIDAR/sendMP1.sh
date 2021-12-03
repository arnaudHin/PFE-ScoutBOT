#!/bin/bash

echo -e "Envoie des fichiers à la MP1"
scp lidar.sh root@192.168.7.1:/usr/local/lidar/SCOUTBOT_LIDAR

cd output

scp prog root@192.168.7.1:/usr/local/lidar/SCOUTBOT_LIDAR
scp main.py root@192.168.7.1:/usr/local/lidar/SCOUTBOT_LIDAR

echo -e "\nFichiers envoyés !"

