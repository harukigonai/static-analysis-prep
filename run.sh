#!/bin/bash

cd ./untrusted_lib_tester && make all LLVM_COMPILER=clang && cd ..
cd ./extract_vars && ./run.sh && cd ..
python3 ./vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf /root/static-analysis-prep/untrusted_lib_tester/main --schema /tmp/vprof/schema_app.txt --out /tmp/vprof/var_info_app

python3 ./vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf /root/static-analysis-prep/untrusted_lib_tester/fake_lib.so --schema /tmp/vprof/schema_lib.txt --out /tmp/vprof/var_info_lib

cd ./untrusted_lib_tester
./init_2
cd ..

