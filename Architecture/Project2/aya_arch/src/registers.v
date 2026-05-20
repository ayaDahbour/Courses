module registers(
	input wire clk,
	input wire regWr,
	input wire dec_reg_F,
	input wire [2:0] reg_to_decremented,
	input wire [2:0] readRegister1, 
	input wire [2:0] readRegister2, 
	input wire [2:0] writeRegister,
	input wire [15:0] writeData,
	output reg [15:0] readData1,
	output reg [15:0] readData2
	);
	
	reg [15:0] register [7:0];
	integer i; 
	
	initial
	begin
			for(i=0; i < 8; i = i + 1)
			begin
					register[i] = i;
			end
	end	   
	
	always @* begin
	assign readData1 = register[readRegister1];
	assign readData2 = register[readRegister2];		  
	end
	always @(posedge clk)
	begin
		if (dec_reg_F == 1 && register[reg_to_decremented]>0)
			register[reg_to_decremented] = register[reg_to_decremented]-1;	
		if (regWr == 1)
			begin
				register[writeRegister] = writeData;	
			end	
	end
			
endmodule
