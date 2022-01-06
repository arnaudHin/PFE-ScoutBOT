
# 1. The decreased collection (DC) represents singular weakened RSSIs such as blocked and reflected signals.
# 2. The normal collection (NC) represents normal path-loss and fading RSSIs
# 3. The increased collection (IC) represents singular enhanced RSSIs caused by transmitting equipment such as antenna gained power or transmitting power mutation

#Input in function
#from typing import no_type_check


#RSSI_list = [random.sample(range(-70,-30), 10) for i in range(3)] #[-50,-48,-49,-51,-50,-47,-53,-49,-50,-48,-51]  #the collected RSSI samples (~10 samples)
#Output of the function --> NC, IC, DC


#init 
IC = []
NC = []
DC = []

tab_return = [[],[],[]]

synch_buf_start = [[], [], [], 0, 0, 0, 0.0, 0.0, 0.0, 0.0]


def average_func(list):
    if(len(list) == 0):
        return 0
    else:
        mean = sum(list)/len(list)
    return mean



def tri_partition_func(RSSI_list, state, buf):

    global IC 
    global NC
    global DC
    global tab_return 

    if(state=="Init"):
        IC = []
        NC = []
        DC = []

        ICt = []
        NCt = []
        DCt = []

        IV = max(RSSI_list)
        NV = average_func(RSSI_list)
        DV = min(RSSI_list)

        TD=ID=DD=ND=0.0

        #ICt = synch_buf[0]
        #NCt = synch_buf[1]
        #DCt = synch_buf[2]
        #IV = synch_buf[3]
        #NV = synch_buf[4]
        #DV = synch_buf[5]
        #TD = synch_buf[6]
        #ID = synch_buf[7]
        #DD = synch_buf[8]
        #ND = synch_buf[9]
        synch_buf_init = [ICt, NCt, DCt, IV, NV, DV, TD, ID, DD, ND]

        tri_partition_func(RSSI_list, "Classify", synch_buf_init)


    #Calculate its distance to IV, NV, DV
    if(state=="Classify"):
        j=0
        list_TV = [0,0,0]
        for rssi in RSSI_list:
            ID = abs(buf[3]-rssi) #IV
            ND = abs(buf[4]-rssi) #NV
            DD = abs(buf[5]-rssi) #DV
            list_TV[0] = ID
            list_TV[1] = ND
            list_TV[2] = DD

            #select the minimum distance 
            TD = min(list_TV)

            #add RSSIi=rssi to sub-classification 
            #variables globales (IC, NC, DC)
            if(TD==ID):
                IC.append(rssi) 
            elif(TD==ND):
                NC.append(rssi)
            else:
                DC.append(rssi)
            j=j+1
        #End of for loop
        synch_buf_classify = [buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], TD, ID, DD, ND]
        tri_partition_func(RSSI_list, "Judge", synch_buf_classify)

    if(state=="Judge"):
        #Judge the change of sub-classification
        ICt = buf[0]
        NCt = buf[1]
        DCt = buf[2]
        if(ICt == NCt == DCt == NC):
            ICt = IC
            NCt = NC
            DCt = DC
            synch_buf_reset_V1 = [ICt, NCt, DCt, buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9]]
            tri_partition_func(RSSI_list, "Reset", synch_buf_reset_V1)

        #if convergence, exit
        if(ICt==IC and NCt==NC and DCt==DC):
            synch_buf_reset_V2 = [ICt, NCt, DCt, buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9]]
            tri_partition_func(RSSI_list, "Exit", synch_buf_reset_V2)
        else:
            ICt = IC
            NCt = NC
            DCt = DC
            synch_buf_reset_V3 = [ICt, NCt, DCt, buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9]]
            tri_partition_func(RSSI_list, "Exit", synch_buf_reset_V3)


    if(state=="Reset"):
        #Reset : reser the sub classification center
        IV = average_func(IC)
        NV = average_func(NC)
        DV = average_func(DC)
        synch_buf_reset_reset = [buf[0], buf[1], buf[2], IV, NV, DV, buf[6], buf[7], buf[8], buf[9]]
        tri_partition_func(RSSI_list, "Classify", synch_buf_reset_reset)

    if(state=="Exit"):
        #deal with NC
        tab = [0,0,0]
        TD = buf[6]
        DD = buf[8]
        ID = buf[7]
        if(NC==0):
            TD=(ID+DD)/2
            NC = TD
        tab_return[0]  = IC
        tab_return[1]  = NC
        tab_return[2]  = DC
    return tab_return[1]