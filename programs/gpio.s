mov r0, #0x00040000 ;store the instruction to put pin 16 in output mode and all others in input mode
mov r1, #0x20200004 ;store the memory adress for pin 16
str r0, [r1] ;put the instruction in correct memory location


;begin loop to repeat operation a couple of time
mov r2, 0 ;instruction that doesn't change anything
mov r6, #0 ;counter
mov r7, #4 ; repeat it four times
bigLoop:
cmp r6, r7
bge endBigLoop


;turn pin 16 on
mov r0, #0x00010000; ;put the instruction to clear pin 16
mov r1, #0x20200028 ;store the memory adress for clearing
str r0, [r1] ;clear the pin 16
mov r1, #0x2020001c ;store the memory adress for setting
str r0, [r1] ;set pin 16
;light is set


;begin loop to hold for a long time
mov r5, #0 ;let counter be equal to 0
mov r6, #1000 ;repeat it 1000 times
loop:
cmp r5, r6
jg endloop
str r5, r1 ;put all zeros instruction in clearing memory location
add r5, #1 ;increment the counter
endloop:

;turn pin 16 off
mov r1, #0x20200028 ;store the memory adress for clearing
str r0, [r1] ;clear the pin 16


;begin loop to hold for a long time
mov r5, #0 ;let counter be equal to 0
mov r6, #1000 ;repeat it 1000 times
loop:
cmp r5, r6
jg endloop
str r5, r1 ;put all zeros instruction in clearing memory location
add r5, #1 ;increment the counter
endloop:



endBigLoop:
