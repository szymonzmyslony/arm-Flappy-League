mov r0, #0x00040000
ldr r1, =0x20200000
str r0, [r1,#4]
mov r11, #0
mov r7, #4

bigLoop:
cmp r11, r7
bge endBigLoop
mov r9, #0

mov r0, #0x00010000
ldr r1, =0x20200000
str r0, [r1,#0x28]
ldr r1, =0x20200000
str r0, [r1,#0x1c]

insideLoop:
ldr r1, =0x20200004

mov r5, #0
ldr r6, =0xFFF ; wait 5 times

loop:
cmp r5, r6
bgt endLoop
str r5, [r1]
add r5, r5, #1
b loop

endLoop:
add r9, r9, #1
ldr r1, =0x20200000
str r0, [r1,#0x28]
cmp r9, #1
beq insideLoop
add r11, r11, #1
b bigLoop

endBigLoop:
andeq r0,r0,r0
