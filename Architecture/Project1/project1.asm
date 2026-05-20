.data
base_path: .asciiz "C:\\Users\\EASY LIFE\\Desktop\\apps\\mars\\"
filename_prompt: .asciiz "Enter file name (or 'e' to exit): "
file_not_found: .asciiz "File not found. Try again.\n"
file_exists: .asciiz "File opened successfully!\n"
file_empty: .asciiz "The file is empty.\n"
valid_system_msg: .asciiz "Valid system of linear equations.\n"
invalid_system_msg: .asciiz "Invalid system of linear equations.\n"
variable_count_error_msg: .asciiz "Equation count does not match variable count.\n"
x_count_msg: .asciiz "Number of x's: "
y_count_msg: .asciiz "Number of y's: "
z_count_msg: .asciiz "Number of z's: "
eq_count_msg: .asciiz "Number of equations: "
newline: .asciiz "\n"
x_result_msg: .asciiz "Result x: "
y_result_msg: .asciiz "Result y: "
z_result_msg: .asciiz "Result z: "
buffer: .space 1024                 # Space for file content
full_filename: .space 64           # Space for the full path (base + filename)
filename: .space 32                # Space for the file name
prompt: .asciiz "Enter s or S to display on screen, f or F to save to file: "
filename_output: .asciiz "output.txt"
Num_eq: .space 1
.align 2
x_value: .space 4
y_value: .space 4
z_value: .space 4
constants_x: .space 32          # Array to store constants of x for each equation
constants_y: .space 32          # Array to store constants of y for each equation
constants_z: .space 32          # Array to store constants of z for each equation
constants_b: .space 32          # Array to store constants after '=' for each equation
float_buffer: .space 32

.text
main:
    # Main loop to keep asking for the file name if it doesn't exist
main_loop:
reset_full_filename:
    li $t0, 0                      # Initialize index to 0
    li $t1, 64                     # Set the size of full_filename to clear
reset_loop:
    sb $zero, full_filename($t0)   # Store 0 (null byte) at full_filename[$t0]
    addi $t0, $t0, 1               # Move to the next byte
    blt $t0, $t1, reset_loop       # Repeat until full_filename is cleared

    # Load the base path into full_filename
    la $t1, base_path              # Load base path address
    la $t2, full_filename          # Destination address for full filename
copy_base_path:
    lb $t3, 0($t1)                 # Load byte from base path
    beq $t3, $zero, read_filename  # End of string, go to filename read
    sb $t3, 0($t2)                 # Store byte in full_filename
    addi $t1, $t1, 1               # Move to next byte in base path
    addi $t2, $t2, 1               # Move to next byte in full_filename
    j copy_base_path               # Repeat until base path is copied

read_filename:
    # Prompt for file name
    li $v0, 4                      # System call for print_string
    la $a0, filename_prompt        # Load prompt message
    syscall                         # Print the prompt

    # Read the file name from the user
    li $v0, 8                      # System call for read_string
    la $a0, filename               # Load the space to store file name
    li $a1, 32                     # Max length of the filename
    syscall                         # Get the file name from user

    # Check if user entered 'e' or 'E' to exit
    la $t1, filename               # Load filename address
    lb $t3, 0($t1)                 # Load first character of filename
    li $t4, 101                    # ASCII for 'e'
    li $t5, 69                     # ASCII for 'E'
    beq $t3, $t4, exit_program     # If 'e', exit program
    beq $t3, $t5, exit_program     # If 'E', exit program

    # Append user-entered filename to the full path
    la $t1, filename               # Load address of filename input

append_filename:
    lb $t3, 0($t1)                 # Load byte from filename
    beq $t3, 10, open_file         # If newline, end of filename, go to open file
    
    sb $t3, 0($t2)                 # Append byte to full_filename
    addi $t1, $t1, 1               # Move to next byte in filename
    addi $t2, $t2, 1               # Move to next byte in full_filename
    j append_filename              # Repeat until full path is complete

