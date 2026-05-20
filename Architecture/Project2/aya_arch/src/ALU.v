module ALU(
  input [15:0] operandA,
  input [15:0] operandB,
  input [3:0] opcode,
  input [2:0] Function,
  output reg [15:0] result,
  output reg zeroFlag = 0,
  output reg carryFlag = 0,
  output reg negativeFlag = 0
);

  always @* begin
    // Reset flags
    zeroFlag = 0;
    carryFlag = 0;
    negativeFlag = 0;

    case (opcode)
      4'b0000: // Function-based operations
        case (Function)
          3'b000: // AND
            result = operandA & operandB;

          3'b001: // ADD
            begin
              result = operandA + operandB;
              carryFlag = (result < operandA);
            end

          3'b010: // SUB
            begin
              result = operandA - operandB;
              negativeFlag = (result > operandA);
            end

          3'b011: // Logical Left Shift
            result = operandA << operandB[3:0]; // Consider lower 4 bits for shifts

          3'b100: // Logical Right Shift
            result = operandA >> operandB[3:0];
          
          default:
            result = 16'b0; // Default case to avoid undefined behavior
        endcase

      4'b0010: // AND operation
        result = operandA & operandB;

      4'b0011: // ADD operation
        begin
          result = operandA + operandB;
          carryFlag = (result < operandA);
        end

      default:
        result = 16'b0; // Default case for opcode
    endcase

    // Zero flag update
    zeroFlag = (result == 16'b0);
  end
endmodule






module ALU_tb;

  // Inputs
  reg [15:0] operandA;
  reg [15:0] operandB;
  reg [3:0] opcode;
  reg [2:0] Function;

  // Outputs
  wire [15:0] result;
  wire zeroFlag;
  wire carryFlag;
  wire negativeFlag;

  // Instantiate the ALU module
  ALU uut (
    .operandA(operandA),
    .operandB(operandB),
    .opcode(opcode),
    .Function(Function),
    .result(result),
    .zeroFlag(zeroFlag),
    .carryFlag(carryFlag),
    .negativeFlag(negativeFlag)
  );

  initial begin
    // Initialize inputs
    operandA = 16'd0;
    operandB = 16'd0;
    opcode = 4'b0000;
    Function = 3'b000;

    // Monitor signals
    $monitor("Time=%0t | opcode=%b, Function=%b | operandA=%d, operandB=%d | result=%d, zeroFlag=%b, carryFlag=%b, negativeFlag=%b",
             $time, opcode, Function, operandA, operandB, result, zeroFlag, carryFlag, negativeFlag);

    // Test Case 1: AND operation (opcode=0000, Function=000)
    #10 operandA = 16'hF0F0; operandB = 16'h0F0F; opcode = 4'b0000; Function = 3'b000;
    
    // Test Case 2: ADD operation (opcode=0000, Function=001)
    #10 operandA = 16'd10; operandB = 16'd20; opcode = 4'b0000; Function = 3'b001;

    // Test Case 3: SUB operation (opcode=0000, Function=010)
    #10 operandA = 16'd50; operandB = 16'd25; opcode = 4'b0000; Function = 3'b010;

    // Test Case 4: Logical Left Shift (opcode=0000, Function=011)
    #10 operandA = 16'd8; operandB = 16'd1; opcode = 4'b0000; Function = 3'b011;

    // Test Case 5: Logical Right Shift (opcode=0000, Function=100)
    #10 operandA = 16'd16; operandB = 16'd1; opcode = 4'b0000; Function = 3'b100;

    // Test Case 6: AND operation (opcode=0010)
    #10 operandA = 16'hFFFF; operandB = 16'h000F; opcode = 4'b0010; Function = 3'bxxx;

    // Test Case 7: ADD operation (opcode=0011)
    #10 operandA = 16'd32768; operandB = 16'd32768; opcode = 4'b0011; Function = 3'bxxx;

    // Test Case 8: Zero Flag Check (AND resulting in zero)
    #10 operandA = 16'h0000; operandB = 16'hFFFF; opcode = 4'b0000; Function = 3'b000;

    // End simulation
    #10 $finish;
  end

endmodule
