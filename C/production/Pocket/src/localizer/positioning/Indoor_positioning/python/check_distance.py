import check_rssi
import subprocess
import statistics
import tri_partition

def check_distance_median(addrMac, x, size, Pt, Gt, Gr, pertes, synch_buf_start):
    distance_tab_tri = [[] for i in range(x)]
    distance = [100 for i in range(x)]
    rssi = [100 for i in range(x)]
    l = (3*10**8)/(2.4*10**9)
    rssi_tab = [[0 for i in range(size)] for i in range(x)]
    distance_tab = [[0 for i in range(size)] for i in range(x)]
    for i in range(size):
        subprocess.run(["./Indoor_positioning/bash/btmon_search.sh"])
        temp = check_rssi.check_RSSI(addrMac,x)
        for n in range(x):
            rssi_tab[n][i] = temp[n]
            if(rssi_tab[n][i] != 0):
                Pr=rssi_tab[n][i]
                inverse_distance = (4*3.14*10**((Pr-Pt-Gt-Gr-pertes)/20))/(l)
                distance_tab[n][i] = 1/(inverse_distance)
                #distance_tab[n][i] = 10**((-52.5-Pr)/(10*N))
            if(rssi_tab[n][i]==0):
                distance_tab[n][i] = distance_tab[n][i-1]
    for k in range(x):
        distance_tab_tri[k] = tri_partition.tri_partition_func(distance_tab[k], "Init", synch_buf_start)
    #print("distance_tab = "+str(distance_tab))
    print("distance_tab_tri = "+str(distance_tab_tri))
    #print("rssi_tab"+str(rssi_tab))
    for n in range(x):            
        rssi[n] = statistics.median(rssi_tab[n])
        if(len(distance_tab_tri[n]) == 0):
            distance[n] = 200
        else :
            distance[n] = statistics.median(distance_tab_tri[n])
    print("distance = "+str(distance))
    return distance