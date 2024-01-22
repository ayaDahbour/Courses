module Mux2x1(input A, B, S,input enable,output reg Y);
  // mux code
  always @*
  begin
    if (enable)
      Y = (~S & A) | (S & B);  //logical modeling 
    else
      Y = 0; // when enable is not active
  end
endmodule  

/////////////////////////////////////	/////////////////////////////////////////

module Quad_Mux2x1(input [3:0] A,B,input S,input enable,output reg [3:0] Y); 
  wire Y0, Y1, Y2, Y3;
			// use 4 Mux2x1 to get the Quad
  Mux2x1 mux0(.A(A[0]), .B(B[0]), .S(S), .enable(enable) , .Y(Y0));
  Mux2x1 mux1(.A(A[1]), .B(B[1]), .S(S), .enable(enable) , .Y(Y1));
  Mux2x1 mux2(.A(A[2]), .B(B[2]), .S(S), .enable(enable) , .Y(Y2));
  Mux2x1 mux3(.A(A[3]), .B(B[3]), .S(S), .enable(enable) , .Y(Y3));

  assign Y = {Y3, Y2, Y1, Y0};
endmodule

/////////////////////////////////////	/////////////////////////////////////////

module BCD_7Segment_driver(input [3:0] BCD,output reg [6:0] Seg);
  always @*
  begin
    case (BCD)
      0: Seg = 7'b0111111; // 0
      1: Seg = 7'b0000110; // 1
      2: Seg = 7'b1011011; // 2
      3: Seg = 7'b1001111; // 3
      4: Seg = 7'b1100110; // 4
      5: Seg = 7'b1101101; // 5
      6: Seg = 7'b1111101; // 6
      7: Seg = 7'b0000111; // 7
      8: Seg = 7'b1111111; // 8
      9: Seg = 7'b1101111; // 9
      default: Seg = 7'b0000000; // all segments off for invalid BCD
    endcase
  end
endmodule

/////////////////////////////////////	/////////////////////////////////////////
 
module Decoder2x4(input A1,B1,input en,output reg [3:0] out);
  always @(*)
  begin
    if (en)
    begin
      case ({A1, B1}) // the decoder outputs are inverted
        2'b00: out = 4'b1110;
        2'b10: out = 4'b1101;
        2'b01: out = 4'b1011;
        2'b11: out = 4'b0111;
      endcase
    end
    else
      out = 4'b1111; // when the enable off
  end
endmodule

/////////////////////////////////////	/////////////////////////////////////////

module System (input S,input En,input [3:0] A,input [3:0] B,output reg [6:0] segment_Y,output reg [1:0] decoder_Y);

wire [3:0] BCD_input;
wire [6:0] segment_Y_wire;
 
  // Instantiate the modules
  Quad_Mux2x1 mux (.A(A),.B(B),.S(S),.enable(1'b1),.Y(BCD_input));

  BCD_7Segment_driver driver (.BCD(BCD_input),.Seg(segment_Y_wire));
  
  Decoder2x4 Decoder (.A1(S),.B1(1'b0),.en(1'b1),.out(decoder_Y)); 
  
 	// Combinational logic to determine segment_Y output based on decoder_Y
 always @*
  begin
    if (decoder_Y == 2'b01)
    begin
      segment_Y = segment_Y_wire;
  
    end
    else if (decoder_Y == 2'b10)
    begin
      segment_Y = segment_Y_wire;	 
    end
    else
    begin
      segment_Y = 7'b0000000; 
     
    end
  end 
endmodule	

/////////////////////////////////////	/////////////////////////////////////////

module System_tb;

  // Inputs
  reg [3:0] A;
  reg [3:0] B;
  reg S;
  reg E ; 

  // Outputs
  wire [6:0] Segment;
  wire [1:0] Decoder_Y;

  // Instantiate System module
  System uut (.A(A),.B(B),.S(S),.En(E),.segment_Y(Segment),.decoder_Y(Decoder_Y));

  
  initial begin
    // Test case 1
    A = 4'b0101;    //5
    B = 4'b0111;  	//7
	E = 1;  
    S = 1'b0;
    #10;

    // Test case 2
     A = 4'b0101;   
     B = 4'b0111;   
	 E = 1;         
     S = 1'b1;         // the same input with selection 1
     #10;           

    // Finish simulation
    $stop;
  end

endmodule