#!/usr/bin/env python3

import threading
import requests
import random
from requests.auth import HTTPDigestAuth
from datetime import datetime
from urllib3.exceptions import InsecureRequestWarning

HOST = "172.17.0.2"
PORT = "8081"
USER = "myuser"
PASSWORD = "password"
CERT_PATH = "../../../ca/root-ca/private/root-ca.crt"
NUM_REQUESTS = 100000
NUM_THREADS = 4

ENDPOINT_LIST = [
    "hello", "transaction/largest", "transaction/merchant", "transaction/fraud",
    "transaction/types", "transaction/insufficient", "transaction/state", 
    "transaction/zipcode", "transaction/lowest_online", "transaction/city_with_online",
    "transaction/most_transactions_by_city", "transaction/deposits", 
    "transaction/bottom_online_transaction_months", "transaction/top_merchant_insufficient_balance",
    "transaction/wrong"
]


def requestThread():
    for i in range(NUM_REQUESTS):
        s = requests.Session()
        if (i % 1000) == 0:
            s.auth = HTTPDigestAuth(USER, "wrong pass")
        elif (i % 498) == 0:
            s.auth = HTTPDigestAuth("wrong_user", PASSWORD)
        else:
            s.auth = HTTPDigestAuth(USER, PASSWORD)
        #s.verify = CERT_PATH
        #s.get(f"https://{HOST}:{PORT}/{random.choice(ENDPOINT_LIST)}")
        endpoint = random.choice(ENDPOINT_LIST)
        resp = s.get(("https://172.17.0.2:8081/" + endpoint), verify=False)


if __name__ == "__main__":
    requests.packages.urllib3.disable_warnings(category=InsecureRequestWarning)
    threads = []

    for i in range(NUM_THREADS):
        thread = threading.Thread(target=requestThread)
        thread.start()
        threads.append(thread)
    
    for t in threads:
        t.join()