open_file:
    # Open the file
    li $v0, 13                     # System call for open_file
    la $a0, full_filename          # Full filename address
    li $a1, 0                      # Mode: 0 for read-only
    li $a2, 0                      # Flags (not used here)
    syscall                         # Open file
    move $t0, $v0                  # Store file descriptor in $t0

    # Check if file was opened successfully
    bltz $t0, file_error           # If $v0 is -1, file open failed

    # File opened successfully message
    li $v0, 4                      # System call for print_string
    la $a0, file_exists            # Load success message
    syscall                         # Print success message

# Read and process each line in the file
read_loop:
    li $v0, 14                     # System call for read_file
    move $a0, $t0                  # File descriptor
    la $a1, buffer                 # Buffer to store file content
    li $a2, 1024                    # Number of bytes to read
    syscall                         # Read from file

    # Check if file is empty (returns 0 bytes read)
    beq $v0, $zero, file_empty_msg    # If $v0 == 0, end of file


    # Initialize or reset counters for a new equation system
    li $t4, 0                      # Equation count
    li $t5, 0                      # x presence count
    li $t6, 0                      # y presence count
    li $t7, 0                      # z presence count

check_x:
    lb $t9, buffer($t8)            # Load byte from buffer
    beq $t9, 13, increment_eq_count # If newline, increment equation count
    beq $t9, $zero, validate_system_before       # End of buffer, read next line
    beq $t9, $zero, end_of_file       # End of buffer, read next line

    li $t0, 'x'
    beq $t9, $t0, x_found         # If 'x' found, count it
    j check_y

x_found:
    addi $t5, $t5, 1               # Increment x count
    j increment_index

check_y:
    li $t0, 'y'
    beq $t9, $t0, y_found         # If 'y' found, count it
    j check_z

y_found:
    addi $t6, $t6, 1               # Increment y count
    j increment_index

check_z:
    li $t0, 'z'
    beq $t9, $t0, z_found         # If 'z' found, count it
    j increment_index

z_found:
    addi $t7, $t7, 1               # Increment z count

increment_index:
    addi $t8, $t8, 1               # Move to next character in buffer
    j check_x                      # Continue scanning line

increment_eq_count:
    addi $t4, $t4, 1               # Increment equations count
    addi $t8, $t8, 2               # Move to next character in buffer
    lb $t9, buffer($t8)            # Load byte from buffer
    beq $t9, 13, validate_and_reset_system # If newline, increment equation count
    j check_x

validate_and_reset_system:
    # Validate current system and print results
    addi $t8, $t8, 2               # Move to next character in buffer
    jal printXYZ                   # Print counts for current system
    jal validate_system            # Call validation function
    jal clear_all
    #mfc1 $t8, $f30
    lb $t9, buffer($t8)            # Load byte from buffer
    beq $t9, $zero, close_file
    # Reset counters for next system
    li $t4, 0                      # Reset equation count
    li $t5, 0                      # Reset x count
    li $t6, 0                      # Reset y count
    li $t7, 0                      # Reset z count
    j check_x                    # Continue reading next system
validate_system_before:
    addi $t8, $t8, 2               # Move to next character in buffer
    lb $t9, buffer($t8)            # Load byte from buffer
validate_system:
    # Validate system by comparing variable and equation counts
    beq $t4, $t5, check_two_var    
    j invalid_system               # If equation and variable counts don't match

check_two_var:
    beq $t6, $t4, check_three_var  
    j invalid_system               

check_three_var:
    beq $t7, $t4, valid_system
    beq $t4, 2,  store    
    j invalid_system
store:
    sb $t4, Num_eq
    li $v0, 1    
    move $a0, $t4                  
    syscall   
valid_system:
    li $v0, 4                      
    la $a0, valid_system_msg       
    syscall                         # Print valid system message
    j reading_variables                          # Return to caller

