#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags
{
    char zero[10];
    char carry;
    char Negative;
    char psw_flag[10];
};

struct flags_forwarding
{
    char zero[10];
    char carry;
    char Negative;
    char psw_flag[10];
    int pcvalue;
};

struct stats
{
    int cycle;
};

struct instruction
{
    char instruction_string[100];
    int line_number;
    int address;
};

struct register1
{
    int value;
    char status[10];
};

struct register_file
{
    char registers[4];
    struct register1 regtr;
};

struct instruction_info
{
    int pc_value;
    char instruction_str[100];
    int source_reg1;
    int source_reg2;
    int  dest_reg1;
    int  dest_reg2;
    int target_memory_addr;
    int target_memory_data;
    int source_reg1_value;
    int source_reg2_value;
    int dest_reg1_value;
    int dest_reg2_value;
    int dest_reg_value;
    int literal_value;
    char opcode[20];
    int line_number;
};

struct stage
{
    struct instruction_info input_instruction;
    struct instruction_info output_instruction;
    char stalled[10];
};
struct data_memory
{
    int mem_address;
    int mem_value;
    char mem_char;
};
int display3 = 0;
char char_array[5];
char char_array1[5];
int reg_addr;
struct instruction ins[100];
struct register_file reg[1000];
struct register_file scoreboard[1000];
int register_count = 0;
struct data_memory data_mem[4000];
struct instruction_info ins_info[1000];
int ins_Counter = 0;
struct stats stat;
int cycles = 0;
int cycles1 = 0;
//stat.cycle = 0;
struct stage decode1;
struct stage fetch1;
struct stage execution;
struct stage mem;
struct stage wb;
struct stage exe_mul1;
struct stage exe_mul2;
struct stage exe_div1;
struct stage exe_div2;
struct stage exe_div3;
struct stage exe_div4;
struct flags status_flags;
struct flags_forwarding status_flags_fw;
int bubbles = 0;
int ins_line_number = 1;
char eof_flag[10];
int psw_counter = 0;
int mul2_flag = 1;
int div4_flag = 1;
int div4_mul_flag = 1;;
int fetch_dec = 1;
//int fetch_dec_div = 1;
int exe_stall_by_mul = 1;
/*
void register_Display()
{
 for(;register_count >= 0;--register_count)
                      {
                       printf("\n%s",reg[register_count].registers);

                      }
}*/

