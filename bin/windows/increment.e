# Detailed program.
(program
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
		(+ number_to_increment 1)
	)
	
	# Print value.
	(print "-> " number_to_increment)
)

# One-liner :
# (program (print "->" (+ (to_numeric (input "Enter a number to increment : ")) 1)))
