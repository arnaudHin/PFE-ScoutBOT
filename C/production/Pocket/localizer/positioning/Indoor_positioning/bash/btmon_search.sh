#!/bin/bash

rm btsnoop_hci 
rm btsnoop_hci.txt

btmon -w btsnoop_hci > btsnoop_hci.txt &
PID=$!

hcitool lescan --privacy --passive --whitelist &
PID2=$!
sleep 1.5


kill -INT $PID
kill -INT $PID2
