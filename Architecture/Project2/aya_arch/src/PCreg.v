module PCreg(
	input wire clk,		   
	input wire [15:0] regIn,
	output reg [15:0] regOut,
	input wire enable
	); 			 
	initial begin
		regOut <= 0;
	end

	always @(posedge clk)
		begin						
			if (enable == 1) 	 
				begin
					regOut = regIn;
			end
	end
endmodule
	