module register_decrement (
    input wire clk,                  // Clock signal
    input wire enable,               // Enable signal for writing a new value
	input wire decrement,            // Independent signal for decrementing	
    input wire [15:0] assign_value,  // Value to be assigned
    output reg flag_out,             // Flag output: 1 if register reaches 0, 0 otherwise
    output reg [15:0] reg_value      // Register to store the value
);

	reg decrement_pulse;
	
	loop_pulse LP(clk, decrement, decrement_pulse);

    // Initialize register and flag
    initial begin
        flag_out = 0;
        reg_value = 0;
    end

    always @(posedge clk) begin
        if (enable && reg_value == 0) begin
            reg_value = assign_value + 1; // Assign value if enable is active and register is 0
        end 
		else if (decrement_pulse && reg_value > 0) begin
            reg_value = reg_value - 1; // Decrement only if register > 0
        end

        // Update flag_out
        flag_out <= (reg_value == 0) ? 1'b1 : 1'b0;
    end

endmodule		  


module loop_pulse (
    input wire clk,        // Clock signal
    input wire LOOP,       // Input signal
    output reg LOOP_one    // Output pulse signal
);

    // Register to track the previous state of LOOP
    reg prev_LOOP;

    // Initialize previous state and LOOP_one
    initial begin
        prev_LOOP = 0;
        LOOP_one = 0;
    end

    always @(posedge clk) begin
        if (LOOP && !prev_LOOP) begin
            // Generate a pulse when LOOP transitions from 0 to 1
            LOOP_one <= 1;
        end else begin
            // Clear the pulse on the next clock cycle
            LOOP_one <= 0;
        end

        // Update the previous state of LOOP
        prev_LOOP <= LOOP;
    end

endmodule

