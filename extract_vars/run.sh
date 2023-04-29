#!/bin/bash
sudo rm -f /tmp/vprof/*
sudo mkdir /tmp/vprof
make
./main ../untrusted_lib_tester/main.bc
mv /tmp/vprof/schema.txt /tmp/vprof/schema_app.txt
./main ../untrusted_lib_tester/fake_lib.so.bc
mv /tmp/vprof/schema.txt /tmp/vprof/schema_lib.txt
