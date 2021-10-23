.data
.text
main:
	li $v0,5
	syscall
	add $s0,$v0,$zero
	li $v0,5
	syscall
	add $s1,$v0,$zero
	jal COMPARE

	li $v0,1
	move $a0,$s1
	syscall
	li $v0,10
	syscall
COMPARE:
	addi $sp,$sp,-4
	sw $ra,0($sp)
	slt $t0,$s0,$s1
	bne $t0,$zero,GCD
	j EXC
REC:
	addi $sp,$sp,-4
	sw $ra,0($sp)
EXC:
	move $t0,$s0
	move $s0,$s1
	move $s1,$t0
GCD:
	beq $s0,$zero,BACK
	rem $s1,$s1,$s0
	jal REC
BACK:
	lw $ra,0($sp)
	addi $sp,$sp,4
	jr $ra