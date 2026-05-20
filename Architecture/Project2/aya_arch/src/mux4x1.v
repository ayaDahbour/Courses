module mux4x1 (
    input wire [15:0] a,       // Input 0
    input wire [15:0] b,       // Input 1
    input wire [15:0] c,       // Input 2
    input wire [15:0] d,       // Input 3
    input wire [1:0] sel,      // Select lines (2 bits)
    output wire [15:0] out     // Output
);

assign out = (sel == 2'b00) ? a :
             (sel == 2'b01) ? b :
             (sel == 2'b10) ? c :
             d;
endmodule