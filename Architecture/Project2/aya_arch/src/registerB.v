module registerB(																				   
	input wire regWriteB,
	input wire [15:0] regIn,
	output reg [15:0] regOut
	);
	reg register;
	always @*
		begin		
			if(regWriteB == 1) begin
			regOut = regIn;
			end	   
	end
endmodule
	