module EQ_check (
    input wire [15:0] num1,   // First 16-bit input number
    input wire [15:0] num2,   // Second 16-bit input number
    output wire is_equal      // Output signal: 1 if equal, 0 otherwise
);

    // Compare the two inputs for equality
    assign is_equal = (num1 == num2) ? 1'b1 : 1'b0;

endmodule
