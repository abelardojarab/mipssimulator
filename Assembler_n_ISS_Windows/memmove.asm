addi r1, r0, 20 
addi r3, r0, 10
addi r2, r0, 0 
loop1:
store r2, 0(r3)
addi r2, r2, 1
addi r3, r2, 10
bge r1, r2, loop1
addi r2, r0, 0
addi r3, r0, 10
loop2:
load r4, 0(r3)
addi r4, r4, 1
store r4, 0(r3)
addi r2, r2, 1
addi r3, r2, 10
bge r1, r2, loop2
exit

