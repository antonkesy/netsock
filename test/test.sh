#!/bin/bash

cd ../bin || exit

rm test -drf

mkdir -p test
cd test || exit

yes "random text" | head -n 1000 >test.me

#Test UDP
#v4
../netcopy 127.0.0.1 55000 -u -l >./udp_out_4.tmp & #! NOT CLOSING listener
../netcopy 127.0.0.1 55000 -u <./test.me

sleep 3s

diff -sq ./test.me ./udp_out_4.tmp

#v6
../netcopy ::1 55001 -u -l -6 >./udp_out_6.tmp & #! NOT CLOSING listener
../netcopy ::1 55001 -u -6 <./test.me

sleep 3s

diff -sq ./test.me ./udp_out_6.tmp

#Test TCP
#v4
../netcopy 127.0.0.1 55002 -t -l >./tcp_out_4.tmp &
../netcopy 127.0.0.1 55002 -t <./test.me

sleep 3s

diff -sq ./test.me ./tcp_out_4.tmp

#v6
../netcopy ::1 55003 -u -l -6 >./tcp_out_6.tmp &
../netcopy ::1 55003 -u -6 <./test.me

sleep 3s

diff -sq ./test.me ./tcp_out_6.tmp

cd ..
