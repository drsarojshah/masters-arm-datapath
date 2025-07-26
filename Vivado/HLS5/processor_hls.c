#include <stdio.h>
#include <stdlib.h>

// Synthesizable HLS-compatible processor function

static int main_memory[16][1] = {{0x8B0F01CD}, // ADD R13, R14, R15
                                {0xCB0F01CD}, // SUB R13, R14, R15
                                {0x8A0F01CD}, // AND R13, R14, R15
                                {0xAA0F01CD}, // ORR R13, R14, R15
                                {0x910031ED}, // ADDI R13, R14, #12
                                {0xD10011CD}, // SUBI R13, R14, #5
                                {0xF84001CB}, // LDUR R11, [R14, #0]
                                {0xF80001EC}, // STUR R12, [R15, #0]
                                {0x00000000}, {0x00000000}, {0x00000000}, 
                                {0x00000000}, {0x00000000}, {0x00000000},
                                {0x00000009}, // Data for LDUR
                                {0x00000006}  // Data for LDUR
};

// Initialize register file with zero
static int register_file[16][1] = {{0x00000000},{0x00000000},{0x00000000},{0x00000000},
                                {0x00000000},{0x00000000},{0x00000000},{0x00000000},
                                {0x00000000},{0x00000000},{0x00000000},{0x00000000},
                                {0x00000000},{0x00000000},{0x00000000},{0x00000000}
};

void processor(int pc_in, int* main_memory_out, int* opcode_out, int* op_value_1_out, int* op_value_2_out, int* result_out){
    typedef enum datapath {
        inst_fetch_stage,
        inst_decode_stage,
        operand_fetch_stage,
        execution_stage,
        wb_store_result,
    } pipeline_stage;

    pipeline_stage current_stage, next_stage;

    int prog_count = pc_in;
    int inst_reg = 0;
    int result = 0;

    int temp_opcode = 0;
    int opcode = 0;
    int field_1 = 0, field_2 = 0, field_3 = 0, field_4 = 0, field_5 = 0;
    int op_value_1 = 0, op_value_2 = 0;
    int imm12 = 0;
    int address = 0;

    int numb_arith_ops = 0;   
    current_stage = inst_fetch_stage;

    if (prog_count < 8){
     
        if(current_stage == inst_fetch_stage){
            inst_reg = main_memory[prog_count][0];
            *main_memory_out = inst_reg;                
            current_stage = inst_decode_stage;                
        }

       if (current_stage == inst_decode_stage){
            register_file[14][0] = main_memory[14][0];
            register_file[15][0] = main_memory[15][0];

            temp_opcode = (inst_reg >> 22) & 0x3FF;
            if (temp_opcode == 0x244 || temp_opcode == 0x344) {
                opcode = temp_opcode;
                imm12   = (inst_reg >> 10) & 0xFFF;
                field_3 = (inst_reg >> 5) & 0x1F;
                field_4 = inst_reg & 0x1F;
            } else {
                opcode = (inst_reg >> 21) & 0x7FF;
                field_1 = (inst_reg >> 16) & 0x1F;
                field_2 = (inst_reg >> 10) & 0x3F;
                field_3 = (inst_reg >> 5) & 0x1F;
                field_4 = inst_reg & 0x1F;
                address = (inst_reg >> 12) & 0x1FF;
                field_5 = inst_reg & 0x1F;
            } 
            *opcode_out = opcode;                      
            current_stage = operand_fetch_stage;
        }

        if(current_stage == operand_fetch_stage){
            op_value_1 = register_file[field_1][0];
            op_value_2 = register_file[field_3][0]; 
            *op_value_1_out = op_value_1;
            *op_value_2_out = op_value_2;                   
            current_stage = execution_stage;
        }

        if (current_stage == execution_stage){
            switch (opcode){
                case 0x458:
                    result = (op_value_1 + op_value_2);                   
                    numb_arith_ops++;
                    break;
                case 0x658:
                    result = (op_value_2 - op_value_1);                   
                    numb_arith_ops++;
                    break;
                case 0x450:
                    result = (op_value_1 & op_value_2);                    
                    numb_arith_ops++;
                    break;
                case 0x550:
                    result = (op_value_1 | op_value_2);                    
                    numb_arith_ops++;
                    break;
                case 0x244:
                    result = (op_value_2 + imm12);                   
                    numb_arith_ops++;
                    break;
                case 0x344:
                    result = (op_value_2 - imm12);                    
                    numb_arith_ops++;
                    break;
                case 0x7C2:
                    register_file[field_5][0] = main_memory[field_3 + address][0];                    
                    break;
                case 0x7C0:
                    main_memory[field_5 + address][0] = register_file[field_3][0];                    
                    break;
                default:
                    result = 0;
                    break;
            }            
            current_stage = wb_store_result;
        }

        if(current_stage == wb_store_result){  
            if (opcode == 0x7C2) {
                *result_out = register_file[field_5][0];                    

            }else if (opcode == 0x7C0) {
                *result_out = main_memory[field_5 + address][0];
                
            } else {  
                register_file[field_4][0] = result;
                main_memory[field_4][0] = result;
                *result_out = result;
                
            }

        }
    
    } // End of five stage a pipeline 
    
    current_stage = inst_fetch_stage;
    
}
