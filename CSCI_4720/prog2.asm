    .data
listA:	.word 0 1 0 1 0 1 0 1 0 1
listB:	.word 1 0 1 0 1 0 1 0 1 0
listC:	.word 9 9 9 9 9 9 9 9 9 9

    .text 		
main:
    li $v0, 1 # use print
loop_init:
    li $t3, 0
    li $t4, 10
    
    # set list pointers
    la	$t0, listA
    la 	$t1, listB
    la	$t2, listC 
loop:
    bge $t3, $t4 end_loop # check if need to skip loop
    lw	$t5, 0($t0) # load value from list A
    lw	$t6, 0($t1) # load value from list B
    bge $t5, $t6, endif # check if need to skip if
# if:
    sw $0, 0($t2) # put t0 into t2
endif:
    addi $t0, $t0, 4 # move array iterator
    addi $t1, $t1, 4 # move array iterator
    addi $t2, $t2, 4 # move array iterator
    addi $t3, $t3, 1 # increase iterator
    j loop
end_loop:

# print_loop_init:
    la	$t2, listC 
    li $t3, 0
    li $t4, 10 
print_loop:
    lw $a0 0($t2)
    addi $t2, $t2, 4
    syscall
    addi $t3, $t3, 1
    blt $t3, $t4, print_loop
# exit:
    jr $ra