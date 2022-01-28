#import numpy as np #pour creer une matrice/tableau
#import json

def fonctPrinc() :
    a=1.4
    b=1.0
    d=12.8
    c=[
        ["02:04:73:9D:3B:4A",a],          #b1
        ["02:05:82:06:25:C2",b],          #b2
        ["02:05:82:06:26:16",d]]         #b3
    # print(a)
    print("on entre dans le .py")
    conv = ','.join(str(elem) for elem in c)
    with open("position.txt", "w") as file:
        file.write(str(a))
        file.write(" ")
        file.write(str(b))
    
    return a

fonctPrinc()