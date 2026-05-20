module Sign_Extend_6 (
    input wire [5:0] in,  // 6-bit signed input
    output wire [15:0] out // 16-bit signed output
);
    assign out = {{10{in[5]}}, in}; // Sign extension
endmodule
