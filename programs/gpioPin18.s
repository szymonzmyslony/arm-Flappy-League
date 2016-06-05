mov r0, #0x01000000
ldr r1, =0x20200004
ldr r2, [r1]
orr r3, r0, r2
str r3, [r1]

mov r0, #0x00040000
ldr r1, =0x20200028
ldr r2, [r1]
orr r3, r0, r2
str r3, [r1]

bigLoop:

mov r4, #0
mov r0, #0x00040000
ldr r1, =0x2020001c
ldr r2, [r1]
orr r3, r0, r2
str r3, [r1]

insideLoop:
mov r5, #0
ldr r6, =0x4FFFFF

loop:
cmp r5, r6
bgt endLoop
add r5, r5, #1
sub r5, r5, #1
add r5, r5, #1
b loop

endLoop:
add r4, r4, #1
mov r0, #0x00040000
ldr r1, =0x20200028
ldr r2, [r1]
orr r3, r0, r2
str r3, [r1]
cmp r4, #2
blt insideLoop
b bigLoop

endBigLoop:
andeq r0,r0,r0
