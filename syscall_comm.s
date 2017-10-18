.text
.align 2
.globl readStringState
.globl readFloatState
.globl readInt32State
.globl registerStateChange
.globl unregisterStateChange
.globl registerTimmedPolling
.globl unregisterTimmedPolling

readStringState:
	li		$v0, 16		# Comm call
	li		$v1, 1		# READ_STRING_CODE = 1
	# la 		$a0, a0
	# a0 already contains the pointer to the statePath
	syscall
	jr		$31			# jump to $31 (Return addr)

readFloatState:
	li		$v0, 16		# Comm call
	li		$v1, 6		# READ_FLOAT_CODE = 6
	# la 		$a0, a0
	# a0 already contains the pointer to the statePath
	syscall
	jr		$31			# jump to $31 (Return addr)

readInt32State:
	li		$v0, 16		# Comm call
	li		$v1, 7		# READ_INT32_CODE = 7
	# la 		$a0, a0
	# a0 already contains the pointer to the statePath
	syscall
	jr		$31			# jump to $31 (Return addr)

registerStateChange:
	li		$v0, 14		# Register_handler call
	move	$a2, $a1		# move the second argument (statePath) to the third one
	move	$a1, $a0		# move the first argument (procedure) to the second one
	li		$a0, 99		# communication event_id = 99 // defined in .h
	syscall
	jr		$31			# jump to $31 (Return addr)

unregisterStateChange:
	li		$v0, 15		# Syscall for unregister
	move	$a2, $a1		# move the second argument (statePath) to the third one
	move	$a1, $a0		# move the first argument (procedure) to the second one
	li		$a0, 99		# communication event_id = 99 // defined in .h
	syscall
	jr		$31		

registerTimmedPolling:
	li		$v0, 14		# Register Event Handler call
	li		$v1, 4		# REGISTER_EVENT_CODE = 4    /// TODO: Define it!
	# a0 already contains the pointer to the statePath
	# a0 already contains the interval in ms
	syscall
	jr		$31			# jump to $31 (Return addr)

unregisterTimmedPolling:
	li		$v0, 14		# Register Event Handler call
	li		$v1, 4		# REGISTER_EVENT_CODE = 4    /// TODO: Define it!
	# a0 already contains the pointer to the statePath
	# a0 already contains the interval in ms
	syscall
	jr		$31			# jump to $31 (Return addr)
