#!/bin/bash

case "$1" in
    start)
        ./ledserver;;
    stop)
        kill `cat var/ledserver.pid`;;
    *)
        echo "usage ./ledclient.sh start|stop";;
esac
