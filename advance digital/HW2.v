//1201738 aya dahbour 


// Define the parameters
module Moore (
  input clk,                  
  input x,                     
  input reset_n,               // Active-low asynchronous reset
  output reg z                 
);

  parameter S0 = 2'b00, S1 = 2'b01, S2 = 2'b11, S3 = 2'b10;  // Define states

  reg [1:0] state;              // State register

  always @(posedge clk or negedge reset_n)
    if (~reset_n) 
      state = S0;              // Transition to state 0 on reset
    else
      case (state)			 // State transitions based on input x & current state
        S0: if (x) state = S1; else state = S3;   
        S1: if (x) state = S2; else state = S0;  
        S3: if (x) state = S1; else state = S2;  
        S2: if (~x) state = S1; else state = S2;
      endcase;

  always @(posedge clk or negedge reset_n)
    case (state)
      S0: z = 0 ;               // Output logic based on the state
      S1: z = 1 ;
      S3: z = 1 ;
      S2: z = 0 ;
    endcase;

endmodule	 

///////////////////////////////////////////

module DFlipFlop (
  input clk,                   
  input reset,                 
  input  d,                    
  output reg q                 
);
  always @(posedge clk or negedge reset)
    if (~reset)
      q <= 1'b0;                // Reset output to 0 on reset
    else
      q <= d;                   // Pass input to output

endmodule

////////////////////////////////////////

module Circuit1 (
  input Clock, x, Reset,        
  output reg Z                 
);
  wire qA, qB;

  DFlipFlop uutA (              // Instantiate D flip-flop A
    .clk(Clock),
    .reset(Reset),
    .d(x ^ (~qB)),              // XOR input with inverted qB
    .q(qA)
  );

  DFlipFlop uutB (              // Instantiate D flip-flop B
    .clk(Clock),
    .reset(Reset),
    .d(qA | x),                 // OR input with qA
    .q(qB)
  );

  assign Z = qB ^ qA;           // Output logic

endmodule	   

///////////////////////////////////////

module Circuit2 (
  input Clock, x, Reset,       
  output reg Z                   
);
  wire qA, qB;

  DFlipFlop uutA (              // Instantiate D flip-flop A
    .clk(Clock),
    .reset(Reset),
    .d(x ^ (~qB)),              // XOR input with inverted qB
    .q(qA)
  );

  DFlipFlop uutB (              // Instantiate D flip-flop B
    .clk(Clock),
    .reset(Reset),
    .d(qA ^ x),                 // XOR input with qA
    .q(qB)
  );

  assign Z = qB ^ qA;           // Output logic

endmodule

//////////////////////////////////////

// Test Bench for the state diagram & circuit 1
module Moore_tb1;

  parameter CLK_PERIOD = 10;     
  parameter NUM_TEST_CASES = 5;  // Number of test cases

  reg clk, x, reset_n;
  wire z_moore, z_circuit1;

  Moore uut_moore (             // Instantiate Moore module
    .clk(clk),
    .x(x),
    .reset_n(reset_n),
    .z(z_moore)
  );

  Circuit1 uut_circuit1 (        // Instantiate Circuit1 module
    .Clock(clk),
    .x(x),
    .Reset(reset_n),
    .Z(z_circuit1)
  );

  always #((CLK_PERIOD / 2)) clk = ~clk;

  initial begin
    clk = 0;
    x = 0;
    reset_n = 0;

    #10 reset_n = 1;

    for (int i = 0; i < NUM_TEST_CASES; i = i + 1) begin	// Display the output 
      #10 x = i % 2;
      #50;

      if (z_moore === z_circuit1) begin
        $display("Test Case %0d: PASS", i);
      end else begin
        $display("Test Case %0d: FAIL", i);
      end
    end

    $finish;
  end

endmodule  

////////////////////////////////////////////

// Test Bench for the state diagram & circuit 2
module Moore_tb2;

  parameter CLK_PERIOD = 10;     
  parameter NUM_TEST_CASES = 5;  

  reg clk, x, reset_n;
  wire z_moore, z_circuit1;

  Moore uut_moore (             // Instantiate Moore module
    .clk(clk),
    .x(x),
    .reset_n(reset_n),
    .z(z_moore)
  );

  Circuit2 uut_circuit2 (        // Instantiate Circuit2 module
    .Clock(clk),
    .x(x),
    .Reset(reset_n),
    .Z(z_circuit1)
  );

  always #((CLK_PERIOD / 2)) clk = ~clk;

  initial begin
    clk = 0;
    x = 0;
    reset_n = 0;

    #10 reset_n = 1;

    for (int i = 0; i < NUM_TEST_CASES; i = i + 1) begin
      #10 x = i % 2;
      #50;

      if (z_moore === z_circuit1) begin
        $display("Test Case %0d: PASS", i);
      end else begin
        $display("Test Case %0d: FAIL", i);
      end
    end

    $finish;
  end

endmodule
