/*
    Problem# "Modeling the execution engine (datapath) portion of a 32-bit ARM processor"    
    Programmer# S. Shah
    Date# 05-06-2025
    Reference : Moulic, James R. Class Notes, “---.”
                Muckell J. Class Notes, “---.”                
                Patterson, David A., and John L. Hennessy. Computer Organization and Design ARM Edition: The Hardware Software Interface. 1st ed., Morgan Kaufmann, 2017.
                Google.com,and others
*/
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
/*
    LEGv8 Opcodes (in hex)
    --------------------------
    e.g.: ADD instruction

        Machine code ADD: 0x8B0F01CD                                     
        binary => 1000 1011 000 | 0 1111| 0000 00| 01 110 | 0 1101
                    8    B    0    F        0       1   C     D
                |   opcode      |  Rm   | shamt  |    Rn  |  Rd   |

        OPCODE_ADD    0x458
        Instruction format:  ADD R13, R14, R15: 
        int instr_reg = (0x458 << 21) | (3 << 16) | (0 << 10) | (2 << 5) | 1;
                        [31.......21] | [20...16] | [15...10] | [9....5] | [4...0]
                        |    opcode   |    Rm     |  shamt    |    Rn    |   Rd   |
                        |      ADD    |  field_1  |  field_2  | field_3  |field_4 |
                        |      ADD    |  R15 = 6  |    0      |  R14 = 5 |R13 = 11|

    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    OPCODE_ADD   0x458      machine code => 0x8B0F01CD
    OPCODE_SUB   0x658      machine code => 0xCB0F01CD     SUB R13, R14, R15
    OPCODE_AND   0x450      machine code => 0x8A0F01CD     AND R13, R14, R15
    OPCODE_ORR   0x550      machine code => 0xAA0F01CD     ORR R13, R14, R15

    //OPCODE_ADDI  0x488      machine code => 0x910031ED     ADDI R13, R15, #12     10010001000 | 00000001100 | 01111 | 01101 => for 11 bit opcode
    //OPCODE_SUBI  0x688      machine code => 0xD10011CD     SUBI R13, R14, #4      11010001000 | 00000000100 | 00101 | 01101 => for 11 bit opcode

    OPCODE_ADDI  0x244      machine code => 0x910031ED     ADDI R13, R15, #12       1001000100  |000000001100 | 01111 | 01101  for 10 bit opcode
    OPCODE_SUBI  0x344      machine code => 0xD10011CD     SUBI R13, R14, #4        1101000100  |000000000100 | 00101 | 01101  for 10 bit opcode
																				   [opcode(10)] | [ imm(12)]  |[Rn(5)]| [Rd(5)]

    
    **For Load and Store, since my reg. and main memory size is small I am intentionally making address= 0 and op2= 0 
    Load: memory address => register
    OPCODE_LDUR  0x7C2      machine code => 0xF84001CB     LDUR R11, [R14, #00] where offset = 0 and op2 = 0     Binary=> 1111 1000 010 | 0 0000 0000  |    00    | 01 110   | 0 1011      
                                                           STUR Rt,  [Rn, #add]
    Store: register => memory address
    OPCODE_STUR  0x7C0      machine code => 0xF80001EC     STUR R12, [R15, #00] where offset = 0 and op2 = 0  Binary=>  1111 1000 000  | 0 0000 0000  |    00    | 01 111   | 0 1100         
                                                           STUR Rt,  [Rn, #add]                                         [ opcode(11) ] | [address(9)] | [op2(2)] | [ Rn(5)] | [ Rt(5) ]
                                                                                                                        [ opcode(11) ] | [address(9)] | [op2(2)] | field_3  | field_5)]

*/
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

#include "processor.h"
#include <stdio.h>
#include <stdlib.h>

// Initialize main_memory with instruction and data value
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

