# Detailed program.
(program
	# Define a function.
	(function
		increment_this
		x
		(+ x 1)
	)

	# Get input.
	(assign
		number_to_increment
		(input "Enter a number to increment : ")
	)

	# Convert to numeric.
	(assign
		number_to_increment
		(to_numeric number_to_increment)
	)
	
	# Increment.
	(assign
		number_to_increment
		(increment_this number_to_increment)
	)
	
	# Print value.
	(print "-> " number_to_increment)
)

# One-liner :
# (program (print "->" (+ (to_numeric (input "Enter a number to increment : ")) 1)))
