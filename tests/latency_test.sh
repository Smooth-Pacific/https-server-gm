#!/bin/bash

curl    --cacert /usr/local/share/ca-certificates/root-ca.crt \
        --digest -u myuser:password \
        https://localhost:8081/slow \
        -w @- -o /dev/null -s "$@" <<'EOF'
time_namelookup:     %{time_namelookup}\n
time_connect:        %{time_connect}\n
time_appconnect:     %{time_appconnect}\n
time_pretransfer:    %{time_pretransfer}\n
time_redirect:       %{time_redirect}\n
time_starttransfer:  %{time_starttransfer}\n
-----------------------------\n
time_total:          %{time_total}\n
EOF