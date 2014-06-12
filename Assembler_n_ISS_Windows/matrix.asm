addi	r2, r0, 4
addi	r1, r0, 3
addi	r7, r0, 20		
addi	r8, r0, 40		
addi	r9, r0, 60		
addi	r3, r0, 0		

begina:	
bge	r3, r2, endloopa	
addi	r4, r0, 0		

beginb:	
bge	r4, r2, endloopb	
mult	r3, r2			
add	r10, r30, r4

add	r11, r7, r10		
store r3, 0(r11)		

add	r12, r8, r10		
store r4, 0(r12)

addi	r4, r4, 1		
j	beginb			

endloopb:	
addi	r3, r3, 1		
j	begina			

endloopa:	
addi	r4, r0, 1		

loopa1:
bge	r4, r1, endloop1	
addi	r3, r0, 1
		
loopa2:
bge	r3, r1, endloop2	
addi r29, r29, 1	
subi	r4, r4, 1
mult	r3, r2
add	r13, r30, r4
add	r14, r13, r7
load	r15, 0(r14) 
addi	r4, r4, 2
mult	r3, r2
add	r13, r30, r4
add	r14, r13, r7
load	r20, 0(r14) 
addi	r3, r3, 1
mult	r3, r2
add	r13, r30, r4
add	r14, r13, r7
load	r21, 0(r14) 
subi	r3, r3, 2
mult	r3, r2
add	r13, r30, r4
add	r14, r13, r7
load	r22, 0(r14)  
add r23, r15, r20
add r24, r21, r22
add r25, r24, 23  
addi r26, r0, 4
div r25, r26 
addi r27, r30, 0

subi	r4, r4, 1
addi	r3, r3, 1
mult	r3, r2
add	r19, r30, r4		
add	r17, r19, r8 
store r27, 0(r17)

addi	r3, r3, 1
j	loopa2
endloop2:

addi	r4, r4, 1
j	loopa1
endloop1:
exit
