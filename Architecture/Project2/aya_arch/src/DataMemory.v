module DataMemory(
  input wire clk,
  input wire [15:0] address,
  input  reg [15:0] dataIn,
  input wire MemRead,
  input wire MemWrite,
  output  reg [15:0] dataOut
);

  reg [15:0] memory[0:65536];  


  always @(posedge clk)
  begin
    if (MemWrite)//mode =1 >>write
    begin
      memory[address]= dataIn;
    end 
    if (MemRead) 
    begin
          dataOut = memory[address];
    end
    end
endmodule																				
