#!/bin/bash
if [ -f ``$1'' ]; then 
echo "Running the Assembler."
assembler "$1"
simulator Asm_output_"$1" "$2"
echo "Running the Simulator."
fi 
