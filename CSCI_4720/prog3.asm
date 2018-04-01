# David Luo
# 811357331
    .data
# indicies:   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18  19
data:   .word 9 8 7 6 5 4 3 2 1 0 -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
    .text
main:
    li $v0, 5       # read input
    syscall
    move $t0, $v0   # t0 = value to find
    # li $t0, 4
loop_init:          # t1 = value being compared to
    la $t2, data    # t2 = list pointer
    li $t3, 0       # t3 = index
    li $t4, 20      # t4 = max
loop:
    bge $t3, $t4, end_loop

    lw $t1, 0($t2)
    beq $t0, $t1, end_loop

    addi $t2, $t2, 4
    addi $t3, $t3, 1
    j loop
end_loop:
    bne $t0, $t1, not_equal
    j print
not_equal:
    li $t3, -1 # -1 = not found
print:
    li $v0, 1 # use print
    move $a0, $t3
    syscall
exit:
    jr $ra