#!/bin/bash

hciconfig reset
hciconfig hci0 up
hcitool lewladd 02:04:73:9D:3B:4A #B1
hcitool	lewladd 02:05:82:06:25:C2 #B2
hcitool	lewladd 02:05:82:06:26:16 #B4
hcitool lewladd 02:09:13:5D:1C:0E #B3
hcitool lewladd 02:04:73:9D:2F:2A #B5
hcitool lewladd 02:02:27:4E:39:5A #B6
hcitool lewladd 02:06:17:1E:1A:02 #B7
hcitool lewladd 02:02:27:4E:29:1E #B8
hcitool lewladd 02:04:88:16:3A:EE #B9
