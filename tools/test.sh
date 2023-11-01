#!/bin/bash

for file in roms/gb-test-roms/cpu_instrs/individual/*; do
    echo $file;
    build/gb $file;
done;