invalid_system:
    li $v0, 4                      
    la $a0, invalid_system_msg     
    syscall                         # Print invalid system message
    jr $ra                          # Return to caller

end_of_file:
    # Final system validation at end of file
    #jal validate_and_reset_system
    j close_file                    # Close file after processing all systems

file_empty_msg:
    li $v0, 4                      
    la $a0, file_empty             
    syscall                         
    j close_file                   

file_error:
    li $v0, 4                      
    la $a0, file_not_found         
    syscall                         
    j main_loop                    

close_file:
    li $v0, 16                     
    move $a0, $t0                  
    syscall                         
    j main_loop                    

exit_program:
    li $v0, 10                     
    syscall

printXYZ:

    # Print number of x's
    li $v0, 4                      
    la $a0, x_count_msg            
    syscall                         
    li $v0, 1                      
    move $a0, $t5                  # $t5 holds x count
    syscall                         
    li $v0, 4                      
    la $a0, newline                
    syscall                         

    # Print number of y's
    li $v0, 4                      
    la $a0, y_count_msg            
    syscall                         
    li $v0, 1                      
    move $a0, $t6                  # $t6 holds y count
    syscall                         
    li $v0, 4                      
    la $a0, newline                
    syscall                         

    # Print number of z's
    li $v0, 4                      
    la $a0, z_count_msg            
    syscall                         
    li $v0, 1                      
    move $a0, $t7                  # $t7 holds z count
    syscall                         
    li $v0, 4                      
    la $a0, newline                
    syscall                         

    # Print number of equations
    li $v0, 4                      
    la $a0, eq_count_msg           
    syscall                         
    li $v0, 1                      
    move $a0, $t4                  # $t4 holds equation count
    syscall                         
    li $v0, 4                      
    la $a0, newline                
    syscall

    jr $ra
reading_variables:
    move $s7, $t8
    move $s5, $t4
    move $s4, $t5
    move $s3, $t6
    move $s2, $t7
    mfc1 $t8, $f30                   # Index for the buffer
    li $s0, 0                   # Index for equations (array position)
    li $t9, 0                   # Temp variable for constants
    li $a3, 1
read_next_char:
    lb $t0, buffer($t8)         # Load the current character from the buffer
    beq $t8, $s7, end_of_buffer   # If end of buffer, finish reading
    # Check if character is a digit or a sign
    li $t1, '0'                 # ASCII code for '0'
    li $t2, '9'                 # ASCII code for '9'
    li $t3, '-'                 # ASCII code for '-'
    li $t4, '+'                 # ASCII code for '+'
    beq $t0, 32, next_char
    blt $t0, $t1, not_digit     # If below '0', not a digit
    bgt $t0, $t2, not_digit     # If above '9', not a digit

    # Accumulate digits for a constant
    sub $t0, $t0, $t1           # Convert ASCII to integer (0-9)
    mul $t9, $t9, 10            # Shift previous digits to the left
    add $t9, $t9, $t0           # Add new digit to the constant
    j next_char                 # Move to the next character

not_digit:
    # Handle negative or positive sign at the start of a number
    beq $t0, $t3, negative_sign
    beq $t0, $t4, positive_sign
    j check_variable             # If not digit or sign, check for variables

negative_sign:
    li $a3, -1                   # Set a negative sign
    j next_char

positive_sign:
    li $a3, 1                    # Set a positive sign
    j next_char

check_variable:
    li $t5, 'x'
    beq $t0, $t5, store_x        # If 'x', store in x constant array

    li $t5, 'y'
    beq $t0, $t5, store_y        # If 'y', store in y constant array

    li $t5, 'z'
    beq $t0, $t5, store_z        # If 'z', store in z constant array

    li $t5, '='
    beq $t0, $t5, read_result    # If '=', move to read the result constant

    j next_char                  # Continue to next character if none match

store_x:
    beq $t9, $zero, t9isZero
