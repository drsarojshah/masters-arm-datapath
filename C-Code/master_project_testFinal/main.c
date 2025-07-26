/*
    Problem# "Modeling the execution engine (datapath) portion of a 32-bit ARM processor"
    
    Programmer# S. Shah
    Date#
    Reference : Prof. Moulic, Prof. Muckel, Google.com, ChatGPT and others
*/
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
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
                                                           STUR Rt,  [Rn, #imm]
    Store: register => memory address
    OPCODE_STUR  0x7C0      machine code => 0xF80001EC     STUR R12, [R15, #00] where offset = 0 and op2 = 0  Binary=>  1111 1000 000 | 0 0000 0000  |    00    | 01 111    | 0 1100         
                                                           STUR Rt,  [Rn, #imm]                                         [ opcode(11) ] | [address(9)] | [op2(2)] | [ Rn(5)] | [ Rt(5) ]
                                                                                                                        [ opcode(11) ] | [address(9)] | [op2(2)] | field_3  | field_5)]

*/
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

# include<stdio.h>
# include<stdlib.h>

int main(int argc, char const *argv[]){

    typedef enum datapath{
        inst_fetch_stage,
        inst_decode_stage,
        operand_fetch_stage,
        execution_stage,
        wb_store_result,
    } pipeline_stage;  

    // Array of stage names corresponding to enum values
    const char *pipeline_stage_names[] = {
        "Instruction Fetch Stage",
        "Instruction Decode Stage",
        "Operand_Fetch Stage",
        "Execution Stage",
        "WB and Store Result",
    };

    // LEGv8 Register and Memory setup
    int main_memory[16][1] = {0x8B0F01CD, 0xCB0F01CD, 0x8A0F01CD, 0xAA0F01CD,  0x910031ED, 0xD10011CD, 0xF84001CB, 0xF80001EC, 0x00000000, 
                                0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000009, 0x00000006};
    int register_file[16][1] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
                                0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}; 

    pipeline_stage current_stage, next_stage;

    int prog_count = 0;                      // program counter (PC)
    int inst_reg = 0;                       // instruciton register (IR)
    int result;        

    int temp_opcode;
    int opcode;      
    int field_1;                            // a place holder for intermediatery destinaion register or Rm => Second operand register    
    int field_2;                            // shamt is for shift operation
    int field_3;                            // a place holder for intermediatery destinaion register or Rn => First operand register
    int field_4;                            // Rd => output register or  Destination register
    int field_5;                            // Rt => Target register (for load/store)


    int op_value_1;                         // intermediatery place holder for value in registerfield_3    
    int op_value_2;                         // intermediatery place holder for value in register field_1
    int imm12;
    int address;

    int numb_arith_ops = 0;
    int cc_count = 0;                       // clock cycle counter
    int instr_fetch_latency = 1;            // holds the no. of clock cycles required for instruction fetch stage. Here it takes 1 cc for it
    int instr_decode_latency = 1;           // holds the no. of clock cycles required for instruction decode stage. Here it takes 1 cc for it
    int operand_fetch_latency = 1;    
    int execution_latency = 1;     
    int execution_passthru_latency = 1;
    int write_back_store_passthru_latency = 1;
    int write_back_latency_to_register_file = 1;   
    int value_store_latency_to_main_mem = 1;

    int i = 0;
    int j = 0; 
    printf("\n\nInitial Value in Memory and Reg File are: \n\n");

    for (i=0; i < 16; i++){
        //printf("Value in main_memory: %X \n", main_memory[i][0]);
        printf("Value in main_memory[%d]: 0x%llX\n", i, main_memory[i][0]);

    }
    printf("\n");
    for (j=0; j < 16; j++){
        //printf("Value in register_file: %X \n", register_file[i][0]);
        printf("Value in register_file[%d]: 0x%llX\n", j, register_file[j][0]);
    }

    current_stage = inst_fetch_stage;    

    // Outer loop is for iterating  instructions - add, sub, AND, ORR, LUR, STR
    while(prog_count < 8){
        
        // Inner loop: System clock cycles (It iterate five times to simulate five pipeline stage i.e., insturion feth... upto result write back)
        while(cc_count < 40){
            // Pipeline stage 0 - Instruction Fetch
            if(current_stage == inst_fetch_stage){

                printf("\nxxxxxxxxxxx Now entering stage: %s, clock cycle count: %d xxxxxxxxxxxxx\n", pipeline_stage_names[current_stage], cc_count);
                printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
                printf("Fetching instruction at memory address: %X (hex)\n", prog_count);
                //printf("\n instr_fetch_stage latency spec: %d \n",  instr_fetch_latency);

                inst_reg = main_memory[prog_count][0];                   // Fetch/load instruction from pc
                cc_count = cc_count + instr_fetch_latency;
                printf("Fetched instruction is: %llX (hex)\n", inst_reg); 
                printf("Current clock cycle count is: %d\n", cc_count);
                next_stage = current_stage + 1;                
                current_stage = next_stage;                             // Move to next stage i.e.,decode stage
                //printf("Next stage is: %d", current_stage);
                printf("Next stage: %s\n\n", pipeline_stage_names[current_stage]);

            }

            // Pipeline stage 1 - Instruction Decode
            else if (current_stage == inst_decode_stage){
                printf("\n-------Now entering stage: %s, clock cycle count: %d-------\n", pipeline_stage_names[current_stage], cc_count);
                //printf("instr_decode_stage latency spec: %d \n",  instr_decode_latency);

                // Copying value from main memory => Register
                register_file[14][0] = main_memory[14][0];               //copying the value from main_memory[14][0] into register_file[14][0]
                register_file[15][0] = main_memory[15][0];               //copying the value from main_memory[14][0] into register_file[15][0]
                printf("Value in Register file [14][0] is: %llX (hex)\n", register_file[14][0]);
                printf("Value in Register file [15][0] is: %llX (hex)\n", register_file[15][0]);

                // --------- OPCODE DETECTION ----------
                temp_opcode = (inst_reg >> 22) & 0x3FF;
                //if (temp_opcode == 0x488 || temp_opcode == 0x688) {   // use this if every instruction is 11 bit binary
                if (temp_opcode == 0x244 || temp_opcode == 0x344) {     // use this if every instruction is not 11 bit i.e., it is for 10 bit binary
                    
                    opcode = temp_opcode;                               // 10-bit opcode for ADDI or SUBI
                    // I-type decoding
                    imm12   = (inst_reg >> 10) & 0xFFF;                 // 12-bit immediate
                    field_3 = (inst_reg >> 5) & 0x1F;                   // Rn
                    field_4 = inst_reg & 0x1F;                          // Rd

                    printf("Decoded I-type instruction:\n Opcode: 0x%X\n imm12: %d\n Rn (field_3): %d\n Rd (field_4): %d\n",
                            opcode, imm12, field_3, field_4);
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
                    
                    printf("Decoded R/D-type instruction:\n Opcode: 0x%X\n Rm (field_1): %d\n shamt (field_2): %d\n Rn (field_3): %d\n Rd (field_4): %d\n Rt (field_5): %d\n Address: %d\n",
                            opcode, field_1, field_2, field_3, field_4, field_5, address);
                } 
                //printf("Decoded instruction are:\n Opcode: %d,\n Field 1: %d,\n Field 2: %d,\n Field 3: %d,\n Field 4: %d,\n imm12: %d,\n Field 5: %d,\n Address: %d\n", opcode, field_1, field_2, field_3, field_4, imm12, field_5, address);
                cc_count = cc_count + instr_decode_latency;             // Add cycles to decode instruction
                printf("Current clock cycle count is: %d\n", cc_count);

                next_stage = current_stage + 1;
                current_stage = next_stage;                             // Move to next stage i.e., operand fetch/load stage                
                printf("Next stage: %s\n\n", pipeline_stage_names[current_stage]); 
            }

            // Pipeline stage 2 - Operand Fetch
            else if(current_stage == operand_fetch_stage){
                printf("\n------- Now entering stage: %s, clock cycle count: %d -------\n", pipeline_stage_names[current_stage], cc_count);
                //printf("operand_fetch_stage latency spec: %d \n",  operand_fetch_latency);

                // Simulating operand fetch from reg_file
                // In a real case, there will be fetch from reg_file usingfield_3 and field_1 as register indices
                op_value_1 = register_file[field_1][0];                 // Fetch operand 1 from register field_1
                op_value_2 = register_file[field_3][0];                 // Fetch operand 2 from registerfield_3 register
                
                printf("Fetched operands:\n Op1: %llX\n Op2: %llX\n", op_value_1, op_value_2);
                cc_count = cc_count + operand_fetch_latency;            // Add cycles to fetch operands
                printf("Current clock cycle count is: %d\n", cc_count);

                next_stage = current_stage + 1;
                current_stage = next_stage;                             // Move to next stage i.e., execution stage                
                printf("Next stage: %s\n\n", pipeline_stage_names[current_stage]); 
                
            }

            // Pipeline stage 3 - Execution
            else if (current_stage == execution_stage){
                printf("\n-------------- Now entering execution stage, clock cycle count: %d --------------\n", cc_count);
                
                // Simulating ALU operation (simplified for now)
                // Assume that the opcode determines the operation (e.g., ADD, SUBTRACT, etc.)
                switch (opcode){
                    case 0x458:  // ADD operation
                        result = (op_value_1 + op_value_2);             // 32-bit result
                        printf("*********** Executing ALU operation Addition: %llX + %llX = %llX (hex) ***********\n", op_value_1, op_value_2, result);
                        printf("Result in decimal: %llu\n", result);
                        numb_arith_ops = numb_arith_ops + 1;
                        cc_count = cc_count+ execution_latency;
                       // printf("execution_stage latency spec: %d \n",  execution_latency);
                        printf("Current clock cycle count is: %d\n", cc_count);
                        break;

                    case 0x658:  // SUBTRACT operation
                    result = (op_value_2 - op_value_1);
                    printf("*********** Executing ALU operation Substraction: %llX - %llX = %llX (hex) ***********\n", op_value_2, op_value_1, result);
                    printf("Result in decimal: %llu\n", result);
                    numb_arith_ops = numb_arith_ops + 1;
                    cc_count = cc_count+ execution_latency;
                    //printf("execution_stage latency spec: %d \n",  execution_latency);
                    printf("Current clock cycle count is: %d\n", cc_count);
                    break;

                    case 0x450:  // AND operation
                        result = (op_value_1 & op_value_2);
                        printf("*********** Executing ALU operation AND: %llX AND %llX = %llX (hex) ***********\n", op_value_1, op_value_2, result);
                        printf("Result in decimal: %llu\n", result);
                        numb_arith_ops = numb_arith_ops + 1;
                        cc_count = cc_count+ execution_latency;
                        //printf("execution_stage latency spec: %d \n",  execution_latency);
                        printf("Current clock cycle count is: %d\n", cc_count);
                        break;
                    case 0x550:  // OOR operation
                        result = (op_value_1 | op_value_2);
                        printf("*********** Executing ALU operation OR: %llX OR %llX = %llX (hex) ***********\n", op_value_1, op_value_2, result);
                        printf("Result in decimal: %llu\n", result);
                        numb_arith_ops = numb_arith_ops + 1;
                        cc_count = cc_count+ execution_latency;
                        //printf("execution_stage latency spec: %d \n",  execution_latency);
                        printf("Current clock cycle count is: %d\n", cc_count);
                        break;

                    // case 0x488:      // ADDI for 11 bit opcode
                    case 0x244:         // ADDI for 10 bit opcode
                        result = (op_value_2 + imm12);
                        printf("*********** Executing ALU operation ANDI: %llX ADDI %llX = %llX (hex) ***********\n", op_value_2, imm12, result);
                        printf("Result in decimal: %llu\n", result);
                        numb_arith_ops = numb_arith_ops + 1;
                        cc_count = cc_count+ execution_latency;
                        // printf("execution_stage latency spec: %d \n",  execution_latency);
                        printf("Current clock cycle count is: %d\n", cc_count);
                        break;

                    // case 0x688:   // SUBI for 11 bit opcode 
                    case 0x344:     // SUBI for 10 bit opcode
                        result = (op_value_2 - imm12);
                        printf("*********** Executing ALU operation SUBI: %llX SUBI %llX = %llX (hex) ***********\n", op_value_2, imm12, result);
                        printf("Result in decimal: %llu\n", result);
                        numb_arith_ops = numb_arith_ops + 1;
                        cc_count = cc_count+ execution_latency;
                        // printf("execution_stage latency spec: %d \n",  execution_latency);
                        printf("Current clock cycle count is: %d\n", cc_count);
                        break;
                    // For Load and Store, since my reg. and main memory size is small thus I am intentionally making address = 0 and op2= 0 
                    case 0x7C2:  // Load (ld) => Load value from memory address (field_3) = 14  into register (field_5) = Reg_11                      
                        //printf("value in field_3 is: %llx\n", field_3);
                        register_file[field_5][0] = main_memory[field_3 + address][0];                    // 0xF84 00 1CB => 1111 1000 010 | 0 0000 0000  |    00    | 01 110   | 0 1011 
                        printf("*********** Executing Load: Loading value: %llX from memory address: %llX into register %llu ***********\n",                             
                        main_memory[field_3 + address][0], field_3 + address, field_5);                            
                        cc_count = cc_count + execution_passthru_latency;
                        printf("Current clock cycle count is: %d\n", cc_count);
                        break;
                    
                    case 0x7C0:  // Store (st) => Store value from register (field_3) = 15 into memory address (field_5) = 12
                        //address = 0 and  op2 = 0;     
                        main_memory[field_5 + address][0] = register_file[field_3][0];                     //0xF80001EC => 1111 1000 000 | 0 0000 0000  |    00    | 01 111   | 0 1100 
                        //printf(" Value in field_5 is : %llX\n",field_5);
                        printf("*********** Executing Store: Storing value: %llX into memory address: %llX from register %llu ***********\n", 
                        register_file[field_3][0], field_5+ address, field_3);
                        cc_count = cc_count + execution_passthru_latency;
                        //printf("Current clock cycle count is: %d\n", cc_count);
                        break; 
                    
                    default:
                        result = 0;
                        printf("Unknown ALU operation!\n");
                        printf("Result in decimal: %llu\n", result);
                        
                        break;   
                }
                next_stage = current_stage + 1;
                current_stage = next_stage;                                                  // Move to next stage i.e., Store and write-back stage                
                printf("Next stage: %s\n\n", pipeline_stage_names[current_stage]); 
            }
            
            // First storing Result into register_13 and then into main-memory_13
            else if(current_stage == wb_store_result){
                
                if(opcode == 0x7C2){
                    printf("\n-------Now entering result_store_write_back stage, clock cycle count: %d-------\n", cc_count);
                    printf("Pass-through\n");
                    cc_count = cc_count + write_back_store_passthru_latency;                  // Add cycles for write-back to register
                    printf("Current clock cycle count is: %d\n", cc_count);
                    current_stage = inst_fetch_stage;
                    next_stage = current_stage + 1;                                           // Cycle back to instruction fetch stage of next pipeline                               
                    printf("Next stage: %s\n", pipeline_stage_names[current_stage]);  
                    prog_count++;                                                             // Increment PC to fetch the next instruction

                }else if (opcode == 0x7C0) {
                    printf("\n-------Now entering result_store_write_back stage, clock cycle count: %d-------\n", cc_count);
                    printf("Pass-through\n");
                    cc_count = cc_count + write_back_store_passthru_latency;                   // Add cycles for write-back to register
                    printf("Current clock cycle count is: %d\n", cc_count);
    
                    current_stage = inst_fetch_stage;
                    next_stage = current_stage + 1;     // Cycle back to instruction fetch stage of next pipeline                               
                    printf("Next stage: %s\n", pipeline_stage_names[current_stage]);  
                    prog_count++;                                                               // Increment PC to fetch the next instruction

                }else{
            
                    printf("\n-------Now entering result_store_write_back stage, clock cycle count: %d-------\n", cc_count);
                                     
                    // Store result back to register (simplified for now) 
                    //printf(" Value in field_4 is : %llX\n",field_4);
                    register_file[field_4][0] = result;                                         // field_4 = D                    
                    printf("Storing result back to register_%d: %llX\n", field_4, register_file[field_4][0]);

                    main_memory[field_4][0] = result;                                           // field_4 = D
                    printf("Storing result back to Main memory_%d: %llX\n", field_4, main_memory[field_4][0]);
                    cc_count = cc_count + write_back_latency_to_register_file;                  // Adding cycles for write-back to register
                    printf("Current clock cycle count is: %d\n", cc_count);

                    current_stage = inst_fetch_stage;
                next_stage = current_stage + 1;                                                 // Cycle back to instruction fetch stage of next pipeline                               
                    printf("Next stage: %s\n", pipeline_stage_names[current_stage]);  
                    prog_count++;                                                               // Increment PC to fetch the next instruction
                }
            } 
        }// end of Inner loop

    }// end of Outer loop

    printf("\n--------------------------------------------------------------------\n");
    printf("--------------------------------------------------------------------\n");
    printf("Number of Arithmetic operations computed : %d \n", numb_arith_ops);
    printf("Number of system clock cycles  : %d \n", cc_count);
    printf("System clock cycles per arithmetic operation, CPO : %.2lf \n", ((float)(cc_count)/numb_arith_ops));
    
    printf("\n\nFinal Memory and Reg File Dump \n\n");
    for (i = 0; (i <= 15); i++)
    {
        //printf("Value in main_memory: %X \n", main_memory[i][0]);
        printf("Value in main_memory[%d]: 0x%llX\n", i, main_memory[i][0]);
    }

    printf("\n");

    for (j = 0; (j <= 15); j++)
    {
        //printf("Value in reg_file: %X \n", register_file[i][0]);
        printf("Value in register_file[%d]: 0x%llX\n", j, register_file[j][0]);
    }  

    printf("\nThank You!\n\n");
    return 0;
}
