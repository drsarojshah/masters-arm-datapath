#include <stdio.h>
#include <stdlib.h>

// Prototype for the synthesizable top-level function
void processor(
     int pc_in, 
     int* main_memory_out,
     int* opcode_out,
     int* op_value_1_out,
     int* op_value_2_out,     
     int *result_out   
);

int main() {    
    int pc_in = 0;
    int main_memory_out = 0; 
    int opcode_out = 0;
    int op_value_1_out = 0;
    int op_value_2_out = 0;  
    int result_out = 0;

    // Call the HLS synthesizable function Eight Times i.e., from pc_in 0 to 7
    pc_in = 0;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    
	//print out the results of this values set on the console
	printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
          pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 1;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 2;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 3;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 4;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 5;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 6;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);

    pc_in = 7;
    processor(pc_in, &main_memory_out, &opcode_out, &op_value_1_out, &op_value_2_out, &result_out);
    printf("\nInput pc = %d; Output main_memory_out = 0x%08X; Opcode_out= 0x%X; OP_value_1_out/Rm= 0x%X; OP_value_2_out/Rn= 0x%X; Result_out = 0x%08X \n",
        pc_in, main_memory_out, opcode_out, op_value_1_out, op_value_2_out, result_out);
    return 0;
}
