#!/bin/bash

[ ! -d "./results" ] && mkdir ./results

curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/largest > results/largest.xml
curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/merchant > results/merchant.xml
curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/fraud > results/fraud.xml
curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/types > results/types.xml
curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/insufficient > results/insufficient.xml
curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/state > results/state.xml
curl --cacert ~/work/ca/root-ca.crt --digest -u myuser:password https://172.17.0.2:8080/transaction/zipcode > results/zipcode.xml