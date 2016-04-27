#!/bin/bash
TEMPERATURE="/sys/class/thermal/thermal_zone0/temp"
COUNT=0
echo "#Temperature Recordings" > data
# bash while loop
while [ $COUNT -lt 40 ]; do
   echo $COUNT " " `cat $TEMPERATURE` >> data
   let COUNT=COUNT+1
   sleep 10
done
