addi r4, r0, 8
addi r1, r0, 3		
addi r2, r0, 1		
addi r3, r0, 1		
loop:
addi r1, r1, 1		
add r5, r2, r3		
add r2, r0, r3		
add r3, r0, r5		
bge r4, r1, loop
exit