// Synthesizable HLS-compatible processor function
void processor(int pc_in, int* main_memory_out, int* opcode_out, int* op_value_1_out, int* op_value_2_out, int* result_out){
    typedef enum datapath {
        inst_fetch_stage,
        inst_decode_stage,
        operand_fetch_stage,
        execution_stage,
        wb_store_result,
    } pipeline_stage;

    pipeline_stage current_stage;

    int prog_count = pc_in;                     // program counter (PC)
    int inst_reg = 0;                           // instruciton register (IR)
    int result = 0;

    int temp_opcode = 0;
    int opcode = 0;
    int field_1 = 0;                            // a place holder for intermediatery destinaion register or Rm => Second operand register    
    int field_2 = 0;                            // shamt is for shift operation
    int field_3 = 0;                            // a place holder for intermediatery destinaion register or Rn => First operand register
    int field_4 = 0;                            // Rd => output register or  Destination register
    int field_5 = 0;                            // Rt => Target register (for load/store)
    
    int op_value_1 = 0;                         // intermediatery place holder for value in register field_3    
    int op_value_2 = 0;                         // intermediatery place holder for value in register field_1
    int imm12 = 0;
    int address = 0;

    int numb_arith_ops = 0;   
    current_stage = inst_fetch_stage;


    if (prog_count < 8){
        
        // Stage 0 - Instruction Fetch
        if(current_stage == inst_fetch_stage){
            inst_reg = main_memory[prog_count][0];                  // Fetch/load instruction from pc
            *main_memory_out = inst_reg;                
            current_stage = inst_decode_stage;                      // Move to next stage i.e.,decode stage             
        }

        // Stage 1 - Instruction Decode
       if (current_stage == inst_decode_stage){
            register_file[14][0] = main_memory[14][0];
            register_file[15][0] = main_memory[15][0];

            temp_opcode = (inst_reg >> 22) & 0x3FF;
            if (temp_opcode == 0x244 || temp_opcode == 0x344) {     // use this if instruction is not 11 bit i.e., it is for 10 bit binary
                opcode = temp_opcode;                               // 10-bit opcode for ADDI or SUBI
                // I-type decoding
                imm12   = (inst_reg >> 10) & 0xFFF;                 // 12-bit immediate
                field_3 = (inst_reg >> 5) & 0x1F;                   // Rn
                field_4 = inst_reg & 0x1F;                          //Rd
            } else {                                                // It take cares 11 bit binary  
                opcode = (inst_reg >> 21) & 0x7FF;                  // 11-bit opcode for others (opcode extraction)
                            
                // R-type/D-type decoding
                field_1 = (inst_reg >> 16) & 0x1F;                  // Rm => Second operand register or First field / Reg_15
                field_2 = (inst_reg >> 10) & 0x3F;                  // shamt is for shift operation
                field_3 = (inst_reg >> 5) & 0x1F;                   // Rn => First operand register or third field or base register for LDUR/STUR (for load/store) / Reg_14
                field_4 = inst_reg & 0x1F;                          // Rd => output register or  Destination register or last or fourth field
                
                // D-type Decoding
                address = (inst_reg >> 12) & 0x1FF;                 // address field for LDUR/STUR (for load/store)
                field_5 = inst_reg & 0x1F;                          // Rt => Target register for LDUR/STUR (for load/store)
            } 
            *opcode_out = opcode;                                   
            current_stage = operand_fetch_stage;                    // Move to next stage i.e.,Operand Fetch stage 
        }

        // Stage 2 - Operand Fetch
        if(current_stage == operand_fetch_stage){

            // Simulating operand fetch from reg_file
            // In a real case, there will be fetch from reg_file usingfield_3 and field_1 as register indices
            op_value_1 = register_file[field_1][0];                 // Fetch operand 1 from register field_1
            op_value_2 = register_file[field_3][0];                 // Fetch operand 2 from register field_3 register                    
            *op_value_1_out = op_value_1;
            *op_value_2_out = op_value_2;                   
            current_stage = execution_stage;                        // Move to next stage i.e., execution stage
        }

        // Stage 3 - Execution
        if (current_stage == execution_stage){
            switch (opcode){
                case 0x458:     // ADD operation
                    result = (op_value_1 + op_value_2);                    
                    numb_arith_ops++;
                    break;
                case 0x658:     // SUBTRACT operation
                    result = (op_value_2 - op_value_1);
                    numb_arith_ops++;
                    break;
                case 0x450:     // AND operation
                    result = (op_value_1 & op_value_2);
                    numb_arith_ops++;
                    break;
                case 0x550:     // OOR operation
                    result = (op_value_1 | op_value_2);
                    numb_arith_ops++;
                    break;
                case 0x244:     // ADDI for 10 bit opcode
                    result = (op_value_2 + imm12);
                    numb_arith_ops++;
                    break;      
                case 0x344:     // SUBI for 10 bit opcode
                    result = (op_value_2 - imm12);
                    numb_arith_ops++;
                    break;
                 
                // For Load and Store, since my reg. and main memory size is small thus I am intentionally making address = 0 and op2= 0 
                case 0x7C2:     // Load (ld) => Load value from memory address (field_3) = 14  into register (field_5) = Reg_11 
                    register_file[field_5][0] = main_memory[field_3 + address][0];      // 0xF84 00 1CB => 1111 1000 010 | 0 0000 0000  |    00    | 01 110   | 0 1011 
                    break;
                case 0x7C0:     // Store (st) => Store value from register (field_3) = 15 into memory address (field_5) = 12
                    main_memory[field_5 + address][0] = register_file[field_3][0];      //0xF80001EC => 1111 1000 000 | 0 0000 0000  |    00    | 01 111   | 0 1100 
                    break;
                default:
                    result = 0;
                    break;
            }            
            current_stage = wb_store_result;                            // Move to next stage i.e., Store and write-back stage
        }

        // Stage 4 - write back or Store Result Stage
        if(current_stage == wb_store_result){  
            if (opcode == 0x7C2) {     //LDUR                                 
                *result_out = register_file[field_5][0];                    

            }else if (opcode == 0x7C0) {    // STUR
                *result_out = main_memory[field_5 + address][0];
                
            } else {      //for others => WB to register and // field_4 = D or 13
                register_file[field_4][0] = result;
                main_memory[field_4][0] = result;
                *result_out = result;
                
            }

        }
    
    } // End of five stage of a pipeline 
    
    current_stage = inst_fetch_stage;
    
}
