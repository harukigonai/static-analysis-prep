#!/bin/bash
sudo rm -f /tmp/vprof/*
cd ../example_programs/2 && make clean && make bc && make main && cd ../../extract_vars
make
./main
# cat /tmp/vprof/schema.txt
python3 ../vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf /home/seclib/fix-project/static-analysis-prep/example_programs/2/main --schema /tmp/vprof/schema.txt --out /tmp/vprof/var_info