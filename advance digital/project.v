//Advance digital project - Aya Dahbour 1201738

`timescale 1ns / 1ps

module alu(
    input [5:0] opcode,
    input signed [31:0] a, b,
    output reg signed [31:0] result
);

    always @(*) begin
        case (opcode)
            6'b000001: result = a + b; // a + b //1
            6'b000110: result = a - b; // a - b //6
            6'b001101: result = a[31] ? -a : a; // |a| //13
            6'b001000: result = -a; // -a //8
            6'b000111: result = (a > b) ? a : b; // max(a, b) //7
            6'b000100: result = (a < b) ? a : b; // min(a, b) //4
            6'b001011: result = (a + b) >> 1; // avg(a, b) //11
            6'b001111: result = ~a; // not a //15
            6'b000011: result = a | b; // a or b //3
            6'b000101: result = a & b; // a and b //5
            6'b000010: result = a ^ b; // a xor b //2
            default: result = 32'd0;
        endcase
    end
endmodule	

//ALU test bench
module alu_tb;

reg [31:0] a, b;
reg [5:0] opcode;
wire [31:0] result;

//Sample of the ALU module
alu uut (
    .a(a), 
    .b(b), 
    .opcode(opcode), 
    .result(result)
);

initial begin
    //Initialize Inputs
    a = 0;
    b = 0;
    opcode = 0;

    //Test case for a + b
    a = 32'd15;
    b = 32'd10;
    opcode = 6'b000001;
    #10; 
	
    if (result == (a + b))
        $display("Addition Test Passed! Result: %d", result);
    else
        $display("Addition Test Failed! Expected: %d, Got: %d", a+b, result);

    //Test case for a - b
    a = 32'd20;
    b = 32'd10;
    opcode = 6'b000110;
    #10;
    if (result == (a - b))
        $display("Subtraction Test Passed! Result: %d", result);
    else
        $display("Subtraction Test Failed! Expected: %d, Got: %d", a-b, result);

    #100; // wait 100 ns for global reset to finish
    $finish;
end

endmodule

////////////////////////////////////////////////////////////////////////////////////////////////

module reg_file(
    input clk,
    input valid_opcode,
    input [4:0] addr1, addr2, addr3,
    input [31:0] in,
    output [31:0] out1, out2
    );

    reg [31:0] registers [31:0];

    // Initialize the register file with the values
    initial begin
        registers[0] = 30;
        registers[1] = 32'd12996;
        registers[2] = 32'd11490;
        registers[3] = 32'd7070;
        registers[4] = 32'd6026;
        registers[5] = 32'd3322;
        registers[6] = 32'd10344;
        registers[7] = 32'd6734;
        registers[8] = 32'd15834;
        registers[9] = 32'd15314;
        registers[10] = 32'd6000;
        registers[11] = 32'd12196;
        registers[12] = 32'd11290;
        registers[13] = 32'd13350;
        registers[14] = 32'd2086;
        registers[15] = 32'd6734;
        registers[16] = 32'd7430;
        registers[17] = 32'd14102;
        registers[18] = 32'd13200;
        registers[19] = 32'd3264;
        registers[20] = 32'd2368;
        registers[21] = 32'd15846;
        registers[22] = 32'd11710;
        registers[23] = 32'd14736;
        registers[24] = 32'd5338;
        registers[25] = 32'd5544;
        registers[26] = 32'd1852;
        registers[27] = 32'd3898;
        registers[28] = 32'd16252;
        registers[29] = 32'd1048;
        registers[30] = 32'd5642;
        registers[31] = 32'd0;
    end

    //Read operations
    assign out1 = registers[addr1];
    assign out2 = registers[addr2];

    //Write operation
    always @(posedge clk) begin
        if (valid_opcode) begin
            registers[addr3] <= in;
        end
    end
endmodule

//register file test bench
module reg_file_tb;

	reg clk;
	reg valid_opcode;
	reg [4:0] addr1, addr2, addr3;
	reg [31:0] in;
	wire [31:0] out1, out2;
	
	//Sample of the register file module
	reg_file uut (
	    .clk(clk),
	    .valid_opcode(valid_opcode),
	    .addr1(addr1),
	    .addr2(addr2),
	    .addr3(addr3),
	    .in(in),
	    .out1(out1),
	    .out2(out2)
	);
	
	
	always #5 clk = ~clk;
	
	initial begin
	    //Initialize signals
	    clk = 0;
	    valid_opcode = 0;
	    addr1 = 0;
	    addr2 = 0;
	    addr3 = 0;
	    in = 0;
	
	    //Read initial value from a register
	    addr1 = 5'd1; //Address of register 1
	    #10;
	    $display("At %0t, it's a reading from Address 1. 12996 expected, Got:%d", $time, out1);
	    
	    // Write operation
	    addr3 = 5'd2; // Address of register 2
	    in = 32'd12345; // New value to write into register 2
		
	    valid_opcode = 1; //Enable writing
	    #10; 
	    valid_opcode = 0; //Disable writing
	    #10;
	    $display("Performed write to Address 2 at %0t", $time);
	    
	    addr2 = 5'd2; //Set Address2 to read from the register that just wrote to
	    #10; 
	    $display("Reading from Address 2 at %0t after write. Expected: 12345, Got: %d", $time, out2);
		
		
		addr1 = 5'd15; //Set Address1 to read from register 15
		#10; 
		$display("Reading from Address 15 at %0t. Expected: 6734, Got: %d", $time, out1);
		    
	    $finish;
	end

endmodule

/////////////////////////////////////////////////////////////////////////////////////////////////////

module mp_top (clk, instruction, result);
   
	input clk;
    input [31:0] instruction;
    output reg [31:0] result;
  
	// Internal signals
    wire [5:0] opcode;
    wire [4:0] addr1, addr2, addr3;
    reg valid_opcode;
    wire [31:0] out1, out2;


    // Parse the instruction
    assign opcode = instruction[5:0];
    assign addr1 = instruction[10:6];
    assign addr2 = instruction[15:11];
    assign addr3 = instruction[20:16];
      always @(*) begin
        valid_opcode = (opcode == 6'd1)  || // a + b
                       (opcode == 6'd6)  || // a - b
                       (opcode == 6'd13) || // |a|
                       (opcode == 6'd8)  || // -a
                       (opcode == 6'd7)  || // max(a, b)
                       (opcode == 6'd4)  || // min(a, b)
                       (opcode == 6'd11) || // avg(a, b)
                       (opcode == 6'd15) || // not a
                       (opcode == 6'd3)  || // a or b
                       (opcode == 6'd5)  || // a and b
                       (opcode == 6'd2);    // a xor b
    end

    //Sample of the Register File based on valid_opcode
    reg_file reg_file_instance (
        .clk(clk),
        .valid_opcode(valid_opcode),
        .addr1(addr1),
        .addr2(addr2),
        .addr3(addr3),
        .in(valid_opcode ? result : 32'd0), //Don't write if opcode is not valid
        .out1(out1),
        .out2(out2)
    );	
    //Sample of the ALU based on valid_opcode
    alu alu_instance (
        .opcode(valid_opcode ? opcode : 6'd0), //Disable ALU if opcode is not valid
        .a(out1),
        .b(out2),
        .result(result)
    ); 

	
endmodule  

//System Test bench
module mp_top_tb;	  
	
	reg clk;
	reg [31:0] instructionss[0:11];
	reg [31:0] instruction;	
	integer currInstruction;
	wire signed [31:0] result;	   
	
	reg signed [31:0] expResult;		
	reg signed [31:0] expResults[0:10];
	
	initial begin
		
		expResults[0] = 24486 ;
		expResults[1] = 3274 ;
		expResults[2] = 5338 ;
		expResults[3] =-13200 ;
		expResults[4] = 12996 ;
		expResults[5] = 1852 ;
		expResults[6] = 4674 ;
		expResults[7] =-1049 ;
		expResults[8] = 16294 ;
		expResults[9] = 5376 ;
		expResults[10] = 4626 ; 
		
	end	
	
	initial 					
	begin
		clk = 0;	
		currInstruction = 0; 
		instructionss[0] = 32'b00000000000000000000100010000001; // r0 = r1+r2	 //24486	
		instructionss[1] = 32'b00000000000111110001100110000110; // r31 = r6-r3	 //3274 
		instructionss[2] = 32'b00000000000000001011111000001101; // r0 = |r24| ,r23 	//5338	
		instructionss[3] = 32'b00000000000111110111010010001000; // r31 = -r18 ,r14   //-13200		
		instructionss[4] = 32'b00000000000000000000110110000111; // r0 = max(r22,r1)	//12996	
		instructionss[5] = 32'b00000000000111111101011011000100; // r31 = min(r27,r26)  //1852	   
		instructionss[6] = 32'b00000000000000000010100100001011; // r0= avg(r4,r5)      //4674	
		instructionss[7] = 32'b00000000000111110010111101001111; // r31 = ~r29 ,r5     //~1048=-1049		
		instructionss[8] = 32'b00000000000000000101110000000011; // r0 = r16 | r11     //7430|12196=16294
		instructionss[9] = 32'b00000000000111111000111001000101; // r31 = r25 & r17	 //5544&14102=5376 	
		instructionss[10] = 32'b00000000000000001111011101000010; // r0 = r29 ^ r30     // 1048^5642=4626  
		instructionss[11] = 32'b00000000000000001111011101001001;  //test for unvalid opcode (9)
	end		

	always #10ns clk = ~clk;

	always @(posedge clk) begin
		if (currInstruction < 12) begin
			instruction = instructionss[currInstruction];
			expResult = expResults[currInstruction];			
           	currInstruction = currInstruction + 1;
		end
		#10ns
			if (result == expResult) 
			  $display("Current Instruction = %h, Result = %d, Expected = %d, Pass", instruction, result, expResult);
			
			else 
			  $display("Current Instruction = %h, Result = %d, Expected = %d, Fail", instruction, result, expResult);	
			
		if (currInstruction == 12)
			$finish;		
    end														
	
	//Sample of the system
   	mp_top mpTop(clk, instruction, result);

endmodule