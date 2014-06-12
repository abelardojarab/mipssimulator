addi r2, r0, 24
addi r3, r0, 0
back: 
addi r3, r3, 3
bge r2, r3, back
subi r3, r3, 3
addi r5, r0, 0
bne r3, r2, end
addi r5, r0, 2
end:
exit
