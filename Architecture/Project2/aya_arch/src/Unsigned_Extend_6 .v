module Unsigned_Extend_6 (
    input wire [5:0] in,  // 6-bit unsigned input
    output wire [15:0] out // 16-bit unsigned output
);
    assign out = {10'b0, in}; // Zero extension
endmodule
