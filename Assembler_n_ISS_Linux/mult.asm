 addi r2, r0, 4
 addi r7, r0, 20
 addi r8, r0, 100 
 addi r9, r0, 120	
 addi r3, r0, 0
 begina:
 bge r3, r2, endloopa
 addi r4, r0, 0
 beginb:
 bge r4, r2, endloopb
 mult r3, r2
 add r10, r30, r4
 add r11, r7, r10
 store r3, 0(r11)
 add r12, r8, r10
 store r4, 0(r12)
 addi r4, r4, 1	
 j beginb	
 endloopb:
 addi r3, r3, 1
 j begina
 endloopa:
 addi r3, r0, 0	
 loop1:
 bge r3, r2, endlop1
 addi r4, r0, 0	
 loop2:
 bge r4, r2, endlop2
 addi r6, r0, 0	
 addi r5, r0, 0
 loop3:
 bge r5, r2, endlop3
 mult r3, r2, r30
 add r13, r30, r5
 add r14, r13, r7
 load r15, 0(r14)
 mult r5, r2
 add r16, r30, r4
 add r17, r16, r8
 load r18, 0(r17)
 mult r15, r18
 add r6, r6, r30
 addi r5, r5, 1
 j loop3
 endlop3:
 mult r3, r2
 add r20, r30, r4
 add r21, r20, r9
 store r6, 0(r21)
 addi r4, r4, 1
 j loop2
 endlop2:
 addi r3, r3, 1
 j loop1
 endlop1:
 exit
