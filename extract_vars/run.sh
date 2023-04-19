#!/bin/bash
sudo rm -f /tmp/vprof/*
make
./main
# cat /tmp/vprof/schema.txt
