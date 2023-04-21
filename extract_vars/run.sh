#!/bin/bash
sudo rm -f /tmp/vprof/*
sudo mkdir /tmp/vprof
make
./main ../example_programs/2/main.bc
mv /tmp/vprof/schema.txt ./schema_main_bc.txt
./main

