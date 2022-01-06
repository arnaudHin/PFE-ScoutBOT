
import subprocess
import statistics
import random
import tri_partition
import check_distance
import tri_file
import position

Pt = 6
Gr = 0.6
Gt = 1.95
pertes = -30
x=8
N = 4
size = 10


#Piece 1 : B1, B2, B3, B4
#Piece 2 : B5, B6, B7, B8 


            #ici B9                                                                                                                                                                              #ici B9  
                #B1                    #B2                     #B3                 #B4                     #B5                     #B6             #B7                     #B8                     #B9
addrMac = ["02:04:88:16:3A:EE" , "02:09:13:5D:1C:0E" , "02:05:82:06:25:C2" ,  "02:05:82:06:26:16", "02:04:73:9D:2F:2A" , "02:02:27:4E:39:5A", "02:06:17:1E:1A:02" , "02:02:27:4E:29:1E" , "02:04:73:9D:3B:4A"]

synch_buf_start = [[], [], [], 0, 0, 0, 0.0, 0.0, 0.0, 0.0]




def save_in_file(data_1, data_2,data_3):
    file = open("Indoor_positioning/results/data_indoor_pos.txt", "w")
    file.write(str(data_1)+" "+str(data_2)+" "+str(data_3))
    return 0


if __name__ == "__main__":
    B = ["B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8"]
    data = check_distance.check_distance_median(addrMac, x, size, Pt, Gt, Gr, pertes, synch_buf_start)
    tab_data = [[B[0],data[0]],[B[1],data[1]],[B[2],data[2]],[B[3],data[3]], [B[4],data[4]], [B[5],data[5]], [B[6],data[6]], [B[7],data[7]]]
    tab_pos_robot = position.maillage(tab_data,x)
    id_place = position.wich_place_func(tab_pos_robot)
    save_in_file(tab_pos_robot[0], tab_pos_robot[1], id_place)



