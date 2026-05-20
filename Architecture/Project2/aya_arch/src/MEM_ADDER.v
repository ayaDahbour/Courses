module MEM_ADDER(
	
	input [15:0] Rs_value,extended_imm,
	output [15:0] address
	
);
	
	
	assign address = Rs_value +	extended_imm;
	
endmodule
							 