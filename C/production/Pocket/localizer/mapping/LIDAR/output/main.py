import math

def lire_file(fichier, encodage):
    liste_data = list()
    with open (fichier,encoding=encodage) as f:
        for ligne in f :
            angle = int(ligne[:5].strip())
            distance = int(ligne[5:14].strip())
            coord = angle, distance
            liste_data.append(coord)
    return liste_data

def tri_tour_data(liste_data):
    tab = list()
    rows, cols = (360,2)
    tab = [[0 for i in range(cols)] for j in range(rows)]

    for i in range(rows):
        for k in range(len(liste_data)):
            #on vérifie si tab est bien remplis de 0 
            if(tab[liste_data[k][0]][0] == 0 & tab[liste_data[k][0]][1] == 0):
                #on vérifie si les distances sont différentes de 0 
                if(int(liste_data[k][1]) !=0):
                    tab[liste_data[k][0]][0] = liste_data[k][0]
                    tab[liste_data[k][0]][1] = liste_data[k][1]
                    i+=1
    return tab


def mix_tour(tab,tour2):
    for i in range(len(tab)):
        if(tab[i][1] == 0):
            tab[tour2[i][0]][0] = tour2[i][0]
            tab[tour2[i][0]][1] = tour2[i][1]
        else:
            tab[tab[i][0]][0] = tab[i][0]
            tab[tab[i][0]][1] = tab[i][1]
        if(tour2[i][1] == 0):
            tab[tab[i][0]][0] = tab[i][0]
            tab[tab[i][0]][1] = tab[i][1]
        else:
            tab[tour2[i][0]][0] = tour2[i][0]
            tab[tour2[i][0]][1] = tour2[i][1]
        i+=1
    return tab

def position(tab):
    angleRadian = 0
    distance = 0
    rows, cols = (360,2)
    position = [[0 for i in range(cols)] for j in range(rows)]

    for i in range (len(position)):
        for k in range(len(tab)):
            angleRadian = math.radians(tab[k][0]) 
            distance = tab[k][1]
            position[tab[k][0]][0] = int(distance* math.cos(angleRadian))
            position[tab[k][0]][1] = int(distance* math.sin(angleRadian))
            i+=1
    return position

def data_final(data):

#analyse des datas

    premiere = data[0:1500]
    deuxieme = data[1500:3000]

    tour1 = tri_tour_data(premiere)
    tour2 = tri_tour_data(deuxieme)

    tab = list()
    rows, cols = (360,2)
    tab = [[0 for i in range(cols)] for j in range(rows)]

    tab = mix_tour(tab,tour1)
    tab = mix_tour(tab,tour2)

    return tab

def ecrire_file(fichier,position):
    with open (fichier,"w") as f:
        for k in range(len(position)):
            f.write(str(position[k][0]))
            f.write(",")
            f.write(str(position[k][1]))
            f.write(",")

#programme principal

#lire les datas du fichier texte 

data = lire_file("data_lidar.txt", 'utf_8')
#réception d'un buffer de 0 à 360 de l'angle et de la position trié
tab = data_final(data)

#on détermine la position (x,y) des obstacles 
position = position(tab)

#envoie du fichier position pour applicationo GTK REMOTE
ecrire_file("position_gtk.txt",position)
