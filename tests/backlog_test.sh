#!/bin/bash

LOG_FILE="backlog_test.log"
LOG_FILE_PATH="../logs"

send_test() {
    _inst=$1
    #_res=$(curl --cacert /usr/local/share/ca-certificates/root-ca.crt --digest -u myuser:password https://172.17.0.2:8081/hello 2>&1)
    _res=$(curl --cacert /usr/local/share/ca-certificates/root-ca.crt --digest -u myuser:password https://172.17.0.2:8081/hello 2>&1)
    _code=$?
    netstat -i >> $LOG_FILE_PATH/$LOG_FILE
    if [ $_code -ne 0 ]; then
        echo "Failed to send request $_inst (return code: $_code)"
        echo "Response: $_res"
        exit $_code
    else
        echo "Successful Response for Instance $_inst"
    fi
}

#send_test 1

for i in {1..30}; do
    send_test "$i" &
done

# Watch performance to see how performance is effected when backlog fills
#mpstat -P ALL
#iostat -c
#sudo iftop -Nnts 1