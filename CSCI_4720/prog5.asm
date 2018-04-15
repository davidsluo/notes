# David Luo
# 811357331
# Bubble Sort
	.data
list:	.word	7 0 2 9 5 6 1 8 4 3
	.text
main:
outer_init:
	la $t0, list # arr
	li $t2, 0 # i
	li $t3, 10 # n
	li $t4, 0 # temp
outer_for:
	bge $t2, $t3, end_outer
inner_init:
	li $t5, 1 # j
	sub $t6, $t3, $t2 # n - i
inner_for:
	bge $t5, $t6, end_inner
	
	addi $a0, $t0, 0
	addi $a1, $t5, 0
	jal get
	addi $t7, $a2, 0 # arr[j]
	
	addi $a0, $t0, 0
	subi $a1, $t5, 1
	jal get
	addi $t8, $a2, 0 # arr[j-1]
	
	ble $t8, $t7, skip_if
# if:
	addi $t4, $t8, 0
	
	addi $a0, $t0, 0
	subi $a1, $t5, 1
	addi $a2, $t7, 0
	jal set
	
	addi $a0, $t0, 0
	addi $a1, $t5, 0
	addi $a2, $t4, 0
	jal set
skip_if:
	addi $t5, $t5, 1	
	j inner_for
end_inner:
	addi $t2, $t2, 1
	j outer_for
end_outer:
print_init:
	la $t0, list
	li $t2, 0
	li $t3, 10
	li $v0, 1
print:
	bge $t2, $t3, end_print
	lw $a0, 0($t0)
	syscall
	addi $t0, $t0, 4
	addi $t2, $t2, 1
	j print
end_print:
exit:
	li $v0, 10
	syscall

# Function to get a value in an array
# x = list[i]
# $a0 = list
# $a1 = i
# $a2 = x
get:
	add $a1, $a1, $a1
	add $a1, $a1, $a1
	add $a0, $a0, $a1
	lw $a2, 0($a0)
	jr $ra

# Function to set a value in an array
# list[i] = x
# $a0 = list
# $a1 = i
# $a2 = x
set:
	add $a1, $a1, $a1
	add $a1, $a1, $a1
	add $a0, $a0, $a1
	sw $a2, 0($a0)
	jr $ra