backX:
    mul $t9, $a3, $t9           # sign
     
      
    sw $t9, constants_x($s0)     # Store constant in x array
    j reset_constant             # Reset constant after storing

store_y:
    beq $t9, $zero, t9isZero
backY:
    mul $t9, $a3, $t9           # sign
    sw $t9, constants_y($s0)     # Store constant in y array
    j reset_constant

store_z:
    beq $t9, $zero, t9isZero
backZ:
    mul $t9, $a3, $t9           # sign
    sw $t9, constants_z($s0)     # Store constant in z array
    j reset_constant

read_result:
    addi $t8, $t8, 1             # Move to the next character in the buffer
    j accumulate_result          # Proceed to read the result after '='
increaseResult:
    addi $t8, $t8, 1             # Move to the next character in the buffer

accumulate_result:
    # Accumulate digits for the result constant
    lb $t0, buffer($t8)          # Read character
    
    beq $t0, 13, result_done    # If below '0', end of result number
    beq $t0, 32, increaseResult    # If above '9', end of result number
    sub $t0, $t0, $t1            # Convert ASCII to integer (0-9)
    mul $t9, $t9, 10             # Shift left
    add $t9, $t9, $t0            # Add digit
    addi $t8, $t8, 1             # Move to next character
    j accumulate_result

result_done:
    mul $t9, $a3, $t9           # sign
    sw $t9, constants_b($s0)     # Store result constant
    addi $s0, $s0, 4             # Move to next equation position in arrays
    li $t9, 0
    j reset_constant

reset_constant:
    li $t9, 0                    # Reset constant accumulator
    li $a3, 1
    j next_char

next_char:
    addi $t8, $t8, 1             # Move to the next character in the buffer
    j read_next_char

end_of_buffer:   
    move $t8, $s6
    move $t4, $s5
    move $t5, $s4
    move $t6, $s3
    move $t7, $s2
    j solve                       # Return from function
t9isZero:
    li $t9, 1
    beq $t5, 'x', backX
    beq $t5, 'y', backY
    beq $t5, 'z', backZ
solve:
    # Load constants from arrays into available registers for D calculation
    lb $a0, Num_eq
    beq $a0, 2, solve2  
    li $v0, 1                      
    syscall                  
    lw $t0, constants_x           # x1
    lw $t1, constants_y           # y1
    lw $t2, constants_z           # z1
    
    lw $t3, constants_x + 4       # x2
    lw $t4, constants_y + 4       # y2
    lw $t5, constants_z + 4       # z2
    
    lw $t6, constants_x + 8       # x3
    lw $t7, constants_y + 8       # y3
    lw $t8, constants_z + 8       # z3
    
    mul $t9, $t4, $t8             # y2 * z3
    mul $a0, $t5, $t7             # z2 * y3
    sub $a1, $t9, $a0             # (y2 * z3 - z2 * y3)
    mul $t9, $t0, $a1             # x1 * (y2 * z3 - z2 * y3)

    #t9 19
    # Calculate D (determinant of the original matrix)
    mul $a0, $t3, $t8             # x2 * z3
    mul $a1, $t6, $t5             # z2 * x3
    sub $a2, $a0, $a1             # (y2 * z3 - z2 * y3)
    mul $a0, $t1, $a2             # x1 * (y2 * z3 - z2 * y3)
#12
    
    mul $a1, $t3, $t7            # x2 * y3
    mul $a2, $t4, $t6             # y2 * x3
    sub $a3, $a1, $a2             # (x2 * z3 - z2 * x3)
    mul $a1, $t2, $a3             # y1 * (x2 * z3 - z2 * x3)
#14
    sub $t9, $t9, $a0             # Subtract for D
    add $t9, $t9, $a1
    move $s0, $t9
#D = 21 = 19 -12 + 14 $s0


