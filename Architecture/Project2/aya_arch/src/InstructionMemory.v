module InstructionMemory(
    input wire [15:0] inputPC,
    output reg [15:0] Instruction,
	output reg [15:0] outputPC,

);

    reg [15:0] mem[6565536:0]; // Memory is 16 bits wide for MIPS

    // Example instructions:
    initial begin
		mem[0] = 16'b 0000000000011000;//AND R	 
		mem[1] = 16'b 0000000000011001;//ADD R
		mem[2] = 16'b 0000000000011000;//AND R	 
		mem[3] = 16'b 0110000011000010;//BEQ	   
		mem[4] = 16'b 0010000000000100;//***
		mem[5] = 16'b 0010000000111111;//ADD I
		mem[6] = 16'b 0001000001000001;//C of	
		mem[7] = 16'b 0000000000011000;
		mem[8] = 16'b 0000000000011000;
		mem[9] = 16'b 0001000000000010;


		

   
    end

    always @* begin
        Instruction = mem[inputPC]; // I'm assuming inputPC is incremented externally
		outputPC <= inputPC + 1;
    end

endmodule


