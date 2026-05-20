module PerformanceRegisters (
    input wire clk,                      // Clock signal
    input wire flag_executed,            // Flag: Executed instruction
    input wire flag_load,                // Flag: Load instruction
    input wire flag_store,               // Flag: Store instruction
    input wire flag_alu,                 // Flag: ALU instruction
    input wire flag_control,             // Flag: Control instruction
    output reg [15:0] total_executed,    // Total executed instructions
    output reg [15:0] total_load,        // Total load instructions
    output reg [15:0] total_store,       // Total store instructions
    output reg [15:0] total_alu,         // Total ALU instructions
    output reg [15:0] total_control,     // Total control instructions
    output reg [15:0] total_cycles       // Total clock cycles
);	

    // Initialize registers to zero
    initial begin
        total_executed = 0;
        total_load = 0;
        total_store = 0;
        total_alu = 0;
        total_control = 0;
        total_cycles = 0;
    end

    // Increment counters based on flags
    always @(posedge clk) begin
        total_cycles = total_cycles + 1;  // Increment clock cycle counter

        // Increment counters based on flags
        if (flag_executed == 1)
            total_executed = total_executed + 1;

        if (flag_load == 1)
            total_load = total_load + 1;

        if (flag_store == 1)
            total_store = total_store + 1;

        if (flag_alu == 1)
            total_alu = total_alu + 1;

        if (flag_control == 1)
            total_control = total_control + 1;
    end
endmodule
