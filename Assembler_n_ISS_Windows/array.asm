addi	r2, r0, 10		
addi	r3, r0, 0		
addi	r4, r0, 15		
addi	r5, r0, 30		
addi	r6, r0, 45		
addi	r7, r0, 60		
addi	r8, r0, 0		

init:
bge	r8, r2, doneit	

add	r9, r3, r8		
store r8,0(r9)

add	r9, r4, r8		
store r8,0(r9)

add	r9, r5, r8		
store r8,0(r9)


add	r9, r6, r8		
store r8,0(r9)	

add	r9, r7, r8		
store r8,0(r9)		

addi	r8, r8, 1		
j	init

doneit:


addi	r8, r0, 0		
loop1:
bge	r8, r2, endlop1	
	
add	r10, r5, r8		

add	r11, r3, r8		
load	r14, 0(r11)		

add	r12, r4, r8		
load r15, 0(r12)		

add	r13, r14, r15	
store r13,0(r10)	

addi	r8, r8, 1		
j	loop1			
	
endlop1:




addi	r8, r0, 0		
lp2:
bge	r8, r2, endel2 

add	r10, r6, r8		

add	r11, r5, r8		
load	r14, 0(r11)		

add	r12, r7, r8		
load	r15, 0(r12)		

add	r13, r14, 15		
store r13,0(r10)		

addi	r8, r8, 1		
j	lp2			
endel2:




addi	r8, r0, 0		
finale:
bge	r8, r2, endf
	
add	r11, r3, r8		
load	r14, 0(r11)		

add	r12, r4, r8		
load r15, 0(r12)		

add	r13, r14, 15		
store r13,0(r11)		

addi	r8, r8, 1		
j	finale			
endf:

exit




