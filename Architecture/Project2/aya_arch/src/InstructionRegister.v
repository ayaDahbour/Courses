module InstructionRegister(
	input wire clk,	  
	input wire IRWrite,
	input wire [15:0] regIn,
	output reg [2:0] Func,	 
	output reg [3:0] Rsr,  
	output reg [3:0] Rtr, 
	output reg [3:0] Rdr,   	
	output reg [3:0] Rsi,  
	output reg [3:0] Rti, 
	output reg [8:0] offset,
	output reg [5:0] imm,
	output reg [3:0] Opcode,  
	);
	always @*
		begin  
			if (IRWrite == 1)
				begin  
					Opcode <= regIn[15:12];
					Rdr <= regIn[11:9];	
					Rsr <= regIn[8:6];
					Rtr <= regIn[5:3];
					Func <= regIn[2:0];
					Rsi <= regIn[11:9];
					Rti <= regIn[8:6];
					imm <= regIn[5:0];
					offset <= regIn[11:3];
				end
	end
endmodule
	