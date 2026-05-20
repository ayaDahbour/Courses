module RR (
    input wire clk,          // Clock input
    input wire rst,          // Reset input (active high)
    input wire enable,       // Enable signal
    input wire [15:0] d_in,   // 8-bit data input
    output reg [15:0] q_out   // 8-bit data output
);

    // Register behavior
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            q_out <= 16'b0;    // Reset the register to 0
        end else if (enable) begin
            q_out <= d_in;    // Load data into the register
        end
    end

endmodule
