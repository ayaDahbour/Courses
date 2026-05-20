module LOOP_ADDRESS (
    input clk,             // Clock signal
    input enable,          // Enable signal
    input [15:0] data_in,  // 16-bit input data
    output reg [15:0] data_out // 16-bit output data
);

    always @(posedge clk) begin
        if (enable) 
            data_out <= data_in; // Load data when enable = 1
        else
            data_out <= data_out; // Hold the current data
    end

endmodule
