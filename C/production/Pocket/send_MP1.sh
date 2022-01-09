#!/bin/bash

echo -e "Compilation du programme Pocket\n\n"
make clean
./make_openST.sh
echo -e "Envoie de Pocket sur la MP1\n\n"
scp ./bin/pocket root@192.168.7.1:/usr/local/testMartin
cd src/localizer/mapping/MapperScanner
pwd
./send_MP1.sh

echo -e "\nFin des envoies\n"
