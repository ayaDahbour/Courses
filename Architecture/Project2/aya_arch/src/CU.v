module CU (
    input wire clk,              // Clock signal
    input wire [3:0] Opcode,     // Opcode of the instruction (4 bits)
    input wire [2:0] Func,       // Function code of the instruction (3 bits)
    input wire EQ_B,             // Equality flag, used in branching (e.g., BEQ, BNE)
	
	output reg [3:0] state_D,
    output reg PCWrite,          // Control signal to write to PC
    output reg MemRead,          // Control signal to read from memory
    output reg MemWrite,         // Control signal to write to memory
    output reg MemToReg,         // Control signal for memory-to-register operation
    output reg IRWrite,          // Control signal to write to Instruction Register
    output reg AND_I,            // Control signal for ANDI operation
    output reg LOOP,             // Control signal for LOOP operation
    output reg enable_RR,        // Control signal to enable register read
    output reg rst_RR,           // Control signal to reset register read

    output reg flag_executed,    // Flag to indicate instruction execution
    output reg flag_load,        // Flag to indicate load operation (LW)
    output reg flag_store,       // Flag to indicate store operation (SW)
    output reg flag_alu,         // Flag to indicate ALU operation
    output reg flag_control,     // Flag to indicate control instruction
    output reg [1:0] PCSource,   // Control signal for PC source selection
    output reg first_instruction, // Flag indicating if this is the first instruction
    output reg I_type,           // Flag indicating if the instruction is of I-type
    output reg RegWrite,         // Control signal to write to registers
    output reg regWriteA,        // Control signal to write to register A
    output reg regWriteB         // Control signal to write to register B
);

    // Instruction OpCodes for R-Type, I-Type, and J-Type operations
    // R-Type instructions
    parameter AND     = 7'b0000000;
    parameter ADD     = 7'b0000001;
    parameter SUB     = 7'b0000010;
    parameter SLL     = 7'b0000011;
    parameter SLR     = 7'b0000100;

    // I-Type instructions
// I-Type instruction parameters
parameter ANDI_4    = 4'b0010;   // 4-bit length variant
parameter ADDI_4    = 4'b0011;
parameter LW_4      = 4'b0100;
parameter SW_4      = 4'b0101;
parameter BEQ_4     = 4'b0110;
parameter BNE_4     = 4'b0111;
parameter FOR_4     = 4'b1000;

// I-Type instructions as 7-bit registers
reg [6:0] ANDI;
reg [6:0] ADDI;
reg [6:0] LW;
reg [6:0] SW;
reg [6:0] BEQ;
reg [6:0] BNE;
reg [6:0] FOR;


