import tri_file

                #ici B9 
tab_point = [["B1",[0,0]] , ["B2",[6,0]] , ["B3",[0,6]] , ["B4",[6,6]] , ["B5",[1,1]] , ["B6",[4.7,0]] , ["B7",[0,4.3]] , ["B8",[0,0]]]



def coord_point(point):
    for i in tab_point:
        if(point == i[0]):
            tab_return = i[1]
    return tab_return


def maillage(tableau,x):
    #tab = get_Distance()  # tab = [[B1,Distance],[B2,Distance],...]
    tab_recup = [0,0,0]
    tab_save = [0,0,0]
    tab_XY = [0,0, []]
    tab_v1 = tri_file.tri(tableau,x)
    tab = tri_file.tri_V2(tab_v1)
    tab_recup[0] = tab[0][0]
    tab_recup[1] = tab[1][0]
    tab_recup[2] = tab[2][0]
    
    print("tab_tri = "+str(tab))
    B = [coord_point(tab[0][0]), tab[0][1]]
    A = [coord_point(tab[1][0]), tab[1][1]]
    C = [coord_point(tab[2][0]), tab[2][1]]
    print(A)
    print(B)
    print(C)
    k=3

    while(A[0][1]==B[0][1]==C[0][1]):
        C = [coord_point(tab[k][0]), tab[k][1]]
        k = k + 1
    if(A[0][1]==C[0][1]):
        temp = B
        B = C
        C = temp
    elif(C[0][1]==B[0][1]):
        temp = A
        A = C
        C = temp

    # X[0][0]=Xx ; X[0][1]=Xy
    a = (C[0][0]-A[0][0])/(A[0][1]-C[0][1])
    b = (A[0][0]**2+A[0][1]**2-A[1]**2-C[0][0]**2-C[0][1]**2+C[1]**2)/(2*(A[0][1]-C[0][1]))
    c = (B[0][0]-C[0][0])/(C[0][1]-B[0][1])
    d = (C[0][0]**2+C[0][1]**2-C[1]**2-B[0][0]**2-B[0][1]**2+B[1]**2)/(2*(C[0][1]-B[0][1]))

    x_robot = (d-b)/(a-c)
    y_robot = (a*x_robot + b)

    print("X = "+str(x_robot))
    print("Y = "+str(y_robot))

    tab_XY[0] = x_robot
    tab_XY[1] = y_robot
    tab_XY[2] = tab
    return tab_XY


def wich_place_func(pos_robot):
    x_robot = pos_robot[0]
    y_robot = pos_robot[1]
    tab_tri = pos_robot[2]
    id_pos = 0

    for i in range(3):
        if(tab_tri[0][0]=="B2" or tab_tri[0][0]=="B3" or tab_tri[0][0]=="B4"):
            if(tab_tri[1][0]=="B2" or tab_tri[1][0]=="B3" or tab_tri[1][0]=="B4" or tab_tri[2][0]=="B2" or tab_tri[2][0]=="B3" or tab_tri[2][0]=="B4"):
                id_pos = 1
        if(tab_tri[0][0]=="B6" or tab_tri[0][0]=="B7" or tab_tri[0][0]=="B8"):
            if(tab_tri[1][0]=="B6" or tab_tri[1][0]=="B7" or tab_tri[1][0]=="B8" or tab_tri[2][0]=="B6" or tab_tri[2][0]=="B7" or tab_tri[2][0]=="B8"):
                id_pos = 2
    print("Id_pos = "+str(id_pos))
    return id_pos