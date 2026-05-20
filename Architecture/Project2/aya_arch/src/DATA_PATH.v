module DATA_PATH(
	//input wire clk
	);
	reg clk;
	
	// wires for  Performance Registers
	wire [15:0]total_executed, total_load, total_store, 
			  total_alu, total_control, total_cycles;  
	
	
	//wire to handle the loop address
	wire [15:0]loop_address;
	
	//PC diffrent values , one value choosen using PC_control
	wire [15:0] PC_value;				//now value
	wire [15:0] PC_plus_one;			//PC+1(normal instruction procidural)
	wire [15:0] PC_branch; 				//Branch value
	wire [15:0] PC_next, PC_next_1;		//the final PC value
	wire [15:0] PC_jump;				//Jump value
	wire [15:0] PC_RR; 					//return address PC
	wire [1:0]  PCSource;				//the control	 
	wire first_instruction;				//to make PC=0
	
	//ALU operands and result
	wire [15:0]alu_operand_1;		
	wire [15:0]alu_operand_2;
	wire [15:0]alu_result;	
	
	//ALU flags
	wire zeroFlag, carryFlag, negativeFlag;
	
	//register file output and input 
	wire [15:0]Reg_out_1;
	wire [15:0]Reg_out_2;
	wire [15:0]LOAD_DATA;  				//the value to write
	
	//RF output register
	wire [15:0]reg_out_1;
	wire [15:0]reg_out_2;
	
	wire [15:0]instruction;				//instruction (input to CU) 
	
	//instruction Decode (input to CU) (used as inputs for MUXs and extenders)
	wire [2:0] Func;
	wire [3:0] Opcode;
	//r type
	wire [2:0] Rsr;  
	wire [2:0] Rtr; 
	wire [2:0] Rdr;
	//i type
	wire [2:0] Rsi;  
	wire [2:0] Rti;
	//offset and immediate
	wire [8:0] offset;
	wire [5:0] imm;
	 	
	//input for read/write addresses in reg file
	wire [2:0]Rs1,Rd,Rs2;  
	
	//extend signed and unsigned for immediate use in I type
	wire [15:0]Unsigned_Extend_6,Signed_Extend_6,Extended_6;
	
	//the acctual address of the memory (register value + signed imm)
	wire [15:0] DM_address;
	
	//register to handle the alu result
	wire [15:0] ALUout;

	//////CU FLAGS//////	 
	wire EQ_B; 							//flag for BEQ,BNE check (input)
	wire PCWrite;						//flag to give the PC new value (output)
	wire RegWrite; 						//flag to give the reg file new value (output)
	wire MemRead; 						//flag to read the memory
	wire MemWrite;						//flag to write on the memory
	wire MemToReg;						//flag to know the source for RF (memory or ALU)
	wire I_type;						//flag used in the reg file to choose the address of the regs
	wire LOOP;							//flag to indecate that the ins is FOR
	wire decrement_loop;				//flag to decrement the loop counter
	wire loop_end;						//wire to indecat that the loop finished(input)
	wire AND_I;							//wire to choose sign or unsign extender
	wire enable_RR; 					//wire to save the current PC to RR reg 
	wire rst_RR;						//wire to reset RR after the CALL
	wire in_loop; 						//wire used to indecate the PC value  
	wire dec_reg_F;						//flag to make the R decremented	
	wire IRWrite;
	
	//flags to write RF value
	wire regWriteA;											
	wire regWriteB;
	
	//flages for  Performance Registers
	wire flag_executed;					
	wire flag_load; 					
	wire flag_store;					
	wire flag_alu;						
	wire flag_control;	
	
	and (dec_reg_F,LOOP,~loop_end); 	//decrement the register with ittertion number (in reg file)  
	
	
	wire  [3:0] state;
	    // Instantiate the Control Unit (CU)
    CU cu_instance (
        .clk(clk),
        .Opcode(Opcode),
        .Func(Func),
        .EQ_B(EQ_B),
		
		.state_D(state),
        .PCWrite(PCWrite),
        .MemRead(MemRead),
        .MemWrite(MemWrite),
        .MemToReg(MemToReg),
        .IRWrite(IRWrite),
        .AND_I(AND_I),
        .LOOP(LOOP),
        .enable_RR(enable_RR),
        .rst_RR(rst_RR),
        
        .flag_executed(flag_executed),
        .flag_load(flag_load),
        .flag_store(flag_store),
        .flag_alu(flag_alu),
        .flag_control(flag_control),
        
        .PCSource(PCSource),
		.first_instruction(first_instruction),
        .I_type(I_type),
        .RegWrite(RegWrite),
        .regWriteA(regWriteA),
        .regWriteB(regWriteB)
    );
	
	
	ALU alu (
				Reg_out_1, 				//always the ALU take the RS reg as input
				alu_operand_2,			//this value is output os the mux
				
				//func and opcode to choose the operation
				Opcode,				    
				Func,
				
				alu_result,				//the output
				
				//flags
				zeroFlag, 
				carryFlag, 
				negativeFlag
			);	  
	
	//store the result to the ALU register
	ALUoutReg ALUOutRegsiterComponent(clk, alu_result, ALUout);
	
	
	//value of PC choosing 
	mux4x1 PC_MUX_4 (
						PC_plus_one, 
	 					PC_branch,
	 					PC_jump,
	 					PC_RR,
	 					PCSource,
	 					PC_next_1
	 				); 
					 
	mux2to1_16_bit PC_MUX_2 (PC_next_1, loop_address, (LOOP&~loop_end), PC_next);
	
	
	
	//Instruction memory take PC return instruction to IR				 
	InstructionMemory IM(
	 						PC_value,
	 						instruction,
	 						PC_plus_one
	 					);
	
	//IR devided
	InstructionRegister IR(
								clk, 
								IRWrite, 
								instruction, 
								Func, 
								Rsr, 
								Rtr,
								Rdr,
								Rsi, 
								Rti,
								offset,
								imm, 
								Opcode
							); 
		
	//PC register with the current and the next value						
	PCreg PC_reg(
	
		clk, 
		PC_next, 
		PC_value, 
		PCWrite					//CU flag
		
	);	   
	
	//MUXs to choose the Rs1 and Rs2(as output reg address)and Rd (as input reg address)
	mux2to1_3_bit mux_Rs1	(Rsr,Rsi,I_type,Rs1); 
	mux2to1_3_bit mux_Rs2	(Rtr,Rti,I_type,Rs2);
	mux2to1_3_bit mux_Rd	(Rdr,Rti,I_type,Rd);  
	
	//MUX to choose the source of the reg input MEM or ALU
	mux2to1_16_bit mux_reg_write (memDataRegOut, ALUout, MemToReg,	LOAD_DATA);	 
	
	
	
	
	//register file instance
	registers regFile(clk, RegWrite, dec_reg_F, Rs2, Rs1, Rs2, Rd, LOAD_DATA, Reg_out_1, Reg_out_2);	
	
	    // regB
    registerB regBComponent(regWriteB, Reg_out_2, reg_out_2);

    // regA
    registerA regAComponenet(regWriteA, Reg_out_1, reg_out_1); 
	
	
	
	
	
	//signed extender of imm
	Sign_Extend_6 SE6 (imm,Signed_Extend_6);
	
	//unsigned extender of imm (for I_AND)
	Unsigned_Extend_6 UE6 (imm,Unsigned_Extend_6);
	
	//adder to calculate the memory address
	MEM_ADDER dm_address(reg_out_1,Signed_Extend_6,DM_address);
	
	//data memory
	DataMemory DM(clk, DM_address, reg_out_2, MemRead, MemWrite, dataOut);
	
	// register to hold the DM output
	memDataReg MDR(clk, dataOut, memDataRegOut);
	
	//MUX to choose wich extending type 
	mux2to1_16_bit extended_imm	(Signed_Extend_6,Unsigned_Extend_6,AND_I,Extended_6);
	
	//ALU operand 2 choosing
	mux2to1_16_bit ALU_Operand_2 (Reg_out_2,Extended_6,I_type, alu_operand_2); 
	
	//JUMP calculations
	CONCAT_JUMP JUMP(PC_value,offset,PC_jump);	   
	
	//RR register to save the call PC
	RR  RR_reg(clk,rst_RR,enable_RR,PC_plus_one,PC_RR);
	
	//EQ checker for BEQ and BNE
	EQ_check EQ_branch (reg_out_1, reg_out_2, EQ_B);  	
	
	wire [15:0] reg_value;
	
	//register to hold the loop numbver of iterations left
	register_decrement RD(clk, LOOP, dec_reg_F, reg_out_2, loop_end, reg_value);
	
	//register to hold the loop instruction address
	LOOP_ADDRESS LA(clk, LOOP,reg_out_1,loop_address);
		
	//Branch adder to calculate the next PC	
	branchAdder BR_ADD(clk, PC_value, Signed_Extend_6, PC_branch);

	// Performance Registers
	PerformanceRegisters PR(
    .clk(clk),
    .flag_executed(flag_executed),
    .flag_load(flag_load),
    .flag_store(flag_store),
    .flag_alu(flag_alu),
    .flag_control(flag_control),
    .total_executed(total_executed),
    .total_load(total_load),
    .total_store(total_store),
    .total_alu(total_alu),
    .total_control(total_control),
    .total_cycles(total_cycles)
);

	 
	
	
	    // Inputs	 

    always
    #20 clk = ~clk; 

    initial begin
        // Initialize Inputs
        clk = 0;
		
		#20
		clk = 0; 
		#5000
		$finish;
    end	
	
	
endmodule	  



