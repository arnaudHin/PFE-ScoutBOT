#!/bin/bash

echo -e "Compilation du programme JumpC\n\n"
make clean
./make_openST.sh
echo -e "Creation du repertoire de travail à envoyer sur la MP1\n\n"
mkdir JumpCMP1
cd JumpCMP1
mkdir Images
mkdir Jump_CSS
cd ..
echo -e "Envoie des fichiers JumpC dans le repertoire de travail à la MP1\n\n"
pwd
cp gtk_glade ./JumpCMP1
cp projetUI.glade ./JumpCMP1
cp ./Images/ST_Logo.png ./JumpCMP1/Images
cp ./Jump_CSS/gtk.css ./JumpCMP1/Jump_CSS
scp -r JumpCMP1 root@192.168.0.13:/usr/local/testMartin
rm -r JumpCMP1
make clean
echo -e "\n\n Dossier JumpCMP1 envoyé !"



