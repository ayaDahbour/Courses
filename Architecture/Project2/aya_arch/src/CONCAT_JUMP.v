module CONCAT_JUMP (
    input [15:0] PC,
    input [8:0] offset,
    output [15:0] PC_jump
);
    
    assign PC_jump = {PC[15:9], offset};

endmodule
