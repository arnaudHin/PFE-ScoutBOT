#!/bin/bash

rm Indoor_positioning/bash/btsnoop_hci 
rm Indoor_positioning/bash/btsnoop_hci.txt

btmon -w Indoor_positioning/bash/btsnoop_hci > Indoor_positioning/bash/btsnoop_hci.txt &
PID=$!

hcitool lescan --privacy --passive --whitelist &
PID2=$!
sleep 1.5


kill -INT $PID
kill -INT $PID2
