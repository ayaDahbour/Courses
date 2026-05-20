module branchAdder(	
	input clk,
    input [15:0] PC,
    input [15:0] imm,
    output reg [15:0] target
);      

    always @(posedge clk)
        begin
            target = PC + imm; // Non-blocking assignment used in procedural blocks
        end
endmodule