#Dx 
    lw $t0, constants_b           # x1
    lw $t3, constants_b + 4       # x2
    lw $t6, constants_b + 8       # x3
    
    mul $t9, $t4, $t8 
    mul $a0, $t5, $t7 
    sub $a1, $t9, $a0 
    mul $t9, $t0, $a1  

    mul $a0, $t3, $t8
    mul $a1, $t6, $t5
    sub $a2, $a0, $a1
    mul $a0, $t1, $a2
    
    mul $a1, $t3, $t7 
    mul $a2, $t4, $t6
    sub $a3, $a1, $a2
    mul $a1, $t2, $a3
    
    sub $t9, $t9, $a0 
    add $t9, $t9, $a1
    move $s1, $t9
#dx= 21 $s1

    lw $t0, constants_x           # x1
    lw $t3, constants_x + 4       # x2
    lw $t6, constants_x + 8       # x3
#DY
    lw $t1, constants_b           # y1
    lw $t4, constants_b + 4       # y2
    lw $t7, constants_b + 8       # y3
    
    mul $t9, $t4, $t8 
    mul $a0, $t5, $t7 
    sub $a1, $t9, $a0 
    mul $t9, $t0, $a1  

    mul $a0, $t3, $t8
    mul $a1, $t6, $t5
    sub $a2, $a0, $a1
    mul $a0, $t1, $a2
    
    mul $a1, $t3, $t7 
    mul $a2, $t4, $t6
    sub $a3, $a1, $a2
    mul $a1, $t2, $a3
    
    sub $t9, $t9, $a0 
    add $t9, $t9, $a1
    move $s2, $t9
#dy= 42 $s2  
    lw $t1, constants_y           # y1
    lw $t4, constants_y + 4       # y2
    lw $t7, constants_y + 8       # y3
#DZ
    lw $t2, constants_b           # z1
    lw $t5, constants_b + 4       # z2
    lw $t8, constants_b + 8       # z3
    
    mul $t9, $t4, $t8 
    mul $a0, $t5, $t7 
    sub $a1, $t9, $a0 
    mul $t9, $t0, $a1  

    mul $a0, $t3, $t8
    mul $a1, $t6, $t5
    sub $a2, $a0, $a1
    mul $a0, $t1, $a2
    
    mul $a1, $t3, $t7 
    mul $a2, $t4, $t6
    sub $a3, $a1, $a2
    mul $a1, $t2, $a3
    
    sub $t9, $t9, $a0 
    add $t9, $t9, $a1
    move $s3, $t9
#dz = 63 
    lw $t2, constants_z           # z1
    lw $t5, constants_z + 4       # z2
    lw $t8, constants_z + 8       # z3
#########################################

    # Calculate x, y, z using Cramer's Rule (x = Dx / D, y = Dy / D, z = Dz / D)
    #div $s4, $s1, $s0             # x = Dx / D
    #div $s5, $s2, $s0             # y = Dy / D
    #div $s6, $s3, $s0             # z = Dz / D
    mtc1 $s0, $f12
    mtc1 $s1, $f16
    mtc1 $s2, $f17
    mtc1 $s3, $f18
    cvt.s.w $f12, $f12
    cvt.s.w $f16, $f16
    cvt.s.w $f17, $f17
    cvt.s.w $f18, $f18
    nop
    div.s $f19, $f16, $f12       # x = Dx / D
    div.s $f20, $f17, $f12       # y = Dy / D
    div.s $f21, $f18, $f12       # z = Dz / D
    swc1 $f19, x_value
    swc1 $f20, y_value
    swc1 $f21, z_value
    mtc1 $zero, $f12
    mtc1 $zero, $f16
    mtc1 $zero, $f17
    mtc1 $zero, $f18
    #div $s4, $s1, $s0             # x = Dx / D
    #div $s5, $s2, $s0             # y = Dy / D
    #div $s6, $s3, $s0             # z = Dz / D
