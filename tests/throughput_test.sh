#!/bin/bash

LOG_FILE="data_throughput.log"
LOG_FILE_PATH="../logs"

echo "Throughput Tests"

#echo "----- Request Throughput Test -----" > $LOG_FILE_PATH/$LOG_FILE
#clang++ std=c++17 request.cpp -o request.exe -lpthread
#./request.exe >> $LOG_FILE_PATH/$LOG_FILE


#echo "----- Response Throughput Test -----" > $LOG_FILE_PATH/$LOG_FILE
#clang++ std=c++17 request.cpp -o request.exe -lpthread
#./request.exe >> $LOG_FILE_PATH/$LOG_FILE


#rm *.o *.exe 


echo "----- Data Throughput Test -----" > $LOG_FILE_PATH/$LOG_FILE

echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "----- Fio Test - 512MB R/W - 64K blocksize -----" >> $LOG_FILE_PATH/$LOG_FILE
fio --name=temp/rw_64k \
    --rw=rw \
    --direct=1 \
    --ioengine=libaio \
    --bs=64k \
    --numjobs=8 \
    --size=512m \
    --runtime=60s \
    --time_based \
    --group_reporting >> $LOG_FILE_PATH/$LOG_FILE

rm -rf temp

echo -e "\n\n\n" >> $LOG_FILE_PATH/$LOG_FILE
echo "----- Fio Test - 1G R/W - 16K blocksize -----" >> $LOG_FILE_PATH/$LOG_FILE
fio --name=temp/rw_64k \
    --rw=rw \
    --direct=1 \
    --ioengine=libaio \
    --bs=16k \
    --numjobs=8 \
    --size=1G \
    --runtime=60s \
    --time_based \
    --group_reporting >> $LOG_FILE_PATH/$LOG_FILE

rm -rf temp