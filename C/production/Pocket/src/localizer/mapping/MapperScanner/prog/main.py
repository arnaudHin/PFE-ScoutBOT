import math
from tkinter import *

def lire_file(fichier, encodage):
    mots = []
    with open (fichier,encoding=encodage) as f:
        data = f.readline()
        mots = data.split(",")
    return mots


def affichage(pair,impair):
    # créer une fenêtre avec un canevas
    win = Tk()
    win.title("MAP_LIDAR")
    win.geometry("800x800")
    can1 = Canvas(win, width = 800, height = 800, bg = 'grey')
    can1.pack()
    x = 400
    y = 400
    DIM = 10
    #centre du repère
    can1.create_oval(x-DIM, y-DIM, x+DIM, y+DIM,fill='yellow')
    can1.create_line(400,0,400,800, fill = 'blue')
    can1.create_line(0,400,800,400, fill = 'blue')

    x = 400
    y = 400
    GRAD = 25
    #graduation 
    for k in range(0,16):
        can1.create_line(x+(k*GRAD),y-DIM,x+(k*GRAD),y+DIM, fill = 'blue')
        can1.create_line(x-(k*GRAD),y-DIM,x-(k*GRAD),y+DIM, fill = 'blue')
        can1.create_line(x-DIM,y+(k*GRAD),x+DIM,y+(k*GRAD), fill = 'blue')
        can1.create_line(x-DIM,y-(k*GRAD),x+DIM,y-(k*GRAD), fill = 'blue')

    can1.create_text(40,20,text="| 25 cm |")

    DIM = 3
    for i in range (len(pair)):
        x = (int(pair[i])/10) + 400
        y = (int(impair[i])/10) + 400
        can1.create_oval(x-DIM, y-DIM, x+DIM, y+DIM, fill='red')
    win.mainloop()


#programme principal

data = lire_file("../output/position_gtk.txt", 'utf_8')
x = data[0::2]
x.pop()
y = data[1::2]
affichage(x,y)











