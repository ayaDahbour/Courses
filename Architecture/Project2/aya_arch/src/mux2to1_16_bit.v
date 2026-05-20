
module mux2to1_16_bit (
  input wire [15:0] a,
  input wire [15:0] b,
  input wire sel,
  output wire [15:0] out
);

  assign out = (sel == 0) ? a : b;

endmodule

