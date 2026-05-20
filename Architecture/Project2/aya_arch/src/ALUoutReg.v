module ALUoutReg(
input wire clk,
	input wire [15:0] regIn,
	output reg [15:0] regOut
	);

	always @(posedge clk)
		begin
			regOut <= regIn;
	end
endmodule
	