void writeback()
{
    FILE *fptr;
    if (strcmp(wb.stalled,"N") == 0)
    {
        strcpy(wb.input_instruction.instruction_str,mem.output_instruction.instruction_str);
        wb.input_instruction.source_reg1 = mem.output_instruction.source_reg1;
        wb.input_instruction.source_reg2 = mem.output_instruction.source_reg2;
        wb.input_instruction.literal_value = mem.output_instruction.literal_value;
        wb.input_instruction.dest_reg1 = mem.output_instruction.dest_reg1;
        wb.input_instruction.dest_reg2 = mem.output_instruction.dest_reg2;
        wb.input_instruction.dest_reg_value = mem.output_instruction.dest_reg_value;
        wb.input_instruction.pc_value = mem.output_instruction.pc_value;
        wb.input_instruction.line_number = mem.output_instruction.line_number;
        strcpy(wb.input_instruction.opcode,mem.output_instruction.opcode);
        wb.input_instruction.target_memory_data = mem.output_instruction.target_memory_data;
        strcpy(mem.output_instruction.opcode,"");
        mem.output_instruction.pc_value = 0;
        //printf("\nInstruction in Writeback is %s\n",wb.input_instruction.instruction_str);
        if (wb.input_instruction.pc_value > 0)
        {
            fptr = fopen("output.txt","a");
            fprintf(fptr,"WB        : (I%d) %s\n",wb.input_instruction.line_number,wb.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            fptr = fopen("output.txt","a");
            fprintf(fptr,"WB        : Empty %s\n",wb.input_instruction.instruction_str);
            fclose(fptr);
        }
        //printf("\n%d%d\n",wb.input_instruction.dest_reg1,wb.input_instruction.dest_reg_value);
        /*if (strcmp(status_flags_fw.psw_flag,"VALID") ==0 )
        {
        if (strcmp(decode1.stalled,"Y") == 0)
                {
                 if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    && fetch_dec == 1)
                 {
                  strcpy(decode1.stalled,"N");
                  strcpy(fetch1.stalled,"N");
                 }
                }
        }*/

        if (strcmp((wb.input_instruction.opcode),"MOVC" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
            // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");

            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }

            /*if (strcmp(decode1.input_instruction.opcode,"ADD") == 0)
            {
             if (strcmp(decode1.stalled,"Y") == 0)
             {
             if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0)
             {
                 strcpy(decode1.stalled,"N");
                 strcpy(fetch1.stalled,"N");
                 printf("\nDecode stage Stalled%s\n",decode1.stalled);
             }
             }
            }*/
        }
        if (strcmp((wb.input_instruction.opcode),"ADD" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            //printf("\nFollowing Instruction %s Added %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
            /*psw_counter = psw_counter - 1;
            if  (wb.input_instruction.dest_reg_value == 0)
            {
                strcpy(status_flags.zero,"Y");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
                // fptr = fopen("output.txt","a");
            // fprintf(fptr,"\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
            //  fclose(fptr);
            }

            if  (wb.input_instruction.dest_reg_value != 0)
            {
                strcpy(status_flags.zero,"N");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
            }


            if (psw_counter == 0)
            {
                strcpy(status_flags.psw_flag,"VALID");
                if (strcmp(decode1.stalled,"Y") == 0)
                {
                 if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    && fetch_dec == 1)
                 {
                  strcpy(decode1.stalled,"N");
                  strcpy(fetch1.stalled,"N");
                 }
                }
            }*/
            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue &&
                    (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
            }

            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue )//&&
                   // (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (strcmp(status_flags_fw.psw_flag,"INVALID") == 0)
                {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
                }
            }
        }

        if (strcmp((wb.input_instruction.opcode),"DIV" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            //printf("\nFollowing Instruction %s Added %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
            /* psw_counter = psw_counter - 1;
             if  (wb.input_instruction.dest_reg_value == 0)
             {
                 strcpy(status_flags.zero,"Y");
                 //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
                  fptr = fopen("output.txt","a");
              fprintf(fptr,"\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
              fclose(fptr);
             }

             if  (wb.input_instruction.dest_reg_value != 0)
             {
                 strcpy(status_flags.zero,"N");
                 //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
             }


             if (psw_counter == 0)
             {
                 strcpy(status_flags.psw_flag,"VALID");
                 if (strcmp(decode1.stalled,"Y") == 0)
                 {
                  if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                     && fetch_dec == 1)
                  {
                   strcpy(decode1.stalled,"N");
                   strcpy(fetch1.stalled,"N");
                  }
                 }
             } */
            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue &&
                    (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
            }

            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue )//&&
                   // (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (strcmp(status_flags_fw.psw_flag,"INVALID") == 0)
                {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
                }
            }
        }

        if (strcmp((wb.input_instruction.opcode),"AND" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            /*psw_counter = psw_counter - 1;
            if  (wb.input_instruction.dest_reg_value == 0)
            {
                strcpy(status_flags.zero,"Y");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
                 fptr = fopen("output.txt","a");
             fprintf(fptr,"\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
             fclose(fptr);
            }

            if  (wb.input_instruction.dest_reg_value != 0)
            {
                strcpy(status_flags.zero,"N");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
            }

            if (psw_counter == 0)
            {
                strcpy(status_flags.psw_flag,"VALID");
                if (strcmp(decode1.stalled,"Y") == 0)
                {
                 if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    && fetch_dec == 1)
                 {
                  strcpy(decode1.stalled,"N");
                  strcpy(fetch1.stalled,"N");
                 }
                }
            } */
        }

        if (strcmp((wb.input_instruction.opcode),"JAL" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.pc_value + 4;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            scoreboard[wb.input_instruction.dest_reg1].regtr.value = reg[wb.input_instruction.dest_reg1].regtr.value;
            strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
        }

        if (strcmp((wb.input_instruction.opcode),"OR" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            /*psw_counter = psw_counter - 1;
            if  (wb.input_instruction.dest_reg_value == 0)
            {
                strcpy(status_flags.zero,"Y");
                //printf("\nStatus Flag set zero by %s\n",wb.input_instruction.instruction_str);
                 fptr = fopen("output.txt","a");
             fprintf(fptr,"\nFetch Stalled for %s\n",fetch1.input_instruction.instruction_str);
             fclose(fptr);
            }

            if  (wb.input_instruction.dest_reg_value != 0)
            {
                strcpy(status_flags.zero,"N");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
            }


            if (psw_counter == 0)
            {
                strcpy(status_flags.psw_flag,"VALID");
                if (strcmp(decode1.stalled,"Y") == 0)
                {
                 if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    && fetch_dec == 1)
                 {
                  strcpy(decode1.stalled,"N");
                  strcpy(fetch1.stalled,"N");
                 }
                }
            }*/
        }

        if (strcmp((wb.input_instruction.opcode),"EX-OR" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            /*psw_counter = psw_counter - 1;
            if  (wb.input_instruction.dest_reg_value == 0)
            {
                strcpy(status_flags.zero,"Y");
                //printf("\nStatus Flag set zero by %s\n",wb.input_instruction.instruction_str);
                 fptr = fopen("output.txt","a");
             fprintf(fptr,"\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
             fclose(fptr);
            }

            if  (wb.input_instruction.dest_reg_value != 0)
            {
                strcpy(status_flags.zero,"N");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
            }


            if (psw_counter == 0)
            {
                strcpy(status_flags.psw_flag,"VALID");
                if (strcmp(decode1.stalled,"Y") == 0)
                {
                 if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    && fetch_dec == 1)
                 {
                  strcpy(decode1.stalled,"N");
                  strcpy(fetch1.stalled,"N");
                 }
                }
            }*/
        }

        if (strcmp((wb.input_instruction.opcode),"SUB" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            /*psw_counter = psw_counter - 1;
            if  (wb.input_instruction.dest_reg_value == 0 )
            {
                strcpy(status_flags.zero,"Y");
                //printf("\nStatus Flag set zero by %s\n",wb.input_instruction.instruction_str);
                 fptr = fopen("output.txt","a");
             fprintf(fptr,"\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
             fclose(fptr);
            }

            if  (wb.input_instruction.dest_reg_value != 0)
            {
                strcpy(status_flags.zero,"N");
                //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
            }


            if (psw_counter == 0)
            {
                strcpy(status_flags.psw_flag,"VALID");
                if (strcmp(decode1.stalled,"Y") == 0)
                {
                 if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    && fetch_dec == 1)
                 {
                  strcpy(decode1.stalled,"N");
                  strcpy(fetch1.stalled,"N");
                 }
                }
            } */
            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue &&
                    (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
            }

            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue )//&&
                   // (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (strcmp(status_flags_fw.psw_flag,"INVALID") == 0)
                {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
                }
            }
        }

        if (strcmp((wb.input_instruction.opcode),"MUL" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
            {
                scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            }
            /* psw_counter = psw_counter - 1;
             if  (wb.input_instruction.dest_reg_value == 0)
             {
                 strcpy(status_flags.zero,"Y");
                 //printf("\nStatus Flag set zero by %s\n",wb.input_instruction.instruction_str);
                  fptr = fopen("output.txt","a");
              fprintf(fptr,"\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
              fclose(fptr);
             }

             if  (wb.input_instruction.dest_reg_value != 0)
             {
                 strcpy(status_flags.zero,"N");
                 //printf("\nStatus Flag zero set by %s\n",wb.input_instruction.instruction_str);
             }


             if (psw_counter == 0)
             {
                 strcpy(status_flags.psw_flag,"VALID");
                 if (strcmp(decode1.stalled,"Y") == 0)
                 {
                  if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                     && fetch_dec == 1)
                  {
                   strcpy(decode1.stalled,"N");
                   strcpy(fetch1.stalled,"N");
                  }
                 }
             } */
            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue &&
                    (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
            }

            if (wb.input_instruction.pc_value == status_flags_fw.pcvalue )//&&
                   // (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (strcmp(status_flags_fw.psw_flag,"INVALID") == 0)
                {
                if (wb.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (wb.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
                }
            }
        }
        if (strcmp((wb.input_instruction.opcode),"LOAD" ) == 0 )
        {
            reg[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.target_memory_data;
            scoreboard[wb.input_instruction.dest_reg1].regtr.value = reg[wb.input_instruction.dest_reg1].regtr.value;
            strcpy(reg[wb.input_instruction.dest_reg1].regtr.status,"VALID");
            strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
        }

        if (strcmp(status_flags_fw.psw_flag,"VALID") ==0 )
        {
            if (strcmp(decode1.stalled,"Y") == 0)
            {
                if((strcmp(decode1.input_instruction.opcode,"BZ") == 0 || strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                        && fetch_dec == 1)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                }
            }
        }
        /*if (strcmp(execution.input_instruction.opcode,"MOVC") == 0 ||
            strcmp(execution.input_instruction.opcode,"ADD") == 0  ||
            strcmp(execution.input_instruction.opcode,"SUB") == 0  ||
           // strcmp(exe_mul1.input_instruction.opcode,"MUL") == 0  ||
            strcmp(execution.input_instruction.opcode,"LOAD") == 0 ||
            strcmp(execution.input_instruction.opcode,"AND") == 0 ||
            strcmp(execution.input_instruction.opcode,"OR") == 0 ||
         //    strcmp(exe_div1.input_instruction.opcode,"DIV") == 0 ||
            strcmp(execution.input_instruction.opcode,"EX-OR") == 0)
        {
         if (strcmp(reg[execution.input_instruction.dest_reg1].regtr.status,"VALID") == 0)
                           {
                             strcpy(reg[execution.input_instruction.dest_reg1].regtr.status,"INVALID");//for testing
                           }
        }
        if (strcmp(exe_mul1.input_instruction.opcode,"MOVC") == 0 ||
            strcmp(exe_mul1.input_instruction.opcode,"ADD") == 0  ||
            strcmp(exe_mul1.input_instruction.opcode,"SUB") == 0  ||
            strcmp(exe_mul1.input_instruction.opcode,"MUL") == 0  ||
            strcmp(exe_mul1.input_instruction.opcode,"LOAD") == 0 ||
            strcmp(exe_mul1.input_instruction.opcode,"AND") == 0 ||
            strcmp(exe_mul1.input_instruction.opcode,"OR") == 0 ||
          //  strcmp(execution.input_instruction.opcode,"DIV") == 0 ||
            strcmp(exe_mul1.input_instruction.opcode,"EX-OR") == 0)
        {
         if (strcmp(reg[exe_mul1.input_instruction.dest_reg1].regtr.status,"VALID") == 0)
                           {
                             strcpy(reg[exe_mul1.input_instruction.dest_reg1].regtr.status,"INVALID");//for testing
                           }
        }
        if (strcmp(exe_mul2.input_instruction.opcode,"MOVC") == 0 ||
            strcmp(exe_mul2.input_instruction.opcode,"ADD") == 0  ||
            strcmp(exe_mul2.input_instruction.opcode,"SUB") == 0  ||
            strcmp(exe_mul2.input_instruction.opcode,"MUL") == 0  ||
            strcmp(exe_mul2.input_instruction.opcode,"LOAD") == 0 ||
            strcmp(exe_mul2.input_instruction.opcode,"AND") == 0 ||
            strcmp(exe_mul2.input_instruction.opcode,"OR") == 0 ||
            strcmp(execution.input_instruction.opcode,"DIV") == 0 ||
            strcmp(exe_mul2.input_instruction.opcode,"EX-OR") == 0)
        {
         if (strcmp(reg[exe_mul2.input_instruction.dest_reg1].regtr.status,"VALID") == 0)
                           {
                             strcpy(reg[exe_mul2.input_instruction.dest_reg1].regtr.status,"INVALID");//for testing
                           }
        }
        if (strcmp(mem.input_instruction.opcode,"MOVC") == 0 ||
            strcmp(mem.input_instruction.opcode,"ADD") == 0  ||
            strcmp(mem.input_instruction.opcode,"SUB") == 0  ||
            strcmp(mem.input_instruction.opcode,"MUL") == 0  ||
            strcmp(mem.input_instruction.opcode,"LOAD") == 0 ||
            strcmp(mem.input_instruction.opcode,"AND") == 0 ||
            strcmp(mem.input_instruction.opcode,"OR") == 0 ||
            strcmp(mem.input_instruction.opcode,"DIV") == 0 ||
            strcmp(mem.input_instruction.opcode,"EX-OR") == 0)
        {
         if (strcmp(reg[mem.input_instruction.dest_reg1].regtr.status,"VALID") == 0)
                           {
                             strcpy(reg[mem.input_instruction.dest_reg1].regtr.status,"INVALID");//for testing
                           }
        }*/

        if (strcmp(decode1.stalled,"Y") == 0)
        {
            if(strcmp(decode1.input_instruction.opcode,"STORE") == 0)
            {
                //if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 && //commented for forwarding
                //   strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 && //commented for forwarding
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 && //added for forwarding
                        strcmp(scoreboard[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&   //added for forwarding
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    //printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"MOVC") == 0)
            {
                //if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 && //commented for forwarding
                //   strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 && //commented for forwarding
                if(strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    //printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"ADD") == 0)
            {
                //  if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //     strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"DIV") == 0)
            {
                // if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //     strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 )
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 )
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"AND") == 0)
            {
                //     if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //        strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"JUMP") == 0)
            {
                // if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"JAL") == 0)
            {
                // if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"OR") == 0)
            {
                //if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //   strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"EX-OR") == 0)
            {
                //  if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //     strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"LOAD") == 0)
            {
                //  if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"SUB") == 0)
            {
                // if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //    strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(execution.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }

            if(strcmp(decode1.input_instruction.opcode,"MUL") == 0)
            {
                // if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                //    strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0)
                if(strcmp(scoreboard[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(scoreboard[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                        strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                        strcmp(exe_mul1.stalled,"N") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                    // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                }
            }
        }
        /*int reg_count = 0;
        for(; reg_count <= 16; reg_count++)
            {
                if  (scoreboard[reg_count].registers[0] == 'R')
                {

                 if (strcmp(scoreboard[reg_count].regtr.status,"VALID") == 0)
                 {
                   if (strcmp(decode1.stalled,"N") == 0 && strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                   {
                     strcpy(scoreboard[reg_count].regtr.status,"INVALID");
                   }
                   if (strcmp(decode1.stalled,"Y") == 0)
                   {
                     if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (reg_count == decode1.input_instruction.source_reg1 ||
                           reg_count == decode1.input_instruction.source_reg2)
                    {

                    }
                    else
                    {
                        if (strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                        {
                        strcpy(scoreboard[reg_count].regtr.status,"INVALID");}
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (reg_count != decode1.input_instruction.source_reg1)
                    {
                      if (strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                        {
                        strcpy(scoreboard[reg_count].regtr.status,"INVALID");}
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (reg_count == decode1.input_instruction.source_reg1 ||
                            reg_count == decode1.input_instruction.dest_reg1)
                    {

                    }
                    else
                    {
                         if (strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                        {
                        strcpy(scoreboard[reg_count].regtr.status,"INVALID");}
                    }
                }
                   }
                 }

                }

            }*/

        //if (strcmp(wb.input_instruction.opcode,"HALT") == 0)
        //{
       //     strcpy(eof_flag,"Y");
       // }
        //if (wb.input_instruction.pc_value == ins_info[ins_Counter].pc_value)
       // {


       //     strcpy(eof_flag,"Y");
       // }
    }
    /*if (stat.cycle==35)
    {
        strcpy(eof_flag,"Y");
    }*/
}
void memory()
{
    FILE *fptr;
    if (strcmp(mem.stalled,"N") == 0)
    {
        if (strcmp((execution.output_instruction.opcode),"ADD" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"SUB" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"BZ" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"BNZ" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"LOAD" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"STORE" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"MOVC" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"AND" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"OR" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"EX-OR" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"JUMP" ) == 0 ||
                strcmp((execution.output_instruction.opcode),"JAL" ) == 0 )
        {
            strcpy(mem.input_instruction.instruction_str,execution.output_instruction.instruction_str);
            mem.input_instruction.source_reg1 = execution.output_instruction.source_reg1;
            mem.input_instruction.source_reg2 = execution.output_instruction.source_reg2;
            mem.input_instruction.literal_value = execution.output_instruction.literal_value;
            mem.input_instruction.dest_reg1 = execution.output_instruction.dest_reg1;
            mem.input_instruction.dest_reg2 = execution.output_instruction.dest_reg2;
            mem.input_instruction.target_memory_addr = execution.output_instruction.target_memory_addr;
            mem.input_instruction.pc_value = execution.output_instruction.pc_value;
            mem.input_instruction.dest_reg_value = execution.output_instruction.dest_reg_value;
            mem.input_instruction.line_number = execution.output_instruction.line_number;
            strcpy(mem.input_instruction.opcode,execution.output_instruction.opcode);
            execution.output_instruction.pc_value = 0;
            strcpy(execution.output_instruction.opcode,"");
        }
        if (strcmp((exe_mul2.output_instruction.opcode),"MUL" ) == 0)
        {
            strcpy(mem.input_instruction.instruction_str,exe_mul2.output_instruction.instruction_str);
            mem.input_instruction.source_reg1 = exe_mul2.output_instruction.source_reg1;
            mem.input_instruction.source_reg2 = exe_mul2.output_instruction.source_reg2;
            mem.input_instruction.literal_value = exe_mul2.output_instruction.literal_value;
            mem.input_instruction.dest_reg1 = exe_mul2.output_instruction.dest_reg1;
            mem.input_instruction.dest_reg2 = exe_mul2.output_instruction.dest_reg2;
            mem.input_instruction.pc_value = exe_mul2.output_instruction.pc_value;
            mem.input_instruction.target_memory_addr = exe_mul2.output_instruction.target_memory_addr;
            mem.input_instruction.dest_reg_value = exe_mul2.output_instruction.dest_reg_value;
            mem.input_instruction.line_number = exe_mul2.output_instruction.line_number;
            exe_mul2.output_instruction.pc_value = 0;
            strcpy(mem.input_instruction.opcode,exe_mul2.output_instruction.opcode);
            strcpy(exe_mul2.output_instruction.opcode,"");
        }

        if (strcmp((exe_div4.output_instruction.opcode),"DIV" ) == 0 ||
                strcmp((exe_div4.output_instruction.opcode),"HALT" ) == 0)
        {
            strcpy(mem.input_instruction.instruction_str,exe_div4.output_instruction.instruction_str);
            mem.input_instruction.source_reg1 = exe_div4.output_instruction.source_reg1;
            mem.input_instruction.source_reg2 = exe_div4.output_instruction.source_reg2;
            mem.input_instruction.literal_value = exe_div4.output_instruction.literal_value;
            mem.input_instruction.dest_reg1 = exe_div4.output_instruction.dest_reg1;
            mem.input_instruction.dest_reg2 = exe_div4.output_instruction.dest_reg2;
            mem.input_instruction.pc_value = exe_div4.output_instruction.pc_value;
            mem.input_instruction.line_number = exe_div4.output_instruction.line_number;
            mem.input_instruction.target_memory_addr = exe_div4.output_instruction.target_memory_addr;
            mem.input_instruction.dest_reg_value = exe_div4.output_instruction.dest_reg_value;
            exe_div4.output_instruction.pc_value = 0;
            strcpy(mem.input_instruction.opcode,exe_div4.output_instruction.opcode);
            strcpy(exe_div4.output_instruction.opcode,"");
        }
        //printf("\nInstruction in Memory is %s\n",mem.input_instruction.instruction_str);
        if (mem.input_instruction.pc_value > 0)
        {
            fptr = fopen("output.txt","a");
            fprintf(fptr,"MEM       : (I%d) %s\n",mem.input_instruction.line_number,mem.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            fptr = fopen("output.txt","a");
            fprintf(fptr,"MEM       : Empty%s\n",mem.input_instruction.instruction_str);
            fclose(fptr);
        }
        if (strcmp((mem.input_instruction.opcode),"MOVC" ) == 0 )
        {
            //printf("\nNo action for Movc in Mem\n");
            //printf("\n%d%d\n",mem.input_instruction.dest_reg1,mem.input_instruction.dest_reg_value);
        }
        if (strcmp((mem.input_instruction.opcode),"ADD" ) == 0 )
        {
            // printf("\nNo action for Add in Mem\n");
        }
        if (strcmp((mem.input_instruction.opcode),"STORE" ) == 0 )
        {
            data_mem[mem.input_instruction.dest_reg_value].mem_address = mem.input_instruction.dest_reg_value;
            data_mem[mem.input_instruction.dest_reg_value].mem_value = mem.input_instruction.source_reg2;
            data_mem[mem.input_instruction.dest_reg_value].mem_char = 'M';
            //printf("\nNo action for Add in Mem\n");
        }
        if (strcmp((mem.input_instruction.opcode),"LOAD" ) == 0 )
        {
            mem.input_instruction.target_memory_data = data_mem[mem.input_instruction.target_memory_addr].mem_value;
            //printf("\nNo action for Add in Mem\n");
        }

        if (strcmp((mem.input_instruction.opcode),"DIV" ) == 0 )
        {
            if (strcmp(exe_mul2.stalled,"Y") == 0)
            {
                strcpy(exe_mul2.stalled,"N");
                if (strcmp(exe_mul1.stalled,"Y") == 0)
                {
                    strcpy(exe_mul1.stalled,"N");
                }
            }
            if (exe_stall_by_mul == 2)
            {
                strcpy(execution.stalled,"N");
                strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                exe_stall_by_mul = 1;
            }
        }

        if (strcmp((mem.input_instruction.opcode),"MUL" ) == 0 )
        {
            //mem.input_instruction.target_memory_data = data_mem[mem.input_instruction.target_memory_addr].mem_value;
            //printf("\nNo action for Add in Mem\n");
            if (strcmp(execution.stalled,"Y") == 0)
            {
                strcpy(execution.stalled,"N");
                fetch_dec = 1;
                exe_stall_by_mul = 1;

                if (strcmp(decode1.stalled,"Y") == 0)
                {
                    if(strcmp(decode1.input_instruction.opcode,"STORE") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                strcmp(reg[decode1.input_instruction.dest_reg1].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    /*if(strcmp(decode1.input_instruction.opcode,"BZ") == 0 ||
                       strcmp(decode1.input_instruction.opcode,"BNZ") == 0)
                    {
                    if(psw_counter == 0 &&
                       fetch_dec == 0)
                    {
                        fetch_dec = 1;
                        strcpy(decode1.stalled,"N");
                        strcpy(fetch1.stalled,"N");
                       // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                    }
                    }*/

                    if(strcmp(decode1.input_instruction.opcode,"ADD") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    if(strcmp(decode1.input_instruction.opcode,"JUMP") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0  &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    if(strcmp(decode1.input_instruction.opcode,"HALT") == 0)
                    {
                        if(fetch_dec == 1)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            //  printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    if(strcmp(decode1.input_instruction.opcode,"AND") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            //  printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    /*if (strcmp((scoreboard[wb.input_instruction.dest_reg1].regtr.status),"INVALID") == 0)
                    {
                        scoreboard[wb.input_instruction.dest_reg1].regtr.value = wb.input_instruction.dest_reg_value;
                    //printf("\nFollowing Instruction %s Moved %d value into %s Register\n",wb.input_instruction.instruction_str,reg[wb.input_instruction.dest_reg1].regtr.value,reg[wb.input_instruction.dest_reg1].registers);
                    // printf("\n%d %d\n",reg[wb.input_instruction.dest_reg1].regtr.value,wb.input_instruction.dest_reg1);
                    strcpy(scoreboard[wb.input_instruction.dest_reg1].regtr.status,"VALID");
                    }*/ if(strcmp(decode1.input_instruction.opcode,"OR") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    if(strcmp(decode1.input_instruction.opcode,"EX-OR") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            // printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    if(strcmp(decode1.input_instruction.opcode,"LOAD") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            //  printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    if(strcmp(decode1.input_instruction.opcode,"SUB") == 0)
                    {
                        if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                                strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0 &&
                                fetch_dec == 0)
                        {
                            fetch_dec = 1;
                            strcpy(decode1.stalled,"N");
                            strcpy(fetch1.stalled,"N");
                            //  printf("\nDecode stage Stalled%s\n",decode1.stalled);
                        }
                    }

                    /*if(strcmp(decode1.input_instruction.opcode,"MUL") == 0)
                    {
                    if(strcmp(reg[decode1.input_instruction.source_reg1].regtr.status,"VALID") == 0 &&
                       strcmp(reg[decode1.input_instruction.source_reg2].regtr.status,"VALID") == 0)
                    {
                        strcpy(decode1.stalled,"N");
                        strcpy(fetch1.stalled,"N");
                        printf("\nDecode stage Stalled%s\n",decode1.stalled);
                    }
                    }*/
                }
            }
        }
    }
}
void execute ()
{
    FILE *fptr;
    int pc_value1;
    if (strcmp(execution.stalled,"Y") == 0)
    {
        //printf("\nExecution Stalled for %s\n",execution.input_instruction.instruction_str);
        if (strcmp((execution.input_instruction.opcode),"MOVC" ) == 0 ||
            strcmp((execution.input_instruction.opcode),"AND" ) == 0 ||
            strcmp((execution.input_instruction.opcode),"OR" ) == 0 ||
            strcmp((execution.input_instruction.opcode),"EX-OR" ) == 0 ||
            strcmp((execution.input_instruction.opcode),"ADD" ) == 0 ||
            strcmp((execution.input_instruction.opcode),"SUB" ) == 0)
        {
        if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
        }
        fptr = fopen("output.txt","a");
        fprintf(fptr,"INTFU     : (I%d) %s Stalled\n",execution.input_instruction.line_number,execution.input_instruction.instruction_str);
        fclose(fptr);
    }
    if (strcmp(execution.stalled,"N") == 0)
    {
        if (strcmp((decode1.output_instruction.opcode),"ADD" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"SUB" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"BZ" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"BNZ" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"LOAD" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"STORE" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"MOVC" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"AND" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"OR" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"EX-OR" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"JUMP" ) == 0 ||
                // strcmp((decode1.output_instruction.opcode),"HALT" ) == 0 ||
                strcmp((decode1.output_instruction.opcode),"JAL" ) == 0)
        {
            strcpy(execution.input_instruction.instruction_str,decode1.output_instruction.instruction_str);
            execution.input_instruction.source_reg1 = decode1.output_instruction.source_reg1;
            execution.input_instruction.source_reg2 = decode1.output_instruction.source_reg2;
            execution.input_instruction.literal_value = decode1.output_instruction.literal_value;
            execution.input_instruction.dest_reg1 = decode1.output_instruction.dest_reg1;
            execution.input_instruction.dest_reg2 = decode1.output_instruction.dest_reg2;
            execution.input_instruction.dest_reg_value = decode1.output_instruction.dest_reg_value;
            execution.input_instruction.pc_value = decode1.output_instruction.pc_value;
            execution.input_instruction.line_number = decode1.output_instruction.line_number;
            strcpy(execution.input_instruction.opcode,decode1.output_instruction.opcode);
            strcpy(decode1.output_instruction.opcode,"");
            strcpy(decode1.output_instruction.instruction_str,"");
            decode1.output_instruction.pc_value = 0;
            //printf("\nInstruction in Execute is %s\n",execution.input_instruction.instruction_str);
            fptr = fopen("output.txt","a");
            fprintf(fptr,"INTFU     : (I%d) %s\n",execution.input_instruction.line_number,execution.input_instruction.instruction_str);
            fclose(fptr);
            // k += sprintf(buffer+k, "\nInstruction in Execute is %s\n", execution.input_instruction.instruction_str);
            //printf("\nChecking if null %s\n",decode1.output_instruction.opcode);
            if (strcmp((execution.input_instruction.opcode),"BZ" ) == 0)
            {
                if (strcmp((/*status_flags*/status_flags_fw.zero),"Y" ) == 0)
                {
                    pc_value1 = execution.input_instruction.pc_value + (execution.input_instruction.literal_value);
                    int i = ins_Counter;
                    execution.input_instruction.pc_value = pc_value1;
                    for (i; i > 0; i--)
                    {
                        if (ins[i].address == pc_value1)
                        {
                            ins_line_number = ins[i].line_number - 1;
                            break;
                        }
                    }
                    if (strcmp(decode1.stalled,"Y") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                }
                    strcpy(fetch1.output_instruction.instruction_str,"");
                    strcpy(fetch1.input_instruction.instruction_str,"");
                    strcpy(decode1.output_instruction.instruction_str,"");
                    strcpy(decode1.input_instruction.instruction_str,"");
                    decode1.output_instruction.source_reg1 = 0;
                    decode1.input_instruction.source_reg1 = 0;
                    decode1.output_instruction.source_reg2 = 0;
                    decode1.input_instruction.source_reg2 = 0;
                    decode1.output_instruction.literal_value = 0;
                    decode1.input_instruction.literal_value = 0;
                    decode1.output_instruction.dest_reg1 = 0;
                    decode1.input_instruction.dest_reg1 = 0;
                    decode1.output_instruction.dest_reg2 = 0;
                    decode1.input_instruction.dest_reg2 = 0;
                    decode1.output_instruction.dest_reg_value = 0;
                    decode1.input_instruction.dest_reg_value = 0;
                    strcpy(decode1.output_instruction.opcode,"");
                    strcpy(decode1.input_instruction.opcode,"");
                    strcpy(/*status_flags*/status_flags_fw.zero,"N");
                    decode1.input_instruction.line_number = 0;
                    decode1.output_instruction.line_number = 0;
                    decode1.input_instruction.pc_value = 0;
                    decode1.output_instruction.pc_value = 0;
                    fetch1.input_instruction.line_number = 0;
                    fetch1.output_instruction.line_number = 0;
                    fetch1.input_instruction.pc_value = 0;
                    fetch1.output_instruction.pc_value = 0;

                }



            }

            if (strcmp((execution.input_instruction.opcode),"BNZ" ) == 0)
            {
                if (strcmp((/*status_flags*/status_flags_fw.zero),"N" ) == 0)
                {
                    pc_value1 = execution.input_instruction.pc_value + (execution.input_instruction.literal_value);
                    int i = ins_Counter;
                    execution.input_instruction.pc_value = pc_value1;
                    for (i; i > 0; i--)
                    {
                        if (ins[i].address == pc_value1)
                        {
                            ins_line_number = ins[i].line_number - 1;
                            break;
                        }
                    }

                    if (strcmp(decode1.stalled,"Y") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                }

                    strcpy(fetch1.output_instruction.instruction_str,"");
                    strcpy(fetch1.input_instruction.instruction_str,"");
                    //memset(fetch1.input_instruction.instruction_str, 0, sizeof(fetch1.input_instruction.instruction_str));
                    strcpy(decode1.output_instruction.instruction_str,"");
                    strcpy(decode1.input_instruction.instruction_str,"");
                    decode1.output_instruction.source_reg1 = 0;
                    decode1.input_instruction.source_reg1 = 0;
                    decode1.output_instruction.source_reg2 = 0;
                    decode1.input_instruction.source_reg2 = 0;
                    decode1.output_instruction.literal_value = 0;
                    decode1.input_instruction.literal_value = 0;
                    decode1.output_instruction.dest_reg1 = 0;
                    decode1.input_instruction.dest_reg1 = 0;
                    decode1.output_instruction.dest_reg2 = 0;
                    decode1.input_instruction.dest_reg2 = 0;
                    decode1.output_instruction.dest_reg_value = 0;
                    decode1.input_instruction.dest_reg_value = 0;
                    strcpy(decode1.output_instruction.opcode,"");
                    strcpy(decode1.input_instruction.opcode,"");
                    decode1.input_instruction.line_number = 0;
                    decode1.output_instruction.line_number = 0;
                    decode1.input_instruction.pc_value = 0;
                    decode1.output_instruction.pc_value = 0;
                    fetch1.input_instruction.line_number = 0;
                    fetch1.output_instruction.line_number = 0;
                    fetch1.input_instruction.pc_value = 0;
                    fetch1.output_instruction.pc_value = 0;
                }

            }

            if (strcmp((execution.input_instruction.opcode),"JAL" ) == 0)
            {

                //pc_value1 = reg[execution.input_instruction.source_reg1].regtr.value   + (execution.input_instruction.literal_value);
                pc_value1 = scoreboard[execution.input_instruction.source_reg1].regtr.value   + (execution.input_instruction.literal_value);
                int i = ins_Counter;
                for (i; i > 0; i--)
                {
                    if (ins[i].address == pc_value1)
                    {
                        ins_line_number = ins[i].line_number - 1;
                        break;
                    }
                }
                if (strcmp(decode1.stalled,"Y") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                }
                strcpy(fetch1.output_instruction.instruction_str,"");
                strcpy(fetch1.input_instruction.instruction_str,"");
                //memset(fetch1.input_instruction.instruction_str, 0, sizeof(fetch1.input_instruction.instruction_str));
                strcpy(decode1.output_instruction.instruction_str,"");
                strcpy(decode1.input_instruction.instruction_str,"");
                decode1.output_instruction.source_reg1 = 0;
                decode1.input_instruction.source_reg1 = 0;
                decode1.output_instruction.source_reg2 = 0;
                decode1.input_instruction.source_reg2 = 0;
                decode1.output_instruction.literal_value = 0;
                decode1.input_instruction.literal_value = 0;
                decode1.output_instruction.dest_reg1 = 0;
                decode1.input_instruction.dest_reg1 = 0;
                decode1.output_instruction.dest_reg2 = 0;
                decode1.input_instruction.dest_reg2 = 0;
                decode1.output_instruction.dest_reg_value = 0;
                decode1.input_instruction.dest_reg_value = 0;
                strcpy(decode1.output_instruction.opcode,"");
                strcpy(decode1.input_instruction.opcode,"");
                decode1.input_instruction.line_number = 0;
                    decode1.output_instruction.line_number = 0;
                    decode1.input_instruction.pc_value = 0;
                    decode1.output_instruction.pc_value = 0;
                    fetch1.input_instruction.line_number = 0;
                    fetch1.output_instruction.line_number = 0;
                    fetch1.input_instruction.pc_value = 0;
                    fetch1.output_instruction.pc_value = 0;


            }

            if (strcmp((execution.input_instruction.opcode),"JUMP" ) == 0)
            {

                //pc_value1 = reg[execution.input_instruction.source_reg1].regtr.value   + (execution.input_instruction.literal_value);
                pc_value1 = scoreboard[execution.input_instruction.source_reg1].regtr.value   + (execution.input_instruction.literal_value);
                int i = ins_Counter;
                for (i; i > 0; i--)
                {
                    if (ins[i].address == pc_value1)
                    {
                        ins_line_number = ins[i].line_number - 1;
                        break;
                    }
                }

                if (strcmp(decode1.stalled,"Y") == 0)
                {
                    strcpy(decode1.stalled,"N");
                    strcpy(fetch1.stalled,"N");
                }
                strcpy(fetch1.output_instruction.instruction_str,"");
                strcpy(fetch1.input_instruction.instruction_str,"");
                //memset(fetch1.input_instruction.instruction_str, 0, sizeof(fetch1.input_instruction.instruction_str));
                strcpy(decode1.output_instruction.instruction_str,"");
                strcpy(decode1.input_instruction.instruction_str,"");
                decode1.output_instruction.source_reg1 = 0;
                decode1.input_instruction.source_reg1 = 0;
                decode1.output_instruction.source_reg2 = 0;
                decode1.input_instruction.source_reg2 = 0;
                decode1.output_instruction.literal_value = 0;
                decode1.input_instruction.literal_value = 0;
                decode1.output_instruction.dest_reg1 = 0;
                decode1.input_instruction.dest_reg1 = 0;
                decode1.output_instruction.dest_reg2 = 0;
                decode1.input_instruction.dest_reg2 = 0;
                decode1.output_instruction.dest_reg_value = 0;
                decode1.input_instruction.dest_reg_value = 0;
                strcpy(decode1.output_instruction.opcode,"");
                strcpy(decode1.input_instruction.opcode,"");
                decode1.input_instruction.line_number = 0;
                    decode1.output_instruction.line_number = 0;
                    decode1.input_instruction.pc_value = 0;
                    decode1.output_instruction.pc_value = 0;
                    fetch1.input_instruction.line_number = 0;
                    fetch1.output_instruction.line_number = 0;
                    fetch1.input_instruction.pc_value = 0;
                    fetch1.output_instruction.pc_value = 0;


            }

            if (strcmp((execution.input_instruction.opcode),"HALT" ) == 0)
            {


                /*ins_line_number = ins_Counter;
                strcpy(fetch1.output_instruction.instruction_str,"");
                strcpy(fetch1.input_instruction.instruction_str,"");
                //memset(fetch1.input_instruction.instruction_str, 0, sizeof(fetch1.input_instruction.instruction_str));
                strcpy(decode1.output_instruction.instruction_str,"");
                strcpy(decode1.input_instruction.instruction_str,"");
                decode1.output_instruction.source_reg1 = 0;
                decode1.input_instruction.source_reg1 = 0;
                decode1.output_instruction.source_reg2 = 0;
                decode1.input_instruction.source_reg2 = 0;
                decode1.output_instruction.literal_value = 0;
                decode1.input_instruction.literal_value = 0;
                decode1.output_instruction.dest_reg1 = 0;
                decode1.input_instruction.dest_reg1 = 0;
                decode1.output_instruction.dest_reg2 = 0;
                decode1.input_instruction.dest_reg2 = 0;
                decode1.output_instruction.dest_reg_value = 0;
                decode1.input_instruction.dest_reg_value = 0;
                strcpy(decode1.output_instruction.opcode,"");
                strcpy(decode1.input_instruction.opcode,"");*/


            }


            if (strcmp((execution.input_instruction.opcode),"LOAD" ) == 0 )
            {
                //execution.input_instruction.target_memory_addr = reg[execution.input_instruction.source_reg1].regtr.value +
                //                                             execution.input_instruction.literal_value;
                execution.input_instruction.target_memory_addr = scoreboard[execution.input_instruction.source_reg1].regtr.value +
                        execution.input_instruction.literal_value;
                // printf("\nLiteral Value for Store %d\n",execution.input_instruction.literal_value);
                //break;
            }
            if (strcmp((execution.input_instruction.opcode),"MOVC" ) == 0 )
            {
                // printf("\nDestination Register is %d %s \n",execution.input_instruction.dest_reg1,reg[execution.input_instruction.dest_reg1].registers);
                // printf("\nLiteral Value in Execution Stage is %d\n",execution.input_instruction.literal_value);
                execution.input_instruction.dest_reg_value = execution.input_instruction.literal_value;
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                // printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
            }
            if (strcmp((execution.input_instruction.opcode),"AND" ) == 0 )
            {

                // printf("Destination Register is %d %s \n",execution.input_instruction.dest_reg1,reg[execution.input_instruction.dest_reg1].registers);
                // printf("\nSource 1 register value is %d\n",reg[execution.input_instruction.source_reg1].regtr.value);
                // printf("\nSource 2 register value is %d\n",reg[execution.input_instruction.source_reg2].regtr.value);
                // execution.input_instruction.dest_reg_value = reg[execution.input_instruction.source_reg1].regtr.value &
                //                                              reg[execution.input_instruction.source_reg2].regtr.value;
                execution.input_instruction.dest_reg_value = scoreboard[execution.input_instruction.source_reg1].regtr.value &
                        scoreboard[execution.input_instruction.source_reg2].regtr.value;
                //scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                //strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                // printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
            }
            if (strcmp((execution.input_instruction.opcode),"OR" ) == 0 )
            {

                // printf("Destination Register is %d %s \n",execution.input_instruction.dest_reg1,reg[execution.input_instruction.dest_reg1].registers);
                // printf("\nSource 1 register value is %d\n",reg[execution.input_instruction.source_reg1].regtr.value);
                // printf("\nSource 2 register value is %d\n",reg[execution.input_instruction.source_reg2].regtr.value);
                // execution.input_instruction.dest_reg_value = reg[execution.input_instruction.source_reg1].regtr.value |
                //                                               reg[execution.input_instruction.source_reg2].regtr.value;
                execution.input_instruction.dest_reg_value = scoreboard[execution.input_instruction.source_reg1].regtr.value |
                        scoreboard[execution.input_instruction.source_reg2].regtr.value;
                //scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                //strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                // printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
            }
            if (strcmp((execution.input_instruction.opcode),"EX-OR" ) == 0 )
            {

                // printf("Destination Register is %d %s \n",execution.input_instruction.dest_reg1,reg[execution.input_instruction.dest_reg1].registers);
                // printf("\nSource 1 register value is %d\n",reg[execution.input_instruction.source_reg1].regtr.value);
                // printf("\nSource 2 register value is %d\n",reg[execution.input_instruction.source_reg2].regtr.value);
                //execution.input_instruction.dest_reg_value = reg[execution.input_instruction.source_reg1].regtr.value ^
                //                                              reg[execution.input_instruction.source_reg2].regtr.value;
                execution.input_instruction.dest_reg_value = scoreboard[execution.input_instruction.source_reg1].regtr.value ^
                        scoreboard[execution.input_instruction.source_reg2].regtr.value;
                //scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                //strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                //printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
            }
            if (strcmp((execution.input_instruction.opcode),"ADD" ) == 0 )
            {

                // printf("Destination Register is %d %s \n",execution.input_instruction.dest_reg1,reg[execution.input_instruction.dest_reg1].registers);
                // printf("\nSource 1 register value is %d\n",reg[execution.input_instruction.source_reg1].regtr.value);
                // printf("\nSource 2 register value is %d\n",reg[execution.input_instruction.source_reg2].regtr.value);
                // execution.input_instruction.dest_reg_value = reg[execution.input_instruction.source_reg1].regtr.value +
                //                                              reg[execution.input_instruction.source_reg2].regtr.value;
                execution.input_instruction.dest_reg_value = scoreboard[execution.input_instruction.source_reg1].regtr.value +
                        scoreboard[execution.input_instruction.source_reg2].regtr.value;
                //scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                // strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (execution.input_instruction.pc_value == status_flags_fw.pcvalue &&
                        (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
                {
                    if (execution.input_instruction.dest_reg_value == 0)
                    {
                        strcpy(status_flags_fw.zero,"Y");
                        strcpy(status_flags_fw.psw_flag,"VALID");
                    }

                    if (execution.input_instruction.dest_reg_value != 0)
                    {
                        strcpy(status_flags_fw.zero,"N");
                        strcpy(status_flags_fw.psw_flag,"VALID");
                    }
                }
                // printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
            }
            if (strcmp((execution.input_instruction.opcode),"SUB" ) == 0 )
            {

                //  printf("\nSource 1 register value is %d\n",reg[execution.input_instruction.source_reg1].regtr.value);
                //  printf("\nSource 2 register value is %d\n",reg[execution.input_instruction.source_reg2].regtr.value);
                //  execution.input_instruction.dest_reg_value = reg[execution.input_instruction.source_reg1].regtr.value -
                //                                               reg[execution.input_instruction.source_reg2].regtr.value;
                execution.input_instruction.dest_reg_value = scoreboard[execution.input_instruction.source_reg1].regtr.value -
                        scoreboard[execution.input_instruction.source_reg2].regtr.value;
                //scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                // strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                // printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
                if (execution.input_instruction.pc_value == status_flags_fw.pcvalue &&
                        (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
                {
                    if (execution.input_instruction.dest_reg_value == 0)
                    {
                        strcpy(status_flags_fw.zero,"Y");
                        strcpy(status_flags_fw.psw_flag,"VALID");
                    }

                    if (execution.input_instruction.dest_reg_value != 0)
                    {
                        strcpy(status_flags_fw.zero,"N");
                        strcpy(status_flags_fw.psw_flag,"VALID");
                    }
                }
            }

            if (strcmp((execution.input_instruction.opcode),"STORE" ) == 0 )
            {
                //  printf("\nSource register value for Store is %d\n",reg[execution.input_instruction.source_reg1].regtr.value);
                //  printf("\nDestination register value for store is %d\n",reg[execution.input_instruction.dest_reg1].regtr.value);
                // execution.input_instruction.dest_reg_value = reg[execution.input_instruction.dest_reg1].regtr.value +
                //                                               execution.input_instruction.literal_value;
                //  execution.input_instruction.source_reg2 = reg[execution.input_instruction.source_reg1].regtr.value;
                execution.input_instruction.dest_reg_value = scoreboard[execution.input_instruction.dest_reg1].regtr.value +
                        execution.input_instruction.literal_value;
                execution.input_instruction.source_reg2 = scoreboard[execution.input_instruction.source_reg1].regtr.value;
                //printf("\nDestination Register value Calculated is %d\n",execution.input_instruction.dest_reg_value);
                //break;
            }


        }
        else
        {
            if (strcmp(execution.stalled,"Y") != 0)
            {
                //printf("\nInstruction in Execute is \n");
                fptr = fopen("output.txt","a");
                fprintf(fptr,"INTFU     : Empty\n");
                fclose(fptr);
            }
        }


    }

}


void execute_mul1()
{
    FILE *fptr;
    if (strcmp(exe_mul1.stalled,"Y") == 0)
    {
        //printf("\nExecution Stalled for %s\n",execution.input_instruction.instruction_str);
        fptr = fopen("output.txt","a");
        fprintf(fptr,"MUL1      : (I%d) %s Stalled\n",exe_mul1.input_instruction.line_number,exe_mul1.input_instruction.instruction_str);
        fclose(fptr);
    }
    if (strcmp(exe_mul1.stalled,"N") == 0)
    {

        if (strcmp(decode1.output_instruction.opcode,"MUL") == 0)
        {
            strcpy(exe_mul1.input_instruction.instruction_str,decode1.output_instruction.instruction_str);
            exe_mul1.input_instruction.source_reg1 = decode1.output_instruction.source_reg1;
            exe_mul1.input_instruction.source_reg2 = decode1.output_instruction.source_reg2;
            exe_mul1.input_instruction.literal_value = decode1.output_instruction.literal_value;
            exe_mul1.input_instruction.dest_reg1 = decode1.output_instruction.dest_reg1;
            exe_mul1.input_instruction.dest_reg2 = decode1.output_instruction.dest_reg2;
            exe_mul1.input_instruction.target_memory_addr = decode1.output_instruction.target_memory_addr;
            exe_mul1.input_instruction.dest_reg_value = decode1.output_instruction.dest_reg_value;
            exe_mul1.input_instruction.pc_value = decode1.output_instruction.pc_value;
            exe_mul1.input_instruction.line_number = decode1.output_instruction.line_number;
            strcpy(exe_mul1.input_instruction.opcode,decode1.output_instruction.opcode);
            strcpy(decode1.output_instruction.opcode,"");
            decode1.output_instruction.pc_value = 0;
            //strcpy(decode1.output_instruction.instruction_str,"");
            //printf("\nInstruction in Execute Mul 1 is %s\n",exe_mul1.input_instruction.instruction_str);
            fptr = fopen("output.txt","a");
            fprintf(fptr,"MUL1      : (I%d) %s\n",exe_mul1.input_instruction.line_number,exe_mul1.input_instruction.instruction_str);
            fclose(fptr);
            /*if (strcmp((exe_mul1.input_instruction.opcode),"MUL" ) == 0 )
            {

               // printf("\nSource 1 register value is %d\n",reg[exe_mul1.input_instruction.source_reg1].regtr.value);
               // printf("\nSource 2 register value is %d\n",reg[exe_mul1.input_instruction.source_reg2].regtr.value);
                exe_mul1.input_instruction.dest_reg_value = reg[exe_mul1.input_instruction.source_reg1].regtr.value *
                                                             reg[exe_mul1.input_instruction.source_reg2].regtr.value;
               // printf("\nDestination Register value Calculated is %d\n",exe_mul1.input_instruction.dest_reg_value);
                //break;
            }*/
        }
        else
        {
            //printf("\nInstruction in Execute Mul 1 is\n");
            if (strcmp(exe_mul1.stalled,"Y") != 0)
            {
                fptr = fopen("output.txt","a");
                fprintf(fptr,"MUL1      : Empty\n");
                fclose(fptr);
            }
        }
    }
}

void execute_mul2()
{
    FILE *fptr;
    if (strcmp(exe_mul2.stalled,"Y") == 0)
    {
        //printf("\nExecution Stalled for %s\n",execution.input_instruction.instruction_str);
        if (strcmp(exe_mul1.output_instruction.opcode,"MUL") == 0)
        {
        if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (execution.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            execution.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                        strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
        }
        fptr = fopen("output.txt","a");
        fprintf(fptr,"MUL2      : (I%d) %s Stalled\n",exe_mul2.input_instruction.line_number,exe_mul2.input_instruction.instruction_str);
        fclose(fptr);
    }
    if (strcmp(exe_mul2.stalled,"N") == 0)
    {

        if (strcmp(exe_mul1.output_instruction.opcode,"MUL") == 0)
        {
            mul2_flag = 0;
            strcpy(exe_mul2.input_instruction.instruction_str,exe_mul1.output_instruction.instruction_str);
            exe_mul2.input_instruction.source_reg1 = exe_mul1.output_instruction.source_reg1;
            exe_mul2.input_instruction.source_reg2 = exe_mul1.output_instruction.source_reg2;
            exe_mul2.input_instruction.literal_value = exe_mul1.output_instruction.literal_value;
            exe_mul2.input_instruction.dest_reg1 = exe_mul1.output_instruction.dest_reg1;
            exe_mul2.input_instruction.dest_reg2 = exe_mul1.output_instruction.dest_reg2;
            exe_mul2.input_instruction.target_memory_addr = exe_mul1.output_instruction.target_memory_addr;
            exe_mul2.input_instruction.dest_reg_value = exe_mul1.output_instruction.dest_reg_value;
            exe_mul2.input_instruction.pc_value = exe_mul1.output_instruction.pc_value;
            exe_mul2.input_instruction.line_number = exe_mul1.output_instruction.line_number;
            strcpy(exe_mul2.input_instruction.opcode,exe_mul1.output_instruction.opcode);
            strcpy(exe_mul1.output_instruction.opcode,"");
            exe_mul1.output_instruction.pc_value = 0;
            // scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.value = exe_mul2.input_instruction.dest_reg_value;
            // strcpy(scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.status,"VALID");
            //strcpy(exe_mul1.output_instruction.instruction_str,"");
            //printf("\nInstruction in Execute Mul 2 is %s\n",exe_mul2.input_instruction.instruction_str);
            if (strcmp((exe_mul2.input_instruction.opcode),"MUL" ) == 0 )
            {

                // printf("\nSource 1 register value is %d\n",reg[exe_mul1.input_instruction.source_reg1].regtr.value);
                // printf("\nSource 2 register value is %d\n",reg[exe_mul1.input_instruction.source_reg2].regtr.value);
                // exe_mul2.input_instruction.dest_reg_value = reg[exe_mul2.input_instruction.source_reg1].regtr.value *
                //                                               reg[exe_mul2.input_instruction.source_reg2].regtr.value;
                exe_mul2.input_instruction.dest_reg_value = scoreboard[exe_mul2.input_instruction.source_reg1].regtr.value *
                        scoreboard[exe_mul2.input_instruction.source_reg2].regtr.value;
                //scoreboard[execution.input_instruction.dest_reg1].regtr.value = execution.input_instruction.dest_reg_value;
                //strcpy(scoreboard[execution.input_instruction.dest_reg1].regtr.status,"VALID");
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (exe_mul2.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            exe_mul2.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.value = exe_mul2.input_instruction.dest_reg_value;
                        strcpy(scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (exe_mul2.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.value = exe_mul2.input_instruction.dest_reg_value;
                        strcpy(scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (exe_mul2.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            exe_mul2.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.value = exe_mul2.input_instruction.dest_reg_value;
                        strcpy(scoreboard[exe_mul2.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                // printf("\nDestination Register value Calculated is %d\n",exe_mul1.input_instruction.dest_reg_value);
                //break;
            }
            if (exe_mul2.input_instruction.pc_value == status_flags_fw.pcvalue &&
                    (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (exe_mul2.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (exe_mul2.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
            }
            fptr = fopen("output.txt","a");
            fprintf(fptr,"MUL2      : (I%d) %s\n",exe_mul2.input_instruction.line_number,exe_mul2.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            //printf("\nInstruction in Execute Mul 2 is\n");
            if (strcmp(exe_mul2.stalled,"Y") != 0)
            {
                fptr = fopen("output.txt","a");
                fprintf(fptr,"MUL2      : Empty\n");
                fclose(fptr);
            }
        }
    }
}

void execute_div1()
{
    FILE *fptr;
    if (strcmp(exe_div1.stalled,"N") == 0)
    {

        if (strcmp(decode1.output_instruction.opcode,"DIV") == 0 ||
                strcmp((decode1.output_instruction.opcode),"HALT" ) == 0)
        {
            // mul2_flag = 0;
            strcpy(exe_div1.input_instruction.instruction_str,decode1.output_instruction.instruction_str);
            exe_div1.input_instruction.source_reg1 = decode1.output_instruction.source_reg1;
            exe_div1.input_instruction.source_reg2 = decode1.output_instruction.source_reg2;
            exe_div1.input_instruction.literal_value = decode1.output_instruction.literal_value;
            exe_div1.input_instruction.dest_reg1 = decode1.output_instruction.dest_reg1;
            exe_div1.input_instruction.dest_reg2 = decode1.output_instruction.dest_reg2;
            exe_div1.input_instruction.target_memory_addr = decode1.output_instruction.target_memory_addr;
            exe_div1.input_instruction.dest_reg_value = decode1.output_instruction.dest_reg_value;
            exe_div1.input_instruction.pc_value = decode1.output_instruction.pc_value;
            exe_div1.input_instruction.line_number = decode1.output_instruction.line_number;
            strcpy(exe_div1.input_instruction.opcode,decode1.output_instruction.opcode);
            strcpy(decode1.output_instruction.opcode,"");
            decode1.output_instruction.pc_value = 0;
            //strcpy(exe_mul1.output_instruction.instruction_str,"");
            //printf("\nInstruction in Execute Mul 2 is %s\n",exe_mul2.input_instruction.instruction_str);
            if (strcmp((exe_div1.input_instruction.opcode),"DIV" ) == 0 )
            {

                //  printf("\nSource 1 register value is %d\n",reg[exe_div1.input_instruction.source_reg1].regtr.value);
                //  printf("\nSource 2 register value is %d\n",reg[exe_div1.input_instruction.source_reg2].regtr.value);
                exe_div1.input_instruction.dest_reg_value = reg[exe_div1.input_instruction.source_reg1].regtr.value /
                        reg[exe_div1.input_instruction.source_reg2].regtr.value;
                // printf("\nDestination Register value Calculated is %d\n",exe_mul1.input_instruction.dest_reg_value);
                //break;
            }
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV1      : (I%d) %s\n",exe_div1.input_instruction.line_number,exe_div1.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            //printf("\nInstruction in Execute Mul 2 is\n");
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV1      : Empty\n");
            fclose(fptr);
        }
    }
}

void execute_div2()
{
    FILE *fptr;
    if (strcmp(exe_div2.stalled,"N") == 0)
    {

        if (strcmp(exe_div1.output_instruction.opcode,"DIV") == 0 ||
                strcmp((exe_div1.output_instruction.opcode),"HALT" ) == 0)
        {
            // mul2_flag = 0;
            strcpy(exe_div2.input_instruction.instruction_str,exe_div1.output_instruction.instruction_str);
            exe_div2.input_instruction.source_reg1 = exe_div1.output_instruction.source_reg1;
            exe_div2.input_instruction.source_reg2 = exe_div1.output_instruction.source_reg2;
            exe_div2.input_instruction.literal_value = exe_div1.output_instruction.literal_value;
            exe_div2.input_instruction.dest_reg1 = exe_div1.output_instruction.dest_reg1;
            exe_div2.input_instruction.dest_reg2 = exe_div1.output_instruction.dest_reg2;
            exe_div2.input_instruction.target_memory_addr = exe_div1.output_instruction.target_memory_addr;
            exe_div2.input_instruction.dest_reg_value = exe_div1.output_instruction.dest_reg_value;
            exe_div2.input_instruction.pc_value = exe_div1.output_instruction.pc_value;
            exe_div2.input_instruction.line_number = exe_div1.output_instruction.line_number;
            strcpy(exe_div2.input_instruction.opcode,exe_div1.output_instruction.opcode);
            strcpy(exe_div1.output_instruction.opcode,"");
            exe_div1.output_instruction.pc_value = 0;
            //strcpy(exe_mul1.output_instruction.instruction_str,"");
            //printf("\nInstruction in Execute Mul 2 is %s\n",exe_mul2.input_instruction.instruction_str);
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV2      : (I%d) %s\n",exe_div2.input_instruction.line_number,exe_div2.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            //printf("\nInstruction in Execute Mul 2 is\n");
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV2      : Empty\n");
            fclose(fptr);
        }
    }
}

void execute_div3()
{
    FILE *fptr;
    if (strcmp(exe_div3.stalled,"N") == 0)
    {

        if (strcmp(exe_div2.output_instruction.opcode,"DIV") == 0 ||
                strcmp((exe_div2.output_instruction.opcode),"HALT" ) == 0)
        {
            // mul2_flag = 0;
            strcpy(exe_div3.input_instruction.instruction_str,exe_div2.output_instruction.instruction_str);
            exe_div3.input_instruction.source_reg1 = exe_div2.output_instruction.source_reg1;
            exe_div3.input_instruction.source_reg2 = exe_div2.output_instruction.source_reg2;
            exe_div3.input_instruction.literal_value = exe_div2.output_instruction.literal_value;
            exe_div3.input_instruction.dest_reg1 = exe_div2.output_instruction.dest_reg1;
            exe_div3.input_instruction.dest_reg2 = exe_div2.output_instruction.dest_reg2;
            exe_div3.input_instruction.target_memory_addr = exe_div2.output_instruction.target_memory_addr;
            exe_div3.input_instruction.dest_reg_value = exe_div2.output_instruction.dest_reg_value;
            exe_div3.input_instruction.line_number = exe_div2.output_instruction.line_number;
            exe_div3.input_instruction.pc_value = exe_div2.output_instruction.pc_value;
            strcpy(exe_div3.input_instruction.opcode,exe_div2.output_instruction.opcode);
            strcpy(exe_div2.output_instruction.opcode,"");
            exe_div2.output_instruction.pc_value = 0;
            //strcpy(exe_mul1.output_instruction.instruction_str,"");
            //printf("\nInstruction in Execute Mul 2 is %s\n",exe_mul2.input_instruction.instruction_str);
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV3      : (I%d) %s\n",exe_div3.input_instruction.line_number,exe_div3.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            //printf("\nInstruction in Execute Mul 2 is\n");
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV3      : Empty\n");
            fclose(fptr);
        }
    }
}

void execute_div4()
{
    FILE *fptr;
    if (strcmp(exe_div4.stalled,"N") == 0)
    {

        if (strcmp(exe_div3.output_instruction.opcode,"DIV") == 0 ||
                strcmp((exe_div3.output_instruction.opcode),"HALT" ) == 0)
        {
            // mul2_flag = 0;
            div4_flag = 0;
            strcpy(exe_div4.input_instruction.instruction_str,exe_div3.output_instruction.instruction_str);
            exe_div4.input_instruction.source_reg1 = exe_div3.output_instruction.source_reg1;
            exe_div4.input_instruction.source_reg2 = exe_div3.output_instruction.source_reg2;
            exe_div4.input_instruction.literal_value = exe_div3.output_instruction.literal_value;
            exe_div4.input_instruction.dest_reg1 = exe_div3.output_instruction.dest_reg1;
            exe_div4.input_instruction.dest_reg2 = exe_div3.output_instruction.dest_reg2;
            exe_div4.input_instruction.target_memory_addr = exe_div3.output_instruction.target_memory_addr;
            exe_div4.input_instruction.dest_reg_value = exe_div3.output_instruction.dest_reg_value;
            exe_div4.input_instruction.pc_value = exe_div3.output_instruction.pc_value;
            exe_div4.input_instruction.line_number = exe_div3.output_instruction.line_number;
            strcpy(exe_div4.input_instruction.opcode,exe_div3.output_instruction.opcode);
            strcpy(exe_div3.output_instruction.opcode,"");
            exe_div3.output_instruction.pc_value = 0;
            //scoreboard[exe_div4.input_instruction.dest_reg1].regtr.value = exe_div4.input_instruction.dest_reg_value;
            // strcpy(scoreboard[exe_div4.input_instruction.dest_reg1].regtr.status,"VALID");
            if (strcmp((exe_div4.input_instruction.opcode),"DIV" ) == 0 )
            {

                //  printf("\nSource 1 register value is %d\n",reg[exe_div1.input_instruction.source_reg1].regtr.value);
                //  printf("\nSource 2 register value is %d\n",reg[exe_div1.input_instruction.source_reg2].regtr.value);
                //  exe_div4.input_instruction.dest_reg_value = reg[exe_div4.input_instruction.source_reg1].regtr.value /
                //                                               reg[exe_div4.input_instruction.source_reg2].regtr.value;
                exe_div4.input_instruction.dest_reg_value = scoreboard[exe_div4.input_instruction.source_reg1].regtr.value /
                        scoreboard[exe_div4.input_instruction.source_reg2].regtr.value;
                // printf("\nDestination Register value Calculated is %d\n",exe_mul1.input_instruction.dest_reg_value);
                if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (exe_div4.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            exe_div4.input_instruction.dest_reg1 == decode1.input_instruction.source_reg2)
                    {
                        scoreboard[exe_div4.input_instruction.dest_reg1].regtr.value = exe_div4.input_instruction.dest_reg_value;
                        strcpy(scoreboard[exe_div4.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (exe_div4.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1)
                    {
                        scoreboard[exe_div4.input_instruction.dest_reg1].regtr.value = exe_div4.input_instruction.dest_reg_value;
                        strcpy(scoreboard[exe_div4.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (exe_div4.input_instruction.dest_reg1 == decode1.input_instruction.source_reg1 ||
                            exe_div4.input_instruction.dest_reg1 == decode1.input_instruction.dest_reg1)
                    {
                        scoreboard[exe_div4.input_instruction.dest_reg1].regtr.value = exe_div4.input_instruction.dest_reg_value;
                        strcpy(scoreboard[exe_div4.input_instruction.dest_reg1].regtr.status,"VALID");
                    }
                }
                //break;
            }
            if (exe_div4.input_instruction.pc_value == status_flags_fw.pcvalue &&
                    (strcmp((decode1.input_instruction.opcode),"BZ") == 0 || strcmp((decode1.input_instruction.opcode),"BNZ") == 0))
            {
                if (exe_div4.input_instruction.dest_reg_value == 0)
                {
                    strcpy(status_flags_fw.zero,"Y");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }

                if (exe_div4.input_instruction.dest_reg_value != 0)
                {
                    strcpy(status_flags_fw.zero,"N");
                    strcpy(status_flags_fw.psw_flag,"VALID");
                }
            }
            //strcpy(exe_mul1.output_instruction.instruction_str,"");
            //printf("\nInstruction in Execute Mul 2 is %s\n",exe_mul2.input_instruction.instruction_str);
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV4      : (I%d) %s\n",exe_div4.input_instruction.line_number,exe_div4.input_instruction.instruction_str);
            fclose(fptr);
        }
        else
        {
            //printf("\nInstruction in Execute Mul 2 is\n");
            fptr = fopen("output.txt","a");
            fprintf(fptr,"DIV4      : Empty\n");
            fclose(fptr);
        }
    }
}

void fetch ()
{
    FILE *fptr;
    fptr = fopen("output.txt","a");
    fprintf(fptr,"----------------------------------------------------------------------------------------------------\n");
    fprintf(fptr,"Cycle %d:\n",stat.cycle);
    fprintf(fptr,"----------------------------------------------------------------------------------------------------\n");
    fclose(fptr);
    //printf("Cycle Number = %d\n",stat.cycle);
    //printf("----------------------------------------------------------------------------------------------------");
    if (strcmp(fetch1.stalled,"Y") == 0)
    {
        //printf("\nFetch Stalled for %s\n",fetch1.input_instruction.instruction_str);
        fptr = fopen("output.txt","a");
        fprintf(fptr,"Fetch     : (I%d) %s Stalled\n",fetch1.input_instruction.line_number,fetch1.input_instruction.instruction_str);
        fclose(fptr);
        //bubbles = bubbles + 1;
        //printf("\nBubbles %d\n",bubbles);
    }
    if (((strcmp(fetch1.stalled,"N")) == 0) && (ins_line_number <= ins_Counter))
    {
        /*fetch.input_instruction.dest_reg1 = ins_info[ins_line_number].dest_reg2;
        fetch.input_instruction.dest_reg2 = ins_info[ins_line_number].dest_reg2;
        fetch.input_instruction.literal_value = ins_info[ins_line_number].literal_value;
        strcpy(fetch.input_instruction.opcode,ins_info[ins_line_number].opcode);
        fetch.input_instruction.pc_value = ins_info[ins_line_number].pc_value;
        fetch.input_instruction.source_reg1 = ins_info[ins_line_number].source_reg1;
        fetch.input_instruction.source_reg2 = ins_info[ins_line_number].source_reg2;*/
        strcpy(fetch1.input_instruction.instruction_str,ins_info[ins_line_number].instruction_str);
        fetch1.input_instruction.pc_value = ins_info[ins_line_number].pc_value;
        fetch1.input_instruction.line_number = ins_line_number-1;
        //printf("\nInstruction in Fetch is %s\n",fetch1.input_instruction.instruction_str);
        fptr = fopen("output.txt","a");
        fprintf(fptr,"Fetch     : (I%d) %s\n",fetch1.input_instruction.line_number,fetch1.input_instruction.instruction_str);
        fclose(fptr);
        //strcpy(fetch1.stalled,"N");
    }
    else
    {
        //printf("\nInstruction in Fetch is %s\n",fetch1.input_instruction.instruction_str);
        if (strcmp(fetch1.stalled,"Y") != 0)
        {
            fptr = fopen("output.txt","a");
            fprintf(fptr,"Fetch     : Empty %s\n",fetch1.input_instruction.instruction_str);
            fclose(fptr);
        }
    }
}
void decode()
{
    int str_counter = 0;
    FILE *fptr;
    FILE *fptr1;
    int register_exist = 0;
    char singleline[150];
    //int ins_Counter = 0;
    int ch, word = 0;
    char prev_word[5];
    int ins_line_cnt = 0;
    if (strcmp(decode1.stalled,"N") == 0)
    {
        fptr = fopen("coa_p2.txt","w");
        fclose(fptr);
        strcpy(decode1.input_instruction.instruction_str,fetch1.output_instruction.instruction_str);
        decode1.input_instruction.pc_value = fetch1.output_instruction.pc_value;
        decode1.input_instruction.line_number = fetch1.output_instruction.line_number;
        fetch1.output_instruction.pc_value = 0;
        strcpy(fetch1.output_instruction.instruction_str,"");
        //printf("\nIs it null %s\n",fetch1.output_instruction.instruction_str);
        //printf("\nInstruction in Decode is %s\n",decode1.input_instruction.instruction_str);
        if (decode1.input_instruction.pc_value > 0)
        {
            fptr1 = fopen("output.txt","a");
            fprintf(fptr1,"DRF       : (I%d) %s\n",decode1.input_instruction.line_number,decode1.input_instruction.instruction_str);
            fclose(fptr1);
        }
        else
        {
            fptr1 = fopen("output.txt","a");
            fprintf(fptr1,"DRF       : Empty %s\n",decode1.input_instruction.instruction_str);
            fclose(fptr1);
        }
        strcpy(singleline,decode1.input_instruction.instruction_str);
        //printf("%s",singleline);
        //printf("%s",decode1.input_instruction.instruction_str);
        fptr = fopen("coa_p2.txt","w");
        fprintf(fptr,"%s\n",singleline);
        fclose(fptr);
        static const char filename[] = "coa_p2.txt";
        FILE *file = fopen(filename, "r");
        if ( file != NULL )
        {

            while ( (ch = fgetc(file)) != EOF )
            {
                if ( isspace(ch) || /*ispunct(ch)*/(ch == 35) || (ch == 44) )

                {
                    if ( word )
                    {

                        if (str_counter >= 0)
                        {

                            if (strcmp(char_array,"MOVC") == 0 || strcmp(char_array,"STORE") == 0 || strcmp(char_array,"LOAD") == 0
                                    || strcmp(char_array,"ADD") == 0 || strcmp(char_array,"SUB") == 0 || strcmp(char_array,"MUL") == 0
                                    || strcmp(char_array,"BZ") == 0 || strcmp(char_array,"BNZ") == 0 || strcmp(char_array,"JUMP") == 0 || strcmp(char_array,"DIV") == 0
                                    || strcmp(char_array,"HALT") == 0 || strcmp(char_array,"AND") == 0 || strcmp(char_array,"OR") == 0 ||
                                    strcmp(char_array,"EX-OR") == 0 || strcmp(char_array,"JAL") == 0)
                            {
                                strcpy(prev_word,char_array);
                                strcpy(decode1.input_instruction.opcode,char_array);
                                // printf("\nOpcode is%s\n",decode1.input_instruction.opcode);
                                ins_line_cnt = 0;
                            }// BZ, BNZ, JUMP and HALT
                            if (strcmp(char_array,"MOVC") != 0 && strcmp(char_array,"STORE") != 0 && strcmp(char_array,"LOAD") != 0
                                    && strcmp(char_array,"ADD") != 0 && strcmp(char_array,"SUB") != 0
                                    && strcmp(char_array,"MUL") != 0 && char_array[0] != 'R' &&  strcmp(char_array,"DIV") != 0 &&
                                    strcmp(char_array,"BZ") != 0 && strcmp(char_array,"BNZ") != 0 && strcmp(char_array,"JUMP") != 0 && strcmp(char_array,"JAL") != 0
                                    && strcmp(char_array,"HALT") != 0 && strcmp(char_array,"AND") != 0
                                    && strcmp(char_array,"OR") != 0 && strcmp(char_array,"EX-OR") != 0)
                            {
                                //strcpy(ins_info[ins_Counter].literal_value,char_array);
                                //printf("\n%s\n",char_array);
                                decode1.input_instruction.literal_value = atoi(char_array);
                                // printf("\nLiteral Value is%d\n",decode1.input_instruction.literal_value);
                            }
                            /*if (strcmp(char_array,"MOVC")==0)
                            {
                                printf("\nIt is a Move Operator \n");
                            }*/
                            if (strcmp(char_array,"BZ") == 0 || strcmp(char_array,"BNZ") == 0)
                            {
                                if (strcmp(status_flags_fw.psw_flag,"INVALID") == 0)
                                {
                                    strcpy(decode1.stalled,"Y");
                                    strcpy(fetch1.stalled,"Y");
                                }
                            }
                            if (strcmp(char_array,"HALT") == 0 )
                            {
                                if (strcmp(decode1.output_instruction.opcode,"BZ") ==0 || strcmp(decode1.output_instruction.opcode,"BNZ") == 0)
                                {
                                    if (strcmp(decode1.output_instruction.opcode,"BZ") == 0 && strcmp(status_flags_fw.zero,"N" ) == 0)
                                    {
                                        ins_line_number = ins_Counter;
                                        strcpy(fetch1.output_instruction.instruction_str,"");
                                        strcpy(fetch1.input_instruction.instruction_str,"");
                                        fetch1.input_instruction.pc_value = 0;
                                        fetch1.output_instruction.pc_value = 0;
                                    }

                                    if (strcmp(decode1.output_instruction.opcode,"BNZ") == 0 && strcmp(status_flags_fw.zero,"Y" ) == 0)
                                    {
                                        ins_line_number = ins_Counter;
                                        strcpy(fetch1.output_instruction.instruction_str,"");
                                        strcpy(fetch1.input_instruction.instruction_str,"");
                                        fetch1.input_instruction.pc_value = 0;
                                        fetch1.output_instruction.pc_value = 0;
                                    }
                                }


                                if (strcmp(decode1.output_instruction.opcode,"MOVC") == 0 || strcmp(decode1.output_instruction.opcode,"STORE") == 0 || strcmp(decode1.output_instruction.opcode,"LOAD") == 0
                                        || strcmp(decode1.output_instruction.opcode,"ADD") == 0 || strcmp(decode1.output_instruction.opcode,"SUB") == 0 || strcmp(decode1.output_instruction.opcode,"MUL") == 0
                                        || strcmp(decode1.output_instruction.opcode,"JUMP") == 0 || strcmp(decode1.output_instruction.opcode,"DIV") == 0 || strcmp(decode1.output_instruction.opcode,"JAL") == 0
                                        || strcmp(decode1.output_instruction.opcode,"AND") == 0 || strcmp(decode1.output_instruction.opcode,"OR") == 0 ||
                                        strcmp(decode1.output_instruction.opcode,"EX-OR") == 0)
                                {
                                    ins_line_number = ins_Counter;
                                    strcpy(fetch1.output_instruction.instruction_str,"");
                                    strcpy(fetch1.input_instruction.instruction_str,"");
                                    fetch1.input_instruction.pc_value = 0;
                                    fetch1.output_instruction.pc_value = 0;
                                }
                            }
                            if (char_array[0] == 'R')
                            {
                                strcpy(char_array1,char_array);
                                char_array1[0] = '/0';
                                reg_addr = atoi(char_array1);
                                if (register_count == 0)
                                {
                                    //printf("\nRegister is %s",char_array);
                                    strcpy(reg[reg_addr].registers, char_array);
                                    strcpy(reg[reg_addr].regtr.status,"VALID");
                                    strcpy(scoreboard[reg_addr].registers, char_array); //added for forwarding
                                    strcpy(scoreboard[reg_addr].regtr.status,"VALID");//added for forwarding
                                    register_count = register_count + 1;
                                    //printf("\nRegister is %s",reg[reg_addr].registers);
                                }
                                else
                                {
                                    int reg_dummy_cnt = register_count;
                                    for(; reg_dummy_cnt >= 0; --reg_dummy_cnt)
                                    {
                                        if (strcmp(reg[reg_addr].registers,char_array) == 0)
                                        {
                                            register_exist = 1;
                                        }
                                    }
                                    if (register_exist == 0)
                                    {
                                        register_exist = 0;
                                        strcpy(reg[reg_addr].registers, char_array);
                                        strcpy(reg[reg_addr].regtr.status,"VALID");
                                        strcpy(scoreboard[reg_addr].registers, char_array); //added for forwarding
                                        strcpy(scoreboard[reg_addr].regtr.status,"VALID");//added for forwarding
                                        register_count = register_count + 1;
                                    }
                                    register_exist = 0;
                                }
                                //printf("\nRegister Address is %s %d %s\n",char_array,reg_addr,char_array1);

                                ins_line_cnt = ins_line_cnt + 1;

                                if (strcmp(char_array,"HALT") == 0 )
                                {
                                    if (strcmp(decode1.output_instruction.opcode,"BZ") || strcmp(decode1.output_instruction.opcode,"BNZ"))
                                    {
                                        if (strcmp(decode1.output_instruction.opcode,"BZ") && strcmp((/*status_flags*/status_flags_fw.zero),"N" ) == 0)
                                        {
                                            ins_line_number = ins_Counter;
                                            strcpy(fetch1.output_instruction.instruction_str,"");
                                            strcpy(fetch1.input_instruction.instruction_str,"");
                                            fetch1.input_instruction.pc_value =0;
                                            fetch1.output_instruction.pc_value=0;
                                        }

                                        if (strcmp(decode1.output_instruction.opcode,"BNZ") && strcmp((/*status_flags*/status_flags_fw.zero),"Y" ) == 0)
                                        {
                                            ins_line_number = ins_Counter;
                                            strcpy(fetch1.output_instruction.instruction_str,"");
                                            strcpy(fetch1.input_instruction.instruction_str,"");
                                            fetch1.input_instruction.pc_value =0;
                                            fetch1.output_instruction.pc_value=0;
                                        }
                                    }


                                    if (strcmp(decode1.output_instruction.opcode,"MOVC") == 0 || strcmp(decode1.output_instruction.opcode,"STORE") == 0 || strcmp(decode1.output_instruction.opcode,"LOAD") == 0
                                            || strcmp(decode1.output_instruction.opcode,"ADD") == 0 || strcmp(decode1.output_instruction.opcode,"SUB") == 0 || strcmp(decode1.output_instruction.opcode,"MUL") == 0
                                            || strcmp(decode1.output_instruction.opcode,"JUMP") == 0 || strcmp(decode1.output_instruction.opcode,"DIV") == 0
                                            || strcmp(decode1.output_instruction.opcode,"AND") == 0 || strcmp(decode1.output_instruction.opcode,"OR") == 0 ||
                                            strcmp(decode1.output_instruction.opcode,"EX-OR") == 0 || strcmp(decode1.output_instruction.opcode,"JAL") == 0)
                                    {
                                        ins_line_number = ins_Counter;
                                        strcpy(fetch1.output_instruction.instruction_str,"");
                                        strcpy(fetch1.input_instruction.instruction_str,"");
                                    }
                                }

                                if ((ins_line_cnt == 1) && (strcmp(prev_word,"JUMP") == 0 ))
                                {
                                    decode1.input_instruction.source_reg1 = reg_addr;
                                    // printf("\nRegister 1 Value is is%d\n",decode1.input_instruction.source_reg1);
                                    decode1.input_instruction.source_reg1_value = reg[reg_addr].regtr.value;
                                    //if (strcmp(reg[reg_addr].regtr.status,"INVALID") == 0 )//commented for scoreboard
                                    if (strcmp(scoreboard[reg_addr].regtr.status,"INVALID") == 0 ) //added for scoreboard
                                    {
                                        strcpy(decode1.stalled,"Y");
                                        strcpy(fetch1.stalled,"Y");
                                        // printf("\nDecode Stage Stalled %s\n",decode1.stalled);
                                    }
                                }
                                if ((ins_line_cnt == 2 || ins_line_cnt ==3)  &&
                                        (strcmp(prev_word,"MOVC") == 0 || strcmp(prev_word,"ADD") == 0 || strcmp(prev_word,"LOAD") ==0
                                         || strcmp(prev_word,"SUB") == 0 || strcmp(prev_word,"MUL") == 0 || strcmp(prev_word,"DIV") == 0 ||
                                         strcmp(prev_word,"AND") == 0 || strcmp(prev_word,"OR") == 0 || strcmp(prev_word,"EX-OR") == 0 || strcmp(prev_word,"JAL") == 0))
                                {
                                    if  (ins_line_cnt == 2)
                                    {
                                        decode1.input_instruction.source_reg1 = reg_addr;
                                        // printf("\nRegister 1 Value is is%d\n",decode1.input_instruction.source_reg1);
                                        decode1.input_instruction.source_reg1_value = reg[reg_addr].regtr.value;
                                        //if (strcmp(reg[reg_addr].regtr.status,"INVALID") == 0 )//commented for scoreboard
                                        if (strcmp(scoreboard[reg_addr].regtr.status,"INVALID") == 0 )//added for scoreboard
                                        {
                                            strcpy(decode1.stalled,"Y");
                                            strcpy(fetch1.stalled,"Y");
                                            //   printf("\nDecode Stage Stalled %s\n",decode1.stalled);
                                        }
                                    }

                                    if  (ins_line_cnt == 3)
                                    {
                                        decode1.input_instruction.source_reg2 = reg_addr;
                                        //  printf("\nRegister 2 Value is is%d\n",decode1.input_instruction.source_reg2);
                                        decode1.input_instruction.source_reg2_value = reg[reg_addr].regtr.value;

                                        // if (strcmp(reg[reg_addr].regtr.status,"INVALID") == 0 ) //commented for scoreboard
                                        if (strcmp(scoreboard[reg_addr].regtr.status,"INVALID") == 0 ) //added for scoreboard
                                        {
                                            strcpy(decode1.stalled,"Y");
                                            strcpy(fetch1.stalled,"Y");
                                            //  printf("\nDecode Stage Stalled %s\n",decode1.stalled);
                                        }
                                    }
                                }

                                if ((ins_line_cnt == 2 || ins_line_cnt ==3)  &&
                                        (strcmp(prev_word,"STORE") == 0))
                                {
                                    if  (ins_line_cnt == 2)
                                    {
                                        decode1.input_instruction.dest_reg1 = reg_addr;
                                        // printf("\nDestination Register 1 Value is is%d\n",decode1.input_instruction.dest_reg1);
                                        decode1.input_instruction.dest_reg1_value = reg[reg_addr].regtr.value;
                                        //if (strcmp(reg[reg_addr].regtr.status,"INVALID") == 0 ) // commented for scoreboard
                                        if (strcmp(scoreboard[reg_addr].regtr.status,"INVALID") == 0 ) //added for scoreboard
                                        {
                                            strcpy(decode1.stalled,"Y");
                                            strcpy(fetch1.stalled,"Y");
                                            //  printf("\nDecode Stage Stalled %s\n",decode1.stalled);
                                        }
                                    }

                                    if  (ins_line_cnt == 3)
                                    {
                                        decode1.input_instruction.dest_reg2 = reg_addr;
                                        // printf("\nDestination Register 2 Value is is%d\n",decode1.input_instruction.dest_reg2);
                                        decode1.input_instruction.dest_reg2_value = reg[reg_addr].regtr.value;
                                    }
                                }

                                if ((ins_line_cnt == 1)  &&
                                        (strcmp(prev_word,"STORE") == 0))
                                {
                                    decode1.input_instruction.source_reg1 = reg_addr;
                                    // printf("\Source Register 1 Value is is%d%s\n",decode1.input_instruction.source_reg1,reg[reg_addr].regtr.status);
                                    //if (strcmp(reg[reg_addr].regtr.status,"INVALID") == 0 ) //commented for scoreboard
                                    if (strcmp(scoreboard[reg_addr].regtr.status,"INVALID") == 0 ) //added for scoreboard
                                    {
                                        strcpy(decode1.stalled,"Y");
                                        strcpy(fetch1.stalled,"Y");
                                        //  printf("\nDecode Stage Stalled %s\n",decode1.stalled);
                                    }
                                }

                                if ((ins_line_cnt == 1)  &&
                                        (strcmp(prev_word,"MOVC") == 0 || strcmp(prev_word,"ADD") == 0 || strcmp(prev_word,"LOAD") ==0
                                         || strcmp(prev_word,"SUB") == 0 || strcmp(prev_word,"MUL") == 0 || strcmp(prev_word,"DIV") == 0 ||
                                         strcmp(prev_word,"AND") == 0 || strcmp(prev_word,"OR") == 0 || strcmp(prev_word,"EX-OR") == 0 || strcmp(prev_word,"JAL") == 0))
                                {
                                    decode1.input_instruction.dest_reg1 = reg_addr;
                                    // printf("\nDestination Register is%d\n",decode1.input_instruction.dest_reg1);
                                    //if (strcmp(reg[reg_addr].regtr.status,"VALID") == 0)
                                    //{
                                    //strcpy(reg[reg_addr].regtr.status,"INVALID");//for testing
                                    //}
                                    if (strcmp(reg[reg_addr].regtr.status,"INVALID") == 0 ) //commented for scoreboard
                                    {
                                        strcpy(decode1.stalled,"Y");
                                        strcpy(fetch1.stalled,"Y");
                                        //  printf("\nDecode Stage Stalled %s\n",decode1.stalled);
                                    } // added for forwarding
                                }
                                /*if (register_count == 0)
                                {
                                //printf("\nRegister is %s",char_array);
                                strcpy(reg[reg_addr].registers , char_array);
                                strcpy(reg[reg_addr].regtr.status,"VALID");
                                register_count = register_count + 1;
                                //printf("\nRegister is %s",reg[reg_addr].registers);
                                }
                                else
                                {
                                   int reg_dummy_cnt = register_count;
                                   for(;reg_dummy_cnt >= 0;--reg_dummy_cnt)
                                   {
                                    if (strcmp(reg[reg_addr].registers,char_array) == 0)
                                    {
                                     register_exist = 1;
                                    }
                                   }
                                   if (register_exist == 0)
                                   {
                                       register_exist = 0;
                                       strcpy(reg[reg_addr].registers , char_array);
                                       strcpy(reg[reg_addr].regtr.status,"VALID");
                                       register_count = register_count + 1;
                                   }
                                   register_exist = 0;
                                }*/
                            }
                            //printf("\n%s",char_array);
                            for(; str_counter >= 0; --str_counter)
                            {
                                char_array[str_counter] = '\0';
                            }
                            str_counter = 0;
                        }
                        word = 0;
                        // putchar('\n');
                    }
                }
                else
                {
                    word = 1;
                    char_array[str_counter] = ch;
                    str_counter = str_counter + 1;
                    //  putchar(ch);
                }
            }
        }
        fclose(file);
    }
    else
    {
        if (strcmp(decode1.stalled,"Y") == 0)
        {
            //printf("\nDecode Stalled for %s\n",decode1.input_instruction.instruction_str);
            fptr1 = fopen("output.txt","a");
            fprintf(fptr1,"DRF       : (I%d) %s Stalled\n",decode1.input_instruction.line_number,decode1.input_instruction.instruction_str);
            fclose(fptr1);

            //bubbles = bubbles + 1;
            //printf("\nNo. of Bubbles %d\n",bubbles);
        }

    }
    /*if (ins_line_number == 2)
    {


    strcpy(eof_flag,"Y");}*/
}
/*
void execute()
{

}*/
void simulator(int simulate_cycle)
{
    FILE *fptr;
    //  strcpy(status_flags.zero,"N");
    while (1)
    {

        if (ins_line_number == 1)
        {
            strcpy(fetch1.stalled,"N");
            strcpy(status_flags.zero,"N");
            strcpy(decode1.stalled,"N");
            strcpy(execution.stalled,"N");
            strcpy(exe_mul1.stalled,"N");
            strcpy(exe_mul2.stalled,"N");
            strcpy(exe_div1.stalled,"N");
            strcpy(exe_div2.stalled,"N");
            strcpy(exe_div3.stalled,"N");
            strcpy(exe_div4.stalled,"N");
            strcpy(mem.stalled,"N");
            strcpy(wb.stalled,"N");
            stat.cycle = 1;
        }
        //fptr = fopen("output.txt","w");
        //fclose(fptr);
        fetch();
        decode();
        execute_div1();
        execute_div2();
        execute_div3();
        execute_div4();
        execute_mul1();
        execute_mul2();
        execute();
        memory();
        writeback();
        if (simulate_cycle == stat.cycle)
        {
            strcpy(eof_flag,"Y");
        }
        stat.cycle++;
        if (div4_flag == 0 )
        {
            if (strcmp((exe_mul2.input_instruction.opcode),"MUL" ) == 0)
            {
                strcpy(exe_mul2.stalled,"Y");
                if (strcmp((exe_mul1.input_instruction.opcode),"MUL" ) == 0)
                {
                    strcpy(exe_mul1.stalled,"Y");
                    if (strcmp((decode1.input_instruction.opcode),"MUL" ) == 0)
                    {
                        strcpy(decode1.stalled,"Y");
                        strcpy(fetch1.stalled,"Y");
                    }
                }
            }

            if (strcmp((execution.input_instruction.opcode),"ADD" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"SUB" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"BZ" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"BNZ" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"LOAD" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"STORE" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"MOVC" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"JUMP" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"AND" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"OR" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"EX-OR" ) == 0 ||
                    //strcmp((execution.input_instruction.opcode),"HALT" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"JAL" ) == 0)
            {
                strcpy(execution.stalled,"Y");
                exe_stall_by_mul = 2;
            }

            if (strcmp((decode1.input_instruction.opcode),"ADD" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"SUB" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"BZ" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"BNZ" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"LOAD" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"STORE" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"MOVC" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"AND" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"OR" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"EX-OR" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"HALT" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL" ) == 0)
            {
                if (strcmp(execution.stalled,"Y") == 0)
                {
                    strcpy(decode1.stalled,"Y");
                    strcpy(fetch1.stalled,"Y");
                    //fetch_dec_div = 0;
                }
            }

            div4_flag = 1;
        }
        if (mul2_flag == 0)
        {
            if (strcmp((execution.input_instruction.opcode),"ADD" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"SUB" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"BZ" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"BNZ" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"LOAD" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"STORE" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"MOVC" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"JUMP" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"AND" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"OR" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"EX-OR" ) == 0 ||
                    //  strcmp((execution.input_instruction.opcode),"HALT" ) == 0 ||
                    strcmp((execution.input_instruction.opcode),"JAL" ) == 0)
            {
                strcpy(execution.stalled,"Y");
                exe_stall_by_mul = 0;
            }

            if (strcmp((decode1.input_instruction.opcode),"ADD" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"SUB" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"BZ" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"BNZ" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"LOAD" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"STORE" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"MOVC" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"AND" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"OR" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"EX-OR" ) == 0||
                    strcmp((decode1.input_instruction.opcode),"HALT" ) == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL" ) == 0)
            {
                if (strcmp(execution.stalled,"Y") == 0)
                {
                    strcpy(decode1.stalled,"Y");
                    /*if (strcmp((fetch1.input_instruction.opcode),"ADD" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"SUB" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"BZ" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"BNZ" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"LOAD" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"STORE" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"MOVC" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"JUMP" ) == 0||
                        strcmp((fetch1.input_instruction.opcode),"AND" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"OR" ) == 0 ||
                        strcmp((fetch1.input_instruction.opcode),"EX-OR" ) == 0)
                    {*/
                    strcpy(fetch1.stalled,"Y");
                    // }
                    fetch_dec = 0;
                    mul2_flag = 1;
                }
            }

        }

        int reg_count = 0;
        for(; reg_count <= 16; reg_count++)
            {
                if  (scoreboard[reg_count].registers[0] == 'R')
                {

                 if (strcmp(scoreboard[reg_count].regtr.status,"VALID") == 0)
                 {
                   if (strcmp(decode1.stalled,"N") == 0 && strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                   {
                     strcpy(scoreboard[reg_count].regtr.status,"INVALID");
                   }
                   if (strcmp(decode1.stalled,"Y") == 0)
                   {
                     if (strcmp((decode1.input_instruction.opcode),"AND") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"EX-OR") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"ADD") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"SUB") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"MUL") == 0 ||
                       // strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                        strcmp((decode1.input_instruction.opcode),"DIV") == 0)
                {
                    if (reg_count == decode1.input_instruction.source_reg1 ||
                           reg_count == decode1.input_instruction.source_reg2)
                    {

                    }
                    else
                    {
                        if (strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                        {
                        strcpy(scoreboard[reg_count].regtr.status,"INVALID");}
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"LOAD") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JUMP") == 0 ||
                    strcmp((decode1.input_instruction.opcode),"JAL") == 0)
                {
                    if (reg_count != decode1.input_instruction.source_reg1)
                    {
                      if (strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                        {
                        strcpy(scoreboard[reg_count].regtr.status,"INVALID");}
                    }
                }

                if (strcmp((decode1.input_instruction.opcode),"STORE") == 0)
                {
                    if (reg_count == decode1.input_instruction.source_reg1 ||
                            reg_count == decode1.input_instruction.dest_reg1)
                    {

                    }
                    else
                    {
                         if (strcmp(reg[reg_count].regtr.status,"INVALID") == 0)
                        {
                        strcpy(scoreboard[reg_count].regtr.status,"INVALID");}
                    }
                }
                   }
                 }

                }

            }

        if (strcmp(fetch1.stalled,"N") == 0 && ins_line_number <= ins_Counter)
        {
            ins_line_number = ins_line_number + 1;
            strcpy(fetch1.output_instruction.instruction_str,fetch1.input_instruction.instruction_str);
            fetch1.output_instruction.pc_value = fetch1.input_instruction.pc_value;
            fetch1.output_instruction.line_number = fetch1.input_instruction.line_number;
            strcpy(fetch1.input_instruction.instruction_str,"");
            fetch1.input_instruction.pc_value = 0;
        }

        if (strcmp(decode1.stalled,"N") == 0)
        {
            strcpy(decode1.output_instruction.instruction_str,decode1.input_instruction.instruction_str);
            decode1.output_instruction.source_reg1 = decode1.input_instruction.source_reg1;
            decode1.output_instruction.source_reg2 = decode1.input_instruction.source_reg2;
            decode1.output_instruction.literal_value = decode1.input_instruction.literal_value;
            decode1.output_instruction.dest_reg1 = decode1.input_instruction.dest_reg1;
            decode1.output_instruction.dest_reg2 = decode1.input_instruction.dest_reg2;
            decode1.output_instruction.dest_reg_value = decode1.input_instruction.dest_reg_value;
            strcpy(decode1.output_instruction.opcode,decode1.input_instruction.opcode);
            decode1.output_instruction.pc_value = decode1.input_instruction.pc_value;
            decode1.output_instruction.line_number = decode1.input_instruction.line_number;
            if (strcmp(decode1.output_instruction.opcode,"MOVC") == 0 ||
                    strcmp(decode1.output_instruction.opcode,"ADD") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"SUB") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"MUL") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"LOAD") == 0 ||
                    strcmp(decode1.output_instruction.opcode,"AND") == 0 ||
                    strcmp(decode1.output_instruction.opcode,"OR") == 0 ||
                    strcmp(decode1.output_instruction.opcode,"DIV") == 0 ||
                    strcmp(decode1.output_instruction.opcode,"JAL") == 0 ||
                    strcmp(decode1.output_instruction.opcode,"EX-OR") == 0)
            {
                if (strcmp(reg[decode1.output_instruction.dest_reg1].regtr.status,"VALID") == 0)
                {
                    strcpy(reg[decode1.output_instruction.dest_reg1].regtr.status,"INVALID");//for testing
                    // printf("\n%d %s %s\n",stat.cycle,decode1.output_instruction.instruction_str,reg[decode1.output_instruction.dest_reg1].regtr.status);
                    /*  printf("\nDestination Register Status%s %s\n",reg[decode1.output_instruction.dest_reg1].registers
                             ,reg[decode1.output_instruction.dest_reg1].regtr.status);*/
                }
                if (strcmp(scoreboard[decode1.output_instruction.dest_reg1].regtr.status,"VALID") == 0)
                {
                    strcpy(scoreboard[decode1.output_instruction.dest_reg1].regtr.status,"INVALID");//for testing
                    // printf("\n%d %s %s\n",stat.cycle,decode1.output_instruction.instruction_str,reg[decode1.output_instruction.dest_reg1].regtr.status);
                    /*  printf("\nDestination Register Status%s %s\n",reg[decode1.output_instruction.dest_reg1].registers
                             ,reg[decode1.output_instruction.dest_reg1].regtr.status);*/
                }
            } // added for scoreboard
            if (strcmp(decode1.output_instruction.opcode,"ADD") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"SUB") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"MUL") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"DIV") == 0)
            {
                strcpy(status_flags_fw.psw_flag,"INVALID");
                status_flags_fw.pcvalue = decode1.input_instruction.pc_value;
            }
            strcpy(decode1.input_instruction.opcode,"");
            decode1.input_instruction.pc_value = 0;

            if (strcmp(decode1.output_instruction.opcode,"ADD") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"SUB") == 0  ||
                    strcmp(decode1.output_instruction.opcode,"MUL") == 0  ||
                    // strcmp(decode1.output_instruction.opcode,"AND") == 0  ||
                    // strcmp(decode1.output_instruction.opcode,"OR") == 0   ||
                    strcmp(decode1.output_instruction.opcode,"DIV") == 0  // ||
                    /*  strcmp(decode1.output_instruction.opcode,"EX-OR") == 0 */ )
            {
                psw_counter = psw_counter + 1;
                if (psw_counter == 1)
                {
                    strcpy(status_flags.psw_flag,"INVALID");
                    //printf("\n%s\n",status_flags.psw_flag);
                }
            }
            //strcpy(decode1.input_instruction.opcode,"");
            //decode1.input_instruction.instruction_str = '/0';
        }
        if (strcmp(execution.stalled,"N") == 0)
        {
            strcpy(execution.output_instruction.instruction_str,execution.input_instruction.instruction_str);
            execution.output_instruction.source_reg1 = execution.input_instruction.source_reg1;
            execution.output_instruction.source_reg2 = execution.input_instruction.source_reg2;
            execution.output_instruction.literal_value = execution.input_instruction.literal_value;
            execution.output_instruction.dest_reg1 = execution.input_instruction.dest_reg1;
            execution.output_instruction.dest_reg_value = execution.input_instruction.dest_reg_value;
            //   printf("\nChecking Value in Exec %d %d\n",execution.output_instruction.dest_reg1,execution.output_instruction.dest_reg_value );
            execution.output_instruction.dest_reg2 = execution.input_instruction.dest_reg2;
            execution.output_instruction.target_memory_addr = execution.input_instruction.target_memory_addr;
            execution.output_instruction.pc_value = execution.input_instruction.pc_value;
            strcpy(execution.output_instruction.opcode,execution.input_instruction.opcode);
            execution.output_instruction.line_number = execution.input_instruction.line_number;
            strcpy(execution.input_instruction.opcode,"");
            strcpy(execution.input_instruction.instruction_str,"");
            execution.input_instruction.pc_value = 0;
        }

        if (strcmp(exe_mul1.stalled,"N") == 0)
        {
            strcpy(exe_mul1.output_instruction.instruction_str,exe_mul1.input_instruction.instruction_str);
            exe_mul1.output_instruction.source_reg1 = exe_mul1.input_instruction.source_reg1;
            exe_mul1.output_instruction.source_reg2 = exe_mul1.input_instruction.source_reg2;
            exe_mul1.output_instruction.literal_value = exe_mul1.input_instruction.literal_value;
            exe_mul1.output_instruction.dest_reg1 = exe_mul1.input_instruction.dest_reg1;
            exe_mul1.output_instruction.pc_value = exe_mul1.input_instruction.pc_value;
            exe_mul1.output_instruction.dest_reg_value = exe_mul1.input_instruction.dest_reg_value;
            exe_mul1.output_instruction.dest_reg2 = exe_mul1.input_instruction.dest_reg2;
            exe_mul1.output_instruction.target_memory_addr = exe_mul1.input_instruction.target_memory_addr;
            exe_mul1.output_instruction.line_number = exe_mul1.input_instruction.line_number;
            strcpy(exe_mul1.output_instruction.opcode,exe_mul1.input_instruction.opcode);
            strcpy(exe_mul1.input_instruction.opcode,"");
            strcpy(exe_mul1.input_instruction.instruction_str,"");
            exe_mul1.input_instruction.pc_value = 0;
        }

        if (strcmp(exe_mul2.stalled,"N") == 0)
        {
            strcpy(exe_mul2.output_instruction.instruction_str,exe_mul2.input_instruction.instruction_str);
            exe_mul2.output_instruction.source_reg1 = exe_mul2.input_instruction.source_reg1;
            exe_mul2.output_instruction.source_reg2 = exe_mul2.input_instruction.source_reg2;
            exe_mul2.output_instruction.literal_value = exe_mul2.input_instruction.literal_value;
            exe_mul2.output_instruction.dest_reg1 = exe_mul2.input_instruction.dest_reg1;
            exe_mul2.output_instruction.pc_value = exe_mul2.input_instruction.pc_value;
            exe_mul2.output_instruction.dest_reg_value = exe_mul2.input_instruction.dest_reg_value;
            exe_mul2.output_instruction.dest_reg2 = exe_mul2.input_instruction.dest_reg2;
            exe_mul2.output_instruction.target_memory_addr = exe_mul2.input_instruction.target_memory_addr;
            exe_mul2.output_instruction.line_number = exe_mul2.input_instruction.line_number;
            strcpy(exe_mul2.output_instruction.opcode,exe_mul2.input_instruction.opcode);
            strcpy(exe_mul2.input_instruction.opcode,"");
            strcpy(exe_mul2.input_instruction.instruction_str,"");
            mul2_flag = 1;
            exe_mul2.input_instruction.pc_value = 0;
        }

        if (strcmp(exe_div1.stalled,"N") == 0)
        {
            strcpy(exe_div1.output_instruction.instruction_str,exe_div1.input_instruction.instruction_str);
            exe_div1.output_instruction.source_reg1 = exe_div1.input_instruction.source_reg1;
            exe_div1.output_instruction.source_reg2 = exe_div1.input_instruction.source_reg2;
            exe_div1.output_instruction.literal_value = exe_div1.input_instruction.literal_value;
            exe_div1.output_instruction.dest_reg1 = exe_div1.input_instruction.dest_reg1;
            exe_div1.output_instruction.pc_value = exe_div1.input_instruction.pc_value;
            exe_div1.output_instruction.dest_reg_value = exe_div1.input_instruction.dest_reg_value;
            exe_div1.output_instruction.dest_reg2 = exe_div1.input_instruction.dest_reg2;
            exe_div1.output_instruction.target_memory_addr = exe_div1.input_instruction.target_memory_addr;
            exe_div1.output_instruction.line_number = exe_div1.input_instruction.line_number;
            strcpy(exe_div1.output_instruction.opcode,exe_div1.input_instruction.opcode);
            strcpy(exe_div1.input_instruction.opcode,"");
            strcpy(exe_div1.input_instruction.instruction_str,"");
            exe_div1.input_instruction.pc_value = 0;
        }

        if (strcmp(exe_div2.stalled,"N") == 0)
        {
            strcpy(exe_div2.output_instruction.instruction_str,exe_div2.input_instruction.instruction_str);
            exe_div2.output_instruction.source_reg1 = exe_div2.input_instruction.source_reg1;
            exe_div2.output_instruction.source_reg2 = exe_div2.input_instruction.source_reg2;
            exe_div2.output_instruction.literal_value = exe_div2.input_instruction.literal_value;
            exe_div2.output_instruction.dest_reg1 = exe_div2.input_instruction.dest_reg1;
            exe_div2.output_instruction.pc_value = exe_div2.input_instruction.pc_value;
            exe_div2.output_instruction.dest_reg_value = exe_div2.input_instruction.dest_reg_value;
            exe_div2.output_instruction.dest_reg2 = exe_div2.input_instruction.dest_reg2;
            exe_div2.output_instruction.target_memory_addr = exe_div2.input_instruction.target_memory_addr;
            exe_div2.output_instruction.line_number = exe_div2.input_instruction.line_number;
            strcpy(exe_div2.output_instruction.opcode,exe_div2.input_instruction.opcode);
            strcpy(exe_div2.input_instruction.opcode,"");
            strcpy(exe_div2.input_instruction.instruction_str,"");
            exe_div2.input_instruction.pc_value = 0;
        }

        if (strcmp(exe_div3.stalled,"N") == 0)
        {
            strcpy(exe_div3.output_instruction.instruction_str,exe_div3.input_instruction.instruction_str);
            exe_div3.output_instruction.source_reg1 = exe_div3.input_instruction.source_reg1;
            exe_div3.output_instruction.source_reg2 = exe_div3.input_instruction.source_reg2;
            exe_div3.output_instruction.literal_value = exe_div3.input_instruction.literal_value;
            exe_div3.output_instruction.dest_reg1 = exe_div3.input_instruction.dest_reg1;
            exe_div3.output_instruction.pc_value = exe_div3.input_instruction.pc_value;
            exe_div3.output_instruction.dest_reg_value = exe_div3.input_instruction.dest_reg_value;
            exe_div3.output_instruction.dest_reg2 = exe_div3.input_instruction.dest_reg2;
            exe_div3.output_instruction.target_memory_addr = exe_div3.input_instruction.target_memory_addr;
            exe_div3.output_instruction.line_number = exe_div3.input_instruction.line_number;
            strcpy(exe_div3.output_instruction.opcode,exe_div3.input_instruction.opcode);
            strcpy(exe_div3.input_instruction.opcode,"");
            strcpy(exe_div3.input_instruction.instruction_str,"");
            exe_div3.input_instruction.pc_value = 0;
        }

        if (strcmp(exe_div4.stalled,"N") == 0)
        {
            strcpy(exe_div4.output_instruction.instruction_str,exe_div4.input_instruction.instruction_str);
            exe_div4.output_instruction.source_reg1 = exe_div4.input_instruction.source_reg1;
            exe_div4.output_instruction.source_reg2 = exe_div4.input_instruction.source_reg2;
            exe_div4.output_instruction.literal_value = exe_div4.input_instruction.literal_value;
            exe_div4.output_instruction.dest_reg1 = exe_div4.input_instruction.dest_reg1;
            exe_div4.output_instruction.pc_value = exe_div4.input_instruction.pc_value;
            exe_div4.output_instruction.dest_reg_value = exe_div4.input_instruction.dest_reg_value;
            exe_div4.output_instruction.dest_reg2 = exe_div4.input_instruction.dest_reg2;
            exe_div4.output_instruction.target_memory_addr = exe_div4.input_instruction.target_memory_addr;
            exe_div4.output_instruction.line_number = exe_div4.input_instruction.line_number;
            strcpy(exe_div4.output_instruction.opcode,exe_div4.input_instruction.opcode);
            strcpy(exe_div4.input_instruction.opcode,"");
            strcpy(exe_div4.input_instruction.instruction_str,"");
            exe_div4.input_instruction.pc_value = 0;
        }


        if (strcmp(mem.stalled,"N") == 0)
        {
            strcpy(mem.output_instruction.instruction_str,mem.input_instruction.instruction_str);
            mem.output_instruction.source_reg1 = mem.input_instruction.source_reg1;
            mem.output_instruction.source_reg2 = mem.input_instruction.source_reg2;
            mem.output_instruction.literal_value = mem.input_instruction.literal_value;
            mem.output_instruction.dest_reg1 = mem.input_instruction.dest_reg1;
            mem.output_instruction.pc_value = mem.input_instruction.pc_value;
            //printf("\nChecking Value in Mem %d%d\n",mem.output_instruction.dest_reg1,mem.output_instruction.dest_reg_value );
            mem.output_instruction.dest_reg2 = mem.input_instruction.dest_reg2;
            mem.output_instruction.dest_reg_value = mem.input_instruction.dest_reg_value;
            mem.output_instruction.target_memory_data = mem.input_instruction.target_memory_data;
            mem.output_instruction.line_number = mem.input_instruction.line_number;
            strcpy(mem.output_instruction.opcode,mem.input_instruction.opcode);
            strcpy(mem.input_instruction.opcode,"");
            strcpy(mem.input_instruction.instruction_str,"");
            mem.input_instruction.pc_value = 0;
        }

        if (strcmp(wb.stalled,"N") == 0)
        {
            strcpy(wb.output_instruction.instruction_str,wb.input_instruction.instruction_str);
            wb.output_instruction.source_reg1 = wb.input_instruction.source_reg1;
            wb.output_instruction.source_reg2 = wb.input_instruction.source_reg2;
            wb.output_instruction.literal_value = wb.input_instruction.literal_value;
            wb.output_instruction.pc_value = wb.input_instruction.pc_value;
            wb.output_instruction.dest_reg1 = wb.input_instruction.dest_reg1;
            wb.output_instruction.dest_reg2 = wb.input_instruction.dest_reg2;
            wb.output_instruction.dest_reg_value = wb.input_instruction.dest_reg_value;
            wb.output_instruction.line_number = wb.input_instruction.line_number;
            strcpy(wb.output_instruction.opcode,wb.input_instruction.opcode);
            strcpy(wb.input_instruction.opcode,"");
            strcpy(wb.input_instruction.instruction_str,"");
            wb.input_instruction.pc_value = 0;
        }

        if (strcmp(eof_flag,"Y") == 0 )
        {
            register_count = 0;
            fptr = fopen("output.txt","a");
            fprintf(fptr,"----------------------------------------------------------------------------------------------------");
            fprintf(fptr,"\nRegister Values are :\n");
            fprintf(fptr,"----------------------------------------------------------------------------------------------------");
            fclose(fptr);
            for(; register_count <= 16; register_count++)
            {
                if  (reg[register_count].registers[0] == 'R')
                {
                    fptr = fopen("output.txt","a");
                    fprintf(fptr,"\nRegister is %s its value is %d and status its is %s\n",reg[register_count].registers,reg[register_count].regtr.value,reg[register_count].regtr.status);
                    fclose(fptr);
                    //printf("\nRegister is %s its value is %d and status its is %s\n",reg[register_count].registers,reg[register_count].regtr.value,reg[register_count].regtr.status);
                }

            }
            int mem_cnt = 0;
            fptr = fopen("output.txt","a");
            fprintf(fptr,"----------------------------------------------------------------------------------------------------");
            fprintf(fptr,"\nMemory Values are :\n");
            fprintf(fptr,"----------------------------------------------------------------------------------------------------");
            fclose(fptr);
            for(; mem_cnt <= 4000; mem_cnt++)
            {
                if (data_mem[mem_cnt].mem_char == 'M')
                {
                    //printf("\nMemory Location is %d and its value is %d\n",data_mem[mem_cnt].mem_address,data_mem[mem_cnt].mem_value);
                    fptr = fopen("output.txt","a");
                    fprintf(fptr,"\nMemory Location is %d and its value is %d\n",data_mem[mem_cnt].mem_address,data_mem[mem_cnt].mem_value);
                    fclose(fptr);
                }
            }
            printf("\nSuccessfully Simulated\n");
            //printf("\nMemory Location and is value %d %d\n",data_mem[0].mem_address,data_mem[0].mem_value);
            //printf("\nMemory Location and is value %d %d\n",data_mem[1].mem_address,data_mem[1].mem_value);
            //printf("\nMemory Location and is value %d %d\n",data_mem[2].mem_address,data_mem[2].mem_value);
            //printf("\nMemory Location and is value %d %d\n",data_mem[3].mem_address,data_mem[3].mem_value);
            //printf("\nTotal No. of Bubbles %d\n",bubbles);
            break;
        }
    }
}
void initialize(char *filename)
{
    //int str_counter = 0;
    FILE * fpointer;
    FILE *fptr;
    FILE *fptr1;
    //FILE *fptr;
    //int register_exist = 0;
    char singleline[150];
    //int ins_Counter = 0;
    int mem_add = 4000;
    //fpointer = fopen("input.txt","r");
    fpointer = fopen(filename,"r");
    int ch, word = 0;
    char prev_word[5];
    int ins_line_cnt = 0;
    stat.cycle = 0;
    ins_Counter = 0;
    ins_line_number =1;
    strcpy(fetch1.stalled,"N");
    strcpy(decode1.stalled,"N");
    strcpy(execution.stalled,"N");
    strcpy(exe_mul1.stalled,"N");
    strcpy(exe_mul2.stalled,"N");
    strcpy(exe_div1.stalled,"N");
    strcpy(exe_div2.stalled,"N");
    strcpy(exe_div3.stalled,"N");
    strcpy(exe_div4.stalled,"N");
    strcpy(mem.stalled,"N");
    strcpy(wb.stalled,"N");
    strcpy(eof_flag,"N");
    wb.input_instruction.pc_value = 0;
    psw_counter = 0;
    mul2_flag = 1;
    register_count =0;
    strcpy(status_flags.zero,"N");
    fptr = fopen("output.txt","w");
    fclose(fptr);
    strcpy(status_flags.psw_flag,"VALID");
    strcpy(status_flags_fw.psw_flag,"VALID");
    status_flags_fw.pcvalue = 0;
    for(; register_count <= 16; register_count++)
    {
        if  (reg[register_count].registers[0] == 'R')
        {
            reg[register_count].registers[0] = '\0';
            reg[register_count].registers[1] = '\0';
            reg[register_count].regtr.value = 0;
        }

        if  (scoreboard[register_count].registers[0] == 'R')
        {
            reg[register_count].registers[0] = '\0';
            reg[register_count].registers[1] = '\0';
            reg[register_count].regtr.value = 0;
        } // added for scoreboard

    }
    int mem_cnt = 0;
    for(; mem_cnt <= 4000; mem_cnt++)
    {
        if (data_mem[mem_cnt].mem_char == 'M')
        {
            data_mem[mem_cnt].mem_address = 0;
            data_mem[mem_cnt].mem_char = '\0';
            data_mem[mem_cnt].mem_value = 0;
        }
    }

    for (;;)
    {

        fgets(singleline,150,fpointer);
        //if (feof( fpointer )) break;
        ins_Counter = ins_Counter + 1;
        strcpy(ins[ins_Counter].instruction_string,singleline);
        ins[ins_Counter].address = mem_add;
        ins[ins_Counter].line_number = ins_Counter;
        ins_info[ins_Counter].pc_value = mem_add;
        strcpy(ins_info[ins_Counter].instruction_str,singleline);
        mem_add = mem_add + 4;
        // printf("\n");
        //printf("\n%s\n",ins_info[ins_Counter].instruction_str);
        //puts(ins[ins_Counter].instruction_string);
        if (feof( fpointer )) break;



    }



    fclose(fpointer);
    fptr1 = fopen("output.txt","a");
    fprintf(fptr1,"\nNumber Of Instructions: %d\n\n",ins_Counter);
    fclose(fptr1);

    //printf("\nNumber Of Instructions: %d\n\n",ins_Counter);
    printf("\nSuccessfully Initialized\n");
}
void display ()
{
    FILE * fpointer;
    char singleline[150];
    fpointer = fopen("output.txt","r");
    for (;;)
    {
        //if (feof( fpointer )) break;
        fgets(singleline,150,fpointer);
        //puts(singleline);
        if (feof( fpointer )) break;
        puts(singleline);
    }
    fclose(fpointer);
}
int main(int argc, char *argv[])
{
    char option[20];
    FILE *fptr;
    int ch, word = 0;
    int ins_line_cnt = 0;
    char char_array2[20];
    int str_counter = 0;
    char singleline1[50];
    fptr = fopen("simulate.txt","w");
    fclose(fptr);
    fptr = fopen("output.txt","w");
    fclose(fptr);
    while(1)
    {
        printf("========================================================\n\n");

        printf("1. Initialize\n\n");
        printf("2. Simulate\n\n");
        printf("3. Display\n\n");
        printf("4. Exit\n\n");
        printf("========================================================\n\n");
        printf("\nPlease Enter the Module name:");
        fgets(singleline1,50,stdin);
        int empty_array = 0;
        for(empty_array; empty_array  < 20; empty_array++)
        {
            char_array2[empty_array] = '\0';
        }

        //printf("%s",option);
        fptr = fopen("simulate.txt","w");
        fprintf(fptr,"%s\n",singleline1);
        fclose(fptr);
        static const char filename[] = "simulate.txt";
        FILE *file = fopen(filename, "r");
        if ( file != NULL )
        {
            while ( (ch = fgetc(file)) != EOF )
            {
                if ( isspace(ch) || ispunct(ch)  )
                {
                    if ( word )
                    {

                        if (str_counter >= 0)
                        {
                            if (strcmp(char_array2,"Initialize") == 0 || strcmp(char_array2,"Simulate") == 0 ||
                                    strcmp(char_array2,"Display") == 0 ||  strcmp(char_array2,"Exit") == 0)
                            {
                                strcpy(option,char_array2);
                            }
                            else
                            {
                                cycles = cycles + atoi(char_array2);
                            }
                            for(; str_counter >= 0; --str_counter)
                            {
                                char_array2[str_counter] = '\0';
                            }
                            str_counter = 0;
                        }
                        word = 0;
                    }
                }
                else
                {
                    word = 1;
                    char_array2[str_counter] = ch;
                    str_counter = str_counter + 1;
                    //putchar(ch);
                }
            }
        }
        //printf("\n%s %d\n",option,cycles);
        if (strcmp(option,"Initialize") == 0)
        {
            //printf("\n%s\n",argv[1]);
            initialize(argv[1]);
            cycles = 0;
        }
        else if (strcmp(option,"Simulate") == 0)
        {
            strcpy(eof_flag,"N");
            simulator(cycles);
        }
        else if (strcmp(option,"Display") == 0)
        {
            display();
            //cycles = 0;
        }
        else if (strcmp(option,"Exit") == 0)
        {
            cycles = 0;
            exit(0);
        }
    }
    // initialize();
    //simulator();
    return 0;
}
