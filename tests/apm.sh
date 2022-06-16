#!/bin/bash

LOG_FILE="apm.log"
LOG_FILE_PATH="../logs"
LOG_VALGRIND='apm_valgrind.log'



echo "Application Performance Monitor"
echo "----- Application Performance Monitoring -----" > $LOG_FILE_PATH/$LOG_FILE
echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - CPU Stats"
echo "-------------- CPU Statistics ----------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "----------------- iostat -c ------------------" >> $LOG_FILE_PATH/$LOG_FILE
iostat -c >> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Disk I/O Stats"
echo "------------ Disk I/O Statistics -------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "----------------- iostat -d ------------------" >> $LOG_FILE_PATH/$LOG_FILE
iostat -d >> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Virtual Memory Stats"
echo "--------- Virtual Memory Statistics ----------" >> $LOG_FILE_PATH/$LOG_FILE
echo "----------------- vmstat -w ------------------" >> $LOG_FILE_PATH/$LOG_FILE
vmstat -w >> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Processor Stats"
echo "----------- Processor Statistics -------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "--------------- mpstat -P ALL ----------------" >> $LOG_FILE_PATH/$LOG_FILE
mpstat -P ALL >> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Network Stats"
echo "------------ Network Statistics --------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "------------- sudo iftop -Nnts 1  ------------" >> $LOG_FILE_PATH/$LOG_FILE
sudo iftop -Nnts 1 &>> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Swap Memory Stats"
echo "---------- Swap Memory Statistics ------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "------------------- free ---------------------" >> $LOG_FILE_PATH/$LOG_FILE
free >> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Memory Info"
echo "------------------- Memory Info ---------------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "------------- sudo cat /proc/meminfo ----------------" >> $LOG_FILE_PATH/$LOG_FILE
sudo cat /proc/meminfo >> $LOG_FILE_PATH/$LOG_FILE


echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - CPU Info"
echo "--------------------- CPU Info ----------------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "------------- sudo cat /proc/cpuinfo ----------------" >> $LOG_FILE_PATH/$LOG_FILE
sudo cat /proc/cpuinfo >> $LOG_FILE_PATH/$LOG_FILE

echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "  - Valgrind Stats"
echo "------------ Valgrind Statistics -------------" >> $LOG_FILE_PATH/$LOG_FILE
echo "------------------- valgrind ---------------------" >> $LOG_FILE_PATH/$LOG_FILE
sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$LOG_FILE_PATH/$LOG_VALGRIND ../server.exe &>/dev/null 2>&1
grep "LEAK SUMMARY:" $LOG_FILE_PATH/$LOG_VALGRIND -A 6 >> $LOG_FILE_PATH/$LOG_FILE
sudo rm -rf logs