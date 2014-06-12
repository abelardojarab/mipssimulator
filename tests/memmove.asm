addi r2, r0, 100
addi r3, r0, 20
addi r4, r0, 0 
loop1:
bge r4, r2 end1
add r5, r3, r4
store r4, 0(r5)
addi r4, r4, 1
j loop1
end1:
addi r4, r0, 0
move:
bge r4, r2, endmv
add r5, r3, r4
load r6, 0(r5)
addi r6, r6, 1
store r6, 0(r5)
addi r4, r4, 1
j move
endmv:
exit

