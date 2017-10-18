.text
.align 2
.globl print_int32
.globl print_float
.globl print_double
.globl print_string

print_int32:
	li		$v0, 12		# $v0 = 12
	li		$v1, 22		# $v1 = 22
	la 		$t4, rxtxID
	syscall 
	jr		$31			# jump to $31 (return address)

print_float:
	li		$v0, 12		# $v0 = 12
	li		$v1, 23		# $v1 = 23
	la 		$t4, rxtxID
	syscall 
	jr		$31			# jump to $31 (return address)

print_double:
	li		$v0, 12		# $v0 = 12
	li		$v1, 24		# $v1 = 24
	la 		$t4, rxtxID
	syscall 
	jr		$31			# jump to $31 (return address)

print_string:
	li		$v0, 12		# $v0 = 12
	li		$v1, 25		# $v1 = 25
	la 		$t4, rxtxID
	syscall 
	jr		$31			# jump to $31 (return address)
