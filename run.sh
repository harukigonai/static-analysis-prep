#!/bin/bash
# rm -f out
# cd example_programs/1 && make clean && make main.o && cd ../..
# python3 ./vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf ./example_programs/1/main.o --schema temp_schema --out out

cd ./example_programs/2 && make all && cd ../..
cd ./extract_vars && ./run.sh && cd ..
python3 ./vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf /root/static-analysis-prep/example_programs/2/main --schema /tmp/vprof/schema.txt --out /tmp/vprof/var_info
cd ./example_programs/2 && ./run.sh && cd ../..

cd ./example_programs/2 && make all && cd ../..
cd ./extract_vars && ./run.sh && cd ..
python3 ./vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf /root/static-analysis-prep/example_programs/2/main --schema /tmp/vprof/schema.txt --out /tmp/vprof/var_info
cd ./example_programs/2 && ./run.sh && cd ../..
