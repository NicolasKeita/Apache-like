#!/bin/sh

./test_serv > /dev/null 2>&1 &
./test_client >> log_client.txt 2>&1 &
sleep 4
FIRST_TIME_RESULT=$(grep "FIRST TEST SUCCESSFULLY COMPLETED" log_client.txt)
SECOND_TIME_RESULT=$(grep "SECOND TEST" log_client.txt)
rm log_client.txt
if [ -n "$FIRST_TIME_RESULT" ] && [ -n "$SECOND_TIME_RESULT" ] ; then
exit 0
else
exit 1
fi
