    .data
listA:	.word 1 2 3 4 5 6 7 8 9 10
listB:	.word 0 0 0 0 0 0 0 0 0 0
listC:	.space	40

    .text 		
main:
    # la	$t0, listA
    # la 	$t1, listB
    # la	$t2, listC
    li $v0, 1 # use print
# loop_A_init:
# 	# index
# 	li $t3, 0
# 	# max index
# 	li $t4, 10 
# print_A:
# 	lw $a0 0($t0)
# 	addi $t0, $t0, 4
# 	syscall
# 	addi $t3, $t3, 1
# 	blt $t3, $t4, print_A
# loop_B_init:
# 	li $t3, 0
# 	li $t4, 10 
# print_B:
# 	lw $a0 0($t1)
# 	addi $t1, $t1, 4
# 	syscall
# 	addi $t3, $t3, 1
# 	blt $t3, $t4, print_B
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
    lw	$t5, 0($t0) # load value from list A
    lw	$t6, 0($t1) # load value from list B
    # lw    $t7, 0($t2) # load value from list C
    sub $t7 $t5 $t6 # t7 = t5 - t6
    sw $t7, 0($t2) # put t7 into t2
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
    lw $a0 0($t2)
    addi $t2, $t2, 4
    syscall
    addi $t3, $t3, 1
    blt $t3, $t4, print_C
exit:
    jr $ra