Print:
    
    # Prompt the user
    li $v0, 4                      # syscall for print string
    la $a0, prompt                 # load prompt string
    syscall

    # Read user's choice
    li $v0, 12                      # syscall for read integer
    syscall
    move $t0, $v0                  # store user choice in $t0
    li $v0, 4                      
    la $a0, newline                
    syscall
    # Check user choice
    li $t1, 83
    
    beq $t0, $t1, display_result   # if choice is 1, display on screen
    li $t1, 115
    
    beq $t0, $t1, display_result   # if choice is 1, display on screen
    li $t1, 70
    beq $t0, $t1, write_float_to_file     # if choice is 2, write to file
    li $t1, 102
    beq $t0, $t1, write_float_to_file     # if choice is 2, write to file
    
    
    # Output results for x, y, and z
    # Print x result
display_result:
    li $v0, 4
    la $a0, x_result_msg
    syscall                    # Print "Result x: "
    
    li $v0, 2                  # Syscall for printing float
    mov.s $f12, $f19           # Move x result to $f12 for printing
    syscall                    # Print float x
    li $v0, 4                      
    la $a0, newline                
    syscall      
    # Print y result
    li $v0, 4
    la $a0, y_result_msg
    syscall                    # Print "Result y: "
    
    li $v0, 2
    mov.s $f12, $f20           # Move y result to $f12 for printing
    syscall                    # Print float y
    li $v0, 4                      
    la $a0, newline                
    syscall      
    # Print z result
    li $v0, 4
    la $a0, z_result_msg
    syscall                    # Print "Result z: "
    
    li $v0, 2
    mov.s $f12, $f21           # Move z result to $f12 for printing
    syscall                    # Print float z
    li $v0, 4                      
    la $a0, newline                
    syscall      
    
    
    
    move $t8, $s7 
    mtc1 $t8, $f30
    li $s7, 0   
    mtc1 $s7, $f0           # Load 0.0 into floating-point register $f0
    swc1 $f0, x_value         # Store 0.0 into x_value
    swc1 $f0, y_value         # Store 0.0 into y_value
    swc1 $f0, z_value         # Store 0.0 into z_value
    jr $ra
# Arguments:
# $f19, $f20, $f21: Floating-point values to be written.

write_float_to_file:
    # Open the file for writing
    li $v0, 13                     # Syscall for open file
    la $a0, filename_output        # Load output filename
    li $a1, 1                      # Open in write mode
    li $a2, 0                      # No permissions required
    syscall
    move $t1, $v0                  # Store file descriptor in $t1

    # Write $f19 to the file
    mov.s $f12, $f19               # Move $f19 to $f12
    li $v0, 2                      # Syscall to print float (debugging, optional)
    syscall

    # Write float to file
    li $v0, 15                     # Syscall for write to file
    move $a0, $t1                  # File descriptor
    la $a1, float_buffer           # Use buffer to hold float
    syscall                        # Write buffer to file

    # Write newline after $f19
    li $v0, 15
    move $a0, $t1
    la $a1, newline                # Newline character
    li $a2, 1
    syscall

    # Repeat for $f20
    mov.s $f12, $f20               # Move $f20 to $f12
    li $v0, 2                      # Print float for debugging (optional)
    syscall

    li $v0, 15
    move $a0, $t1
    la $a1, float_buffer
    syscall

    li $v0, 15
    move $a0, $t1
    la $a1, newline
    li $a2, 1
    syscall

    # Repeat for $f21
    mov.s $f12, $f21               # Move $f21 to $f12
    li $v0, 2                      # Print float for debugging (optional)
    syscall

    li $v0, 15
    move $a0, $t1
    la $a1, float_buffer
    syscall

    li $v0, 15
    move $a0, $t1
    la $a1, newline
    li $a2, 1
    syscall

    # Close the file
    li $v0, 16                     # Syscall for close file
    move $a0, $t1                  # File descriptor
    syscall
    move $t8, $s7 
    mtc1 $t8, $f30
    li $s7, 0   
    mtc1 $s7, $f0           # Load 0.0 into floating-point register $f0
    swc1 $f0, x_value         # Store 0.0 into x_value
    swc1 $f0, y_value         # Store 0.0 into y_value
    swc1 $f0, z_value         # Store 0.0 into z_value
    jr $ra                         # Return to caller


