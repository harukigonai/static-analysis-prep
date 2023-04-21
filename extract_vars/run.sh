#!/bin/bash
sudo rm -f /tmp/vprof/*
sudo mkdir /tmp/vprof
make
./main
# cat /tmp/vprof/schema.txt
