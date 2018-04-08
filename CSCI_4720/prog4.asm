    .data
listA:	.float 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.10
listB:	.float 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.10
listC:	.space 320
    .text 		
main:
loop_subtact_init:
    li $t3, 0
    li $t4, 10

    li $t5, 0
    li $t6, 0
    li $t7, 0
    # reset list pointers
    la	$t0, listA
    la 	$t1, listB
    la	$t2, listC 
subtract_loop:
    lwc1 $f4, 0($t0) # load value from list A
    lwc1 $f5, 0($t1) # load value from list B
    # lw    $t7, 0($t2) # load value from list C
    add.s $f6 $f4 $f5 # t7 = t5 - t6
    swc1 $f6, 0($t2) # put t7 into t2
    addi $t0, $t0, 4 # move array iterator
    addi $t1, $t1, 4 # move array iterator
    addi $t2, $t2, 4 # move array iterator
    addi $t3, $t3, 1 # increase iterator
    blt $t3, $t4, subtract_loop
loop_C_init:
    la	$t2, listC 
    li $t3, 0
    li $t4, 10 
print_C:
    lwc1 $f12, 0($t2)
    addi $t2, $t2, 4
    # print
    li $v0, 2 # use floating point print
    syscall
    # print newline for readability
    li $a0, 0xA # ascii for linefeed
    li $v0, 11 # use character print
    syscall

    addi $t3, $t3, 1
    blt $t3, $t4, print_C
exit:
    jr $ra