// Assigning values
assign ANDI = {ANDI_4, Func};
assign ADDI = {ADDI_4, Func};
assign LW   = {LW_4, Func};
assign SW   = {SW_4, Func};
assign BEQ  = {BEQ_4, Func};
assign BNE  = {BNE_4, Func};
assign FOR  = {FOR_4, Func};


    // J-Type instructions
    parameter J_offset= 7'b0001000;
    parameter C_offset= 7'b0001001;
    parameter RET     = 7'b0001010;
	
	
	
	
    reg [6:0] operand;    // Combined Operand (Opcode + Func)
	
	assign operand = {Opcode, Func};
	
    reg [3:0] state = 10; // State variable for the control unit FSM

    // State encoding
    parameter RS  = 0;
    parameter IF  = 1;
    parameter pID = 2;
    parameter ID  = 3;
    parameter pEX = 4;
    parameter EX  = 5;
    parameter pMEM= 6;
    parameter MEM = 7;
    parameter pWB = 8;
    parameter WB  = 9;
    parameter start = 10;  
	
	always @(posedge clk) begin
		
		state_D = state;
		
	end

 

    always @(posedge clk) begin    // Synchronous block triggered by clock

        case (state)
            start: begin
                first_instruction <= 1;     // Set the first instruction flag
                state <= IF;                // Transition to Fetch state
			              
                MemRead <= 0;              
                MemWrite <= 0;
                IRWrite <= 1;              
                RegWrite <= 0;

                // Clear flags
                flag_load <= 0;
                flag_store <= 0;
                flag_alu <= 0;
                flag_control <= 0;
                flag_executed <= 0;

                // Clear additional control signals
                AND_I <= 0;
                LOOP <= 0;
                enable_RR <= 0;
                rst_RR <= 0;
                I_type <= 0;
                regWriteA <= 0;
                regWriteB <= 0;
            end

            RS: begin
                // Initialize control signals for Reset state
                PCWrite <= 1;              
                MemRead <= 0;              
                MemWrite <= 0;
                IRWrite <= 1;              
                RegWrite <= 0;

                // Clear flags
                flag_load <= 0;
                flag_store <= 0;
                flag_alu <= 0;
                flag_control <= 0;
                flag_executed <= 0;

                // Clear additional control signals
                AND_I <= 0;

                enable_RR <= 0;
                rst_RR <= 0;
                I_type <= 0;
                regWriteA <= 0;
                regWriteB <= 0;

                state <= IF;  // Transition to Instruction Fetch state
            end

            IF: begin
                // First instruction is done, clear flag
                first_instruction <= 0;				   
			   LOOP <= 0;
                PCWrite <= 0;
                IRWrite <= 1;
                regWriteA <= 1;        // Set control signals for register write
                regWriteB <= 1; 

                state <= ID;           // Transition to Instruction Decode state
            end

            ID: begin
                // Determine PCSource based on branching conditions	
				
				case (operand)
    				BEQ: PCSource <= (EQ_B) ? 2'b01 : 2'b00;
    				BNE: PCSource <= (EQ_B) ? 2'b00 : 2'b01;
    				RET: PCSource <= 2'b11;
				J_offset:PCSource <= 2'b10;
				C_offset:PCSource <= 2'b10;
    				default: PCSource <= 2'b00;
				endcase


                // Set control signals based on the instruction operand
                if (operand == ANDI) AND_I <= 1;
                if (operand == FOR) LOOP <= 1;
                if (operand == ANDI || operand == ADDI || operand == AND
                    || operand == ADD || operand == SUB || operand == SLL || operand == SLR)
                    flag_alu <= 1;

                // Set control flag for control instructions (branches, jumps, etc.)
                if (operand == BNE || operand == BEQ || operand == J_offset
                    || operand == C_offset || operand == RET || operand == FOR)
                    flag_control <= 1;

                // Set flag for I-type instructions
                if (operand == ANDI || operand == ADDI || operand == BEQ || operand == BNE
                    || operand == LW || operand == SW || operand == FOR) I_type <= 1;

                // Set flags for load and store instructions
                if (operand == LW) flag_load <= 1;
                if (operand == SW) flag_store <= 1;

                // Control signal for MemToReg
                MemToReg <= (operand != LW);
                enable_RR <= (operand == C_offset);
                RegWrite <= 0;
                flag_executed <= 1;     // Instruction is executed

                state <= pID;           // Transition to Pre-Instruction Decode state
            end

            pID: begin
                // Clear unnecessary flags
                regWriteA <= 0;
                regWriteB <= 0;
               #5 flag_load <= 0;
               #5 flag_store <= 0;
               #5 flag_alu <= 0;
               #5 flag_control <= 0;
               #5 flag_executed <= 0;
               #5 enable_RR <= 0;
			  
			   if (operand == FOR)
				   state <= RS;	
			   else
                	state <= EX;           // Transition to Execute state
            end

            EX: begin
                // Transition based on the operand type	 

                case (operand)
                    AND, ADD, SUB, SLL, SLR, ANDI, ADDI: state <= pWB;  // ALU operations lead to Write-back
                    LW, SW: state <= MEM;      // Memory operations lead to memory stage
                    BEQ, BNE, FOR, J_offset, C_offset, RET:begin state <= RS; end// Branch and jump operations return to Reset state
                endcase
            end

            MEM: begin
                // Set control signals for memory operations
                MemRead <= (operand == LW);
                MemWrite <= (operand == SW);

                // State transition based on memory operation
                case (operand)
                    LW: state <= pWB;  // Load instruction moves to Write-back stage
                    SW: state <= RS;   // Store instruction returns to Reset state
                endcase
            end

            pWB: begin
                RegWrite <= 1;     // Set Register Write control signal
                state <= WB;       // Transition to Write-back state
            end

            WB: begin
                RegWrite <= 1;     // Set Register Write control signal
                state <= RS;       // Return to Reset state
            end

        endcase
    end

endmodule