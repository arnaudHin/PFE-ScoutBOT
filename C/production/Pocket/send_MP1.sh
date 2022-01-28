#!/bin/bash

echo -e "Compilation du programme Pocket\n\n"
make clean
./make_openST.sh
echo -e "Envoie de Pocket sur la MP1\n\n"
scp ./bin/pocket root@192.168.7.1:/usr/local/testMartin
cd src/localizer/mapping/MapperScanner
pwd
./send_MP1.sh

cd ../../positioning
./send_MP1.sh
cd Indoor_positioning/bash
chmod +x btmon_search.sh
chmod +x btsnoop_hci
chmod +x btsnoop_hci.txt
chmod +x init.sh
chmod +x RSSI_scan.sh

cd ..
cd ..
cd ..
cd ..
cd ..

make clean

echo -e "\nFin des envoies\n"
