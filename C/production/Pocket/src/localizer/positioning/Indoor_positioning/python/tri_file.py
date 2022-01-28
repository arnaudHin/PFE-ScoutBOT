def tri(tab,N):
    pos=0
    tab_tri = tab
    for i in range(N-1) :
        min = i
        for j in range(i,N):
            if(abs(tab_tri[j][1])<abs(tab_tri[min][1])):
                min = j
        temp = tab_tri[min]
        tab_tri[min] = tab_tri[i]
        tab_tri[i] = temp
    return tab_tri


def tri_V2(tab_totri):
    tab_tri = tab_totri

    #piece 1 :
    if(tab_tri[0][0]!="B1" or tab_tri[0][0]!="B2" or tab_tri[0][0]!="B3" or tab_tri[0][0]!="B4"):
        if(tab_tri[1][0]=="B1" or tab_tri[1][0]=="B2" or tab_tri[1][0]=="B3" or tab_tri[1][0]=="B4"):
            if(tab_tri[2][0] == "B1" or tab_tri[2][0] == "B2" or tab_tri[2][0]=="B3" or tab_tri[2][0]=="B4"):
                if(tab_tri[3][0] == "B1" or tab_tri[3][0] == "B2" or tab_tri[3][0]=="B3" or tab_tri[3][0]=="B4"):
                    for i in range(4):
                        if (tab_tri[i+4][0] == "B2" or tab_tri[i+4][0] =="B3" or tab_tri[i+4][0] =="B4"):
                            tab_tri[0] = tab_tri[i+4]

    if(tab_tri[0][0]=="B1" or tab_tri[0][0]=="B2" or tab_tri[0][0]=="B3" or tab_tri[0][0]=="B4"):
        if(tab_tri[1][0]=="B1" or tab_tri[1][0]=="B2" or tab_tri[1][0]=="B3" or tab_tri[1][0]=="B4"):
            if(tab_tri[2][0] != "B1" and tab_tri[2][0] != "B2" and tab_tri[2][0] !="B3" and tab_tri[2][0] !="B4"):
                for i in range(4):
                    if (tab_tri[i+4][0] == "B1" or tab_tri[i+4][0] =="B2" or tab_tri[i+4][0] =="B3" or tab_tri[i+4][0] =="B4"):
                        tab_tri[2] = tab_tri[i+4]
            if(tab_tri[2][0]== "B1" or tab_tri[2][0]== "B2" or tab_tri[2][0] =="B3" or tab_tri[2][0] =="B4"):
                if(tab_tri[3][0]==tab_tri[2][0]):
                    for i in range(4):
                        if ((tab_tri[i+4][0] == tab_tri[i+4][0] =="B2" or tab_tri[i+4][0] =="B3" or tab_tri[i+4][0] =="B4") and tab_tri[i+4][0]!= tab_tri[2][0]):
                            tab_tri[3] = tab_tri[i+4]

                if(tab_tri[3][0] != "B1" and tab_tri[3][0] != "B2" and tab_tri[3][0] !="B3" and tab_tri[3][0] !="B4"):
                    for i in range(4):
                        if ((tab_tri[i+4][0]== tab_tri[i+4][0]=="B2" or tab_tri[i+4][0]=="B3" or tab_tri[i+4][0]=="B4") and tab_tri[i+4][0]!= tab_tri[2][0]):
                            tab_tri[3] = tab_tri[i+4]
                        

        if(tab_tri[2][0]=="B1" or tab_tri[2][0]=="B2" or tab_tri[2][0]=="B3" or tab_tri[2][0]=="B4"):
            if(tab_tri[1][0] != "B1" and tab_tri[1][0] != "B2" and tab_tri[1][0]!="B3" and tab_tri[1][0]!="B4"):
                for i in range(4):
                    if(tab_tri[i+4][0] == "B1" or tab_tri[i+4][0] == "B2" or tab_tri[i+4][0] =="B3" or tab_tri[i+4][0] =="B4"):
                        tab_tri[1] = tab_tri[i+4]
            if(tab_tri[3][0] != "B1" and tab_tri[3][0] != "B2" and tab_tri[3][0]!="B3" and tab_tri[3][0]!="B4"):
                for i in range(4):
                    if((tab_tri[i+4][0] == "B1" or tab_tri[i+4][0] == "B2" or tab_tri[i+4][0] =="B3" or tab_tri[i+4][0] =="B4") and tab_tri[i+4][0]!=tab_tri[2][0]):
                        tab_tri[3] = tab_tri[i+4]
    
    

    #Piece 2 :
    if(tab_tri[0][0]!="B5" or tab_tri[0][0]!="B6" or tab_tri[0][0]!="B7" or tab_tri[0][0]!="B8"):
        if(tab_tri[1][0]=="B5" or tab_tri[1][0]=="B6" or tab_tri[1][0]=="B7" or tab_tri[1][0]=="B8"):
            if(tab_tri[2][0] == "B5" or tab_tri[2][0] == "B6" or tab_tri[2][0]=="B7" or tab_tri[2][0]=="B8"):
                if(tab_tri[3][0] == "B5" or tab_tri[3][0] == "B6" or tab_tri[3][0]=="B7" or tab_tri[3][0]=="B8"):
                    for i in range(4):
                        if (tab_tri[i+4][0] == "B6" or tab_tri[i+4][0] =="B7" or tab_tri[i+4][0] =="B8"):
                            tab_tri[0] = tab_tri[i+3]

    if(tab_tri[0][0]=="B5" or tab_tri[0][0]=="B6" or tab_tri[0][0]=="B7" or tab_tri[0][0]=="B8"):
        if(tab_tri[1][0]=="B5" or tab_tri[1][0]=="B6" or tab_tri[1][0]=="B7" or tab_tri[1][0]=="B8"):
            if(tab_tri[2][0] != "B5" and tab_tri[2][0] != "B6" and tab_tri[2][0] !="B7" and tab_tri[2][0] !="B8"):
                for i in range(4):
                    if (tab_tri[i+4][0] == "B5" or tab_tri[i+4][0] =="B6" or tab_tri[i+4][0] =="B7" or tab_tri[i+4][0] =="B8"):
                        tab_tri[2] = tab_tri[i+4]
            if(tab_tri[2][0]== "B5" or tab_tri[2][0]== "B6" or tab_tri[2][0] =="B7" or tab_tri[2][0] =="B8"):
                if(tab_tri[3][0]==tab_tri[2][0]):
                    for i in range(4):
                        if ((tab_tri[i+4][0] == tab_tri[i+4][0] =="B6" or tab_tri[i+4][0] =="B7" or tab_tri[i+4][0] =="B8") and tab_tri[i+4][0]!= tab_tri[2][0]):
                            tab_tri[3] = tab_tri[i+4]

                if(tab_tri[3][0] != "B5" and tab_tri[3][0] != "B6" and tab_tri[3][0] !="B7" and tab_tri[3][0] !="B8"):
                    for i in range(4):
                        if ((tab_tri[i+4][0]== tab_tri[i+4][0]=="B6" or tab_tri[i+4][0]=="B7" or tab_tri[i+4][0]=="B8") and tab_tri[i+4][0]!= tab_tri[2][0]):
                            tab_tri[3] = tab_tri[i+4]
                        

        if(tab_tri[2][0]=="B5" or tab_tri[2][0]=="B6" or tab_tri[2][0]=="B7" or tab_tri[2][0]=="B8"):
            if(tab_tri[1][0] != "B5" and tab_tri[1][0] != "B6" and tab_tri[1][0]!="B7" and tab_tri[1][0]!="B8"):
                for i in range(4):
                    if(tab_tri[i+4][0] == "B5" or tab_tri[i+4][0] == "B6" or tab_tri[i+4][0] =="B7" or tab_tri[i+4][0] =="B8"):
                        tab_tri[1] = tab_tri[i+4]
            if(tab_tri[3][0] != "B5" and tab_tri[3][0] != "B6" and tab_tri[3][0]!="B7" and tab_tri[3][0]!="B8"):
                for i in range(4):
                    if((tab_tri[i+4][0] == "B5" or tab_tri[i+4][0] == "B6" or tab_tri[i+4][0] =="B7" or tab_tri[i+4][0] =="B8") and tab_tri[i+4][0]!=tab_tri[2][0]):
                        tab_tri[3] = tab_tri[i+4]
    print("tab_tri_v2 = " +str(tab_tri))
    return tab_tri