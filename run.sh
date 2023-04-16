#!/bin/bash
rm -f out
cd example_programs/1 && make clean && make main.o && cd ../..
python3 ./vprofAE/LLVMPassSchemaGen/translate_schema_multiprocessing.py --elf ./example_programs/1/main.o --schema temp_schema --out out