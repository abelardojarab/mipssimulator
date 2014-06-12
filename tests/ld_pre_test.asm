addi r4, r0, 10
addi r2, r0, 0
loop: 
load r1, 0(r2)
addi r5, r1, 1112
store r5, 0(r2)
addi r2, r2, 1
bge r4, r2, loop
exit
