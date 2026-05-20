module registerA(		
	input wire regWriteA,
	input wire [15:0] regIn,
	output reg [15:0] regOut
	);
	always @* 
		begin			 
			if (regWriteA == 1) begin
				regOut <= regIn;

		end	
	end
endmodule
	