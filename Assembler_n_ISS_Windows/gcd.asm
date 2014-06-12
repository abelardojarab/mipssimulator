addi r1,r0,15
addi r2,r0,9
addi r4,r0,1
back:
beq r1,r2,end
bge r1,r2,loop1
sub r2,r2,r1
j next
loop1:
sub r1,r1,r2
next:
j back
end:
exit
