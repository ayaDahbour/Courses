
module mux2to1_3_bit (
  input wire [2:0] a,
  input wire [2:0] b,
  input wire sel,
  output wire [2:0] out
);

  assign out = (sel == 0) ? a : b;

endmodule

