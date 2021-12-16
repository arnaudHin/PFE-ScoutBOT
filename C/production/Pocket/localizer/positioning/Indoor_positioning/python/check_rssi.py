def check_RSSI(addrMac,x):
    i=0
    j=0
    block_MACaddr = ""
    RSSI_number = [0 for i in range(x)]
    with open('Indoor_positioning/bash/btsnoop_hci.txt') as temp_f:
        datafile = temp_f.read().split('HCI Event: LE Meta Event')
    for block in datafile:
        for k in range(0,x):
            if addrMac[k] in block:
                block_MACaddr = datafile[i]
                block_MACaddr_split = block_MACaddr.split(' ')
                j=0
                for rssi in block_MACaddr_split :
                    if 'RSSI' in rssi:
                        RSSI_number[k]=int(block_MACaddr_split[j+1])
                    j=j+1
        i = i + 1
    return RSSI_number