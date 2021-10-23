.data

.text
main:		
I:
	li $v0,5
       	syscall       
        add $s0,$v0,$zero
	move $s7,$s0
II:
        li $v0,5
        syscall
        add $s1,$v0,$zero
        beq $s0,$s1,III
        add $s7,$s7,$s1
III:
        li $v0,5
        syscall
        add $s2,$v0,$zero
        beq $s0,$s2,IV
        beq $s1,$s2,IV
        add $s7,$s7,$s2
IV:
        li $v0,5
        syscall
        add $s3,$v0,$zero
        beq $s0,$s3,V
        beq $s1,$s3,V
        beq $s2,$s3,V
        add $s7,$s7,$s3
V:
        li $v0,5
        syscall
        add $s4,$v0,$zero
        beq $s0,$s4,VI
        beq $s1,$s4,VI
        beq $s2,$s4,VI
        beq $s3,$s4,VI
        add $s7,$s7,$s4
VI:
        li $v0,5
        syscall
        add $s5,$v0,$zero
        beq $s0,$s5,VII
        beq $s1,$s5,VII
        beq $s2,$s5,VII
        beq $s3,$s5,VII
        beq $s4,$s5,VII
        add $s7,$s7,$s5
VII:
        li $v0,5
        syscall
        add $s6,$v0,$zero
        beq $s0,$s6,END
        beq $s1,$s6,END
        beq $s2,$s6,END
        beq $s3,$s6,END
        beq $s4,$s6,END
        beq $s5,$s6,END
        add $s7,$s7,$s6
END:
        li $v0,1
        move $a0,$s7
        syscall
        
        li $v0,10
        syscall