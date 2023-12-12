#!/bin/bash

cd build || exit

rm test -drf

mkdir -p test
cd test || exit

yes "random text" | head -n 1000 > test.me

echo "Test UDP IPv4"
../netsock 127.0.0.1 55000 -u -l > ./udp_out_4.tmp & #! NOT CLOSING listener
../netsock 127.0.0.1 55000 -u < ./test.me

sleep 3s

diff -sq ./test.me ./udp_out_4.tmp

echo "Test UDP IPv6"
../netsock ::1 55001 -u -l -6 > ./udp_out_6.tmp & #! NOT CLOSING listener
../netsock ::1 55001 -u -6 < ./test.me

sleep 3s

diff -sq ./test.me ./udp_out_6.tmp

echo "Test TCP IPv4"
../netsock 127.0.0.1 55002 -t -l > ./tcp_out_4.tmp &
../netsock 127.0.0.1 55002 -t < ./test.me

sleep 3s

diff -sq ./test.me ./tcp_out_4.tmp

echo "Test TCP IPv6"
../netsock ::1 55003 -u -l -6 > ./tcp_out_6.tmp &
../netsock ::1 55003 -u -6 < ./test.me

sleep 3s

diff -sq ./test.me ./tcp_out_6.tmp

echo "Test Local Host && Auto IPv Selection"
../netsock localhost 55004 -u -l -5 > ./autohost.tmp & #! NOT CLOSING listener
../netsock ::1 55004 -u < ./test.me

sleep 3s

diff -sq ./test.me ./autohost.tmp

cd ..