clear_all:
    # Clear x_value, y_value, z_value
clear_x_value:
    sw $zero, x_value   # Set each byte in x_value to 0
clear_y_value:
    sw $zero, y_value
clear_z_value:
    sw $zero, z_value
    # Clear constants_x
    li $t0, 0                # Index
    li $t1, 32               # Size of the arrays
clear_constants_x:
    sw $zero, constants_x($t0)
    addi $t0, $t0, 4
    blt $t0, $t1, clear_constants_x

    # Clear constants_y
    li $t0, 0
clear_constants_y:
    sw $zero, constants_y($t0)
    addi $t0, $t0, 4
    blt $t0, $t1, clear_constants_y
    sw $zero, constants_y($t0)

    # Clear constants_z
    li $t0, 0
clear_constants_z:
    sw $zero, constants_z($t0)
    addi $t0, $t0, 4
    blt $t0, $t1, clear_constants_z

    # Clear constants_b
    li $t0, 0
clear_constants_b:
    sw $zero, constants_b($t0)
    addi $t0, $t0, 4
    blt $t0, $t1, clear_constants_b

    jr $ra                    # Return to the caller
solve2:
    sb $zero, Num_eq
    # Load constants from arrays into available registers for D calculation

    lw $t0, constants_x           # x1
    lw $t1, constants_y           # y1
    
    lw $t3, constants_x + 4       # x2
    lw $t4, constants_y + 4       # y2
    

 # d = a1b2 - a2b1   
    mul $t9, $t0, $t4             # a1 * b2
    mul $t8, $t3, $t1             # a2 * b1
    sub $t7, $t9, $t8             # a1 * b2 - a2 * b1
    move $s0, $t7
    
    li $v0, 1                      
    move $a0, $s0                  # $t6 holds y count
    syscall   
#Dx 
    lw $t2, constants_b           # x1
    lw $t5, constants_b + 4       # x2
    
    mul $t9, $t2, $t4             # c1 * b2
    mul $t8, $t5, $t1             # c2 * b1
    sub $t7, $t9, $t8             # a1 * b2 - a2 * b1
    move $s1, $t7
    
        
    li $v0, 1                      
    move $a0, $s1                  # $t6 holds y count
    syscall   
    
#dx= 21 $s1
    mul $t9, $t0, $t5             # a1 * c2
    mul $t8, $t2, $t3             # a2 * c1
    sub $t7, $t9, $t8             # a1 * b2 - a2 * b1
    move $s2, $t7

        
    li $v0, 1                      
    move $a0, $s2                  # $t6 holds y count
    syscall   
#########################################

    # Calculate x, y, z using Cramer's Rule (x = Dx / D, y = Dy / D, z = Dz / D)
    #div $s4, $s1, $s0             # x = Dx / D
    #div $s5, $s2, $s0             # y = Dy / D
    #div $s6, $s3, $s0             # z = Dz / D
    mtc1 $s0, $f12
    mtc1 $zero, $f21
    mtc1 $s1, $f16
    mtc1 $s2, $f17
    cvt.s.w $f12, $f12
    cvt.s.w $f16, $f16
    cvt.s.w $f17, $f17
    nop
    div.s $f19, $f16, $f12       # x = Dx / D
    div.s $f20, $f17, $f12       # y = Dy / D
    swc1 $f19, x_value
    swc1 $f20, y_value
    swc1 $f0, z_value
    mtc1 $zero, $f12
    mtc1 $zero, $f16
    mtc1 $zero, $f17
    #div $s4, $s1, $s0             # x = Dx / D
    #div $s5, $s2, $s0             # y = Dy / D
    #div $s6, $s3, $s0             # z = Dz / D
    j Print
