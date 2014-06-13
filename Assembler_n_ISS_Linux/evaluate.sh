#!/bin/bash
if [ -f ``$1'' ]; then 
echo "******************************************"
echo "---------" "$1" "----------"
#echo "Running the Assembler."
assembler "$1" > tmp
simulator Asm_output_"$1" "$2" > log
#echo "Running the Simulator."
fi
grep CPI log
grep -i "Clock cycles used" log
grep -i "instruction count" log
grep -i "Stall Count" log
grep -i "Register File" log
grep -i "R0:" log
grep -i "R4:" log
grep -i "R8:" log
grep -i "R12:" log
grep -i "R16:" log
grep -i "R20:" log
grep -i "R24:" log
grep -i "R28:" log
echo ""
