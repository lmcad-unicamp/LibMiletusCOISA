/***************************************************************************
 *   Copyright (C) 2015 by                                                 *
 *   - Carlos Eduardo Millani (carloseduardomillani@gmail.com)             *
 *   - Edson Borin (edson@ic.unicamp.br)                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*
 * Description: This file implements the virtual machine. 
 */


#include "vm.h"
#include "syscall.h"
//#include "EH.h"

#ifndef DEBUGGING
#define DEBUGGING 0
#endif

#include <stdio.h>
//#include <cstring>
#if DEBUGGING
//#include <stdio.h>
void print_memory(void);
void print_registers(void);
#endif


#include "libMiletus.h"

#if MEASURING
  #ifdef ARDUINO
  #else 
    #include <papi.h>
  #endif
#endif


#if PRINTING
//#include <stdio.h>
#endif


#if HEAP_COUNT
uint32_t max_heap = VM_MEMORY_SZ;
#endif

/*
 * Central processing unit. It fetches and executes
 * instructions.
 */

/* Register file. */
uint32_t RF[32]; //Register $zero must always be zero
float RF_float[32];
uint32_t hand_addr;
/* Floating-Point Unit condition code flags. */
uint8_t fpu_cc = 0;
/* VM memory vector. */
uint8_t VM_memory[VM_MEMORY_SZ] = {0};

uint32_t PC = 0;
uint32_t nPC = 4;
uint32_t count = 0;

uint32_t instruction_count = 0;

uint32_t comm_call_return_type = RETURN_NONE;
comm_call_return_t comm_call_return_id = { .component_name = "", .trait_name = "", .state_name = ""};

//int32_t* user_int32_buffer = NULL;
//float* user_float_buffer = NULL;
char* user_string_buffer = NULL;
uint32_t user_buffer_addr = 0;

void advance_pc(int32_t offset)
{
	PC  =  nPC;
	nPC  += offset;
}

uint32_t HI = 0, LO = 0;  
uint32_t offset = 4;
uint8_t halted = 0;

void vm_init(uint32_t newPC)
{
	PC = newPC;
	nPC = PC + 4;
	RF[0] = 0; //Register $zero must always be zero
	RF[31] = 1; //Return default (if the program does not set to zero, should put error)
	HI = 0, LO = 0;  
	offset = 4;
	halted = 0;
}

char* my_itoa(uint32_t val, uint32_t base)
{
	static char buf[32] = {0};
	int i;
	
	for (i = 30; val && i ; --i, val /= base) {
		buf[i] = "0123456789abcdef"[val % base];
	}

	return &buf[i+1];
}

int8_t vm_cpu(void)
{
	instruction_count++;
	comm_call_return_type = RETURN_NONE;

	if(halted) return 1;

	//Fetch intstructions
	uint32_t instr = fetch(PC);

	//Separate intruction to get registers, immediate, function and adress, to use depending on instruction encoding
	uint8_t op = (instr >> 26) & 0x3F;
	uint8_t rs = (instr >> 21) & 0x1F;
	uint8_t rt = (instr >> 16) & 0x1F;
	uint8_t rd = (instr >> 11) & 0x1F;
	int16_t immediate = (instr >> 0) & 0xFFFF;
	uint32_t address = (instr >> 0) & 0x3FFFFFF;

#if HEAP_COUNT
    printf("current_heap_size %u\n", RF[29]);
    uint32_t current_heap_size = RF[29];
    if (current_heap_size != 0 && current_heap_size < max_heap )
    {
    	max_heap = current_heap_size;
    }
#endif

#if DEBUGGING
	fprintf(stderr, "\n>> instr: %s\n", my_itoa(instr, 2));
	fprintf(stderr, ">> op=0x%x rs=0x%x rt=0x%x rd=0x%x immediate=0x%x address=0x%x\n", op, rs, rt, rd, immediate, address);
#endif

	offset = 4; //default offset for non-branching instructions

	switch (op) //Executes the instruction
	{
		case 0x0: { // 000000 => Register encoding.
			uint8_t shamt = (instr >> 6) & 0x1F;
			uint8_t funct = (instr >> 0) & 0x3F;

			switch (funct) {
				/*case 0b000001: { // movf rd, rs, cc (value = 0) and movt rd, rs, cc (value = 1)
#if DEBUGGING
					fprintf(stderr, "movf/movt\n");
#endif
					uint8_t cc = (instr >> 18) & 0x7;
					uint8_t value = ((instr >> 16) & 0x3) << cc;

					if ((fpu_cc & (1 << cc)) == value) {
						RF[rd] = RF[rs];
					}
					break;
				}*/
				case 0b001011: { // movn	001011 				if(rt!=0) rd = rs
#if DEBUGGING
					fprintf(stderr, "movn\n");
#endif
					if (RF[rt] != 0) RF[rd] = RF[rs];
					break;
				}
				case 0b001010: { // movz	001010 				if(rt==0) rd = rs
#if DEBUGGING
					fprintf(stderr, "movz\n");
#endif
					if (RF[rt] == 0) RF[rd] = RF[rs];
					break;
				}
				case 0b100000: { // add		100000	ArithLog	$d = $s + $t
#if DEBUGGING
					fprintf(stderr, "add\n");
#endif
					RF[rd] = RF[rs] + RF[rt];
					break;
				}
				case 0b100001: { // addu	100001	ArithLog	$d = $s + $t
#if DEBUGGING
					fprintf(stderr, "addu\n");
#endif
					RF[rd] = RF[rs] + RF[rt];
					break;
				}
				case 0b100100: { // and		100100	ArithLog	$d = $s & $t
#if DEBUGGING
					fprintf(stderr, "and\n");
#endif
					RF[rd] = RF[rs] & RF[rt];
					break;
				}
				case 0b011010: { // div		011010  DivMult		lo = $s / $t; hi = $s % $t
#if DEBUGGING
					fprintf(stderr, "div\n");
#endif
					LO = RF[rs] / RF[rt];
					HI = RF[rs] % RF[rt];
					break;
				}
				case 0b011011: { // divu	011011  DivMult		lo = $s / $t; hi = $s % $t
#if DEBUGGING
					fprintf(stderr, "divu\n");
#endif
					LO = RF[rs] / RF[rt];
					HI = RF[rs] % RF[rt];
					break;
				}
				case 0b011000: { // mult	011000  DivMult		hi:lo = $s * $t
#if DEBUGGING
					fprintf(stderr, "mult\n");
#endif
					uint64_t mult = (uint64_t)RF[rs] * (uint64_t) RF[rt];
					HI = (mult >> 32) & 0xFFFFFFFF;
					LO = mult & 0xFFFFFFFF;
					break;
				}
				case 0b011001: { // multu	011001	DivMult		hi:lo = $s * $t
#if DEBUGGING
					fprintf(stderr, "multu\n");
#endif
					uint64_t mult = (uint64_t)RF[rs] * (uint64_t) RF[rt];
					HI = (mult >> 32) & 0xFFFFFFFF;
					LO = mult & 0xFFFFFFFF;
					break;
				}
				case 0b100111: { // nor		100111	ArithLog	$d = ~($s | $t)
#if DEBUGGING
					fprintf(stderr, "nor\n");
#endif
					RF[rd] = ~(RF[rs] | RF[rt]);
					break;
				}
				case 0b100101: { // or		100101	ArithLog	$d = $s | $t
#if DEBUGGING
					fprintf(stderr, "or\n");
#endif
					RF[rd] = RF[rs] | RF[rt];
					break;
				}
				case 0b000000: { // sll		000000	Shift		$d = $t << a
#if DEBUGGING
					fprintf(stderr, "sll\n");
#endif
					RF[rd] = RF[rt] << shamt;
					break;
				}
				case 0b000100: { // sllv	000100	ShiftV		$d = $t << $s
#if DEBUGGING
					fprintf(stderr, "sllv\n");
#endif
					RF[rd] = RF[rt] << RF[rs];
					break;
				}
				case 0b000011: { // sra		000011	Shift		$d = $t >> a 
#if DEBUGGING
					fprintf(stderr, "sra\n");
#endif
					if ((int32_t)RF[rt] < 0 && shamt > 0)
				        RF[rd] = (int32_t)RF[rt] | ~(~0U >> shamt);
				    else
				        RF[rd] = (int32_t)RF[rt] >> shamt;
					break;
				}
				case 0b000111: { // srav	000111	ShiftV		$d = $t >> $s
#if DEBUGGING
					fprintf(stderr, "srav\n");
#endif
					RF[rd] = RF[rt] >> RF[rs];
					break;
				}
				case 0b000010: { // srl		000010	Shift		$d = $t >>> a
#if DEBUGGING
					fprintf(stderr, "srl\n");
#endif
					RF[rd] = RF[rt] >> shamt;
					break;
				}
				case 0b000110: { // srlv	000110	ShiftV		$d = $t >>> $s
#if DEBUGGING
					fprintf(stderr, "srlv\n");
#endif
					RF[rd] = RF[rt] >> RF[rs];  
					break;
				}
				case 0b100010: { // sub		100010	ArithLog	$d = $s - $t
#if DEBUGGING
					fprintf(stderr, "sub\n");
#endif
					RF[rd] = RF[rs] - RF[rt];  
					break;
				}
				case 0b100011: { // subu	100011	ArithLog	$d = $s - $t
#if DEBUGGING
					fprintf(stderr, "subu\n");
#endif
					RF[rd] = RF[rs] - RF[rt];    
					break;
				}
				case 0b100110: { // xor		100110  ArithLog	$d = $s ^ $t
#if DEBUGGING
					fprintf(stderr, "xor\n");
#endif
					RF[rd] = RF[rs] ^ RF[rt];    
					break;
				}
				case 0b101010: { // slt		101010	ArithLog	$d = ($s < $t)
#if DEBUGGING
					fprintf(stderr, "slt\n");
#endif
					RF[rd] = (RF[rs] < RF[rt])?1:0;
					break;
				}
				case 0b101011: { // sltu	101011	ArithLog	$d = ($s < $t)
#if DEBUGGING
					fprintf(stderr, "sltu\n");
#endif
					RF[rd] = (RF[rs] < RF[rt])?1:0;
					break;
				}
				case 0b001001: { // jalr	001001	JumpR		$31 = pc; pc = $s
#if DEBUGGING
					fprintf(stderr, "jalr\n");
#endif
					RF[31] = PC+8;
					PC = nPC;
					nPC = RF[rs];
					return 0;
				}
				case 0b001000: { // jr		001000	JumpR		pc = $s 
#if DEBUGGING
					fprintf(stderr, "jr\n");
#endif
					PC = nPC;
					nPC = RF[rs];
					return 0;
				}
				case 0b010000: { // mfhi	010000	MoveFrom	$d = hi
#if DEBUGGING
					fprintf(stderr, "mfhi\n");
#endif
					RF[rd] = HI;
					break;
				}
				case 0b010010: { // mflo	010010	MoveFrom	$d = lo
#if DEBUGGING
					fprintf(stderr, "mflo\n");
#endif
					RF[rd] = LO;
					break;
				}
				case 0b010001: { // mthi	010001	MoveTo		hi = $s
#if DEBUGGING
					fprintf(stderr, "mthi\n");
#endif
					HI = RF[rs];
					break;
				}
				case 0b010011: { // mtlo	010011	MoveTo		lo = $s
#if DEBUGGING
					fprintf(stderr, "mtlo\n");
#endif
					LO = RF[rs];
					break;
				}
				case 0b001100: { // syscall 	001100	syscall		$2		seen on the .s generated by the ecc compiler
#if DEBUGGING
					fprintf(stderr, "syscall\n");
#endif
#if MEASURING
	#ifdef ARDUINO
					uint64_t local_vm_cycle_count_start = ESP.getCycleCount();
	#else 
					long long PAPIValues[2] = {0, 0};
					//PAPI_read_counters(PAPIValues, 2);
					uint64_t local_vm_cycle_count_start = PAPIValues[0];
	#endif
#endif
					if (syscall((uint8_t)RF[2])) //registers $4 and $5 used by hal_call (sensid and retval respectively)
					{
						halted = 1; //Syscall returned 1, exit code
					}
#if MEASURING
	#ifdef ARDUINO
					uint64_t local_vm_cycle_count_end = ESP.getCycleCount();
	#else 
					//PAPI_read_counters(PAPIValues, 2);
					uint64_t local_vm_cycle_count_end = PAPIValues[0];
	#endif
					global_vm_cycle_count_without_syscall -= local_vm_cycle_count_end - local_vm_cycle_count_start;
#endif
					break; 
				}	
				default: {
					//TODO: throw an error
					printf(">> Unimplemented instruction (op=0x%x, rs=0x%x, rt=0x%x, rd=0x%x, immediate=0x%x, PC: 0x%x).\n", op, rs, rt, rd, immediate, PC);
					return -1;
				}
			}      
			break; // case 0x0
		}
		
		//Immediate encoding
		case 0b001000: { //addi    001000  ArithLogI       $t = $s + SE(i)
#if DEBUGGING
			fprintf(stderr, "addi\n");
#endif
			RF[rt] = RF[rs] + immediate; //Implementar trap!
			break;
		}
		case 0b001001: { //addiu   001001  ArithLogI       $t = $s + SE(i)
#if DEBUGGING
			fprintf(stderr, "addiu\n");
#endif
			RF[rt] = (RF[rs] + immediate);
			break;
		}
		case 0b001100: { //andi    001100  ArithLogI       $t = $s & ZE(i)
#if DEBUGGING
			fprintf(stderr, "andi\n");
#endif
		    RF[rt] = RF[rs] & (uint32_t)immediate;
			break;
		}
		case 0b001101: { //ori     001101  ArithLogI       $t = $s | ZE(i)
#if DEBUGGING
			fprintf(stderr, "ori\n");
#endif
			//RF[rt] = RF[rs] | (uint32_t)immediate;
			RF[rt] = RF[rs] | (((uint32_t)immediate) & 0xFFFF);
			break;
		}
		case 0b001110: { //xori    001110  ArithLogI       $d = $s ^ ZE(i)
#if DEBUGGING
			fprintf(stderr, "xori\n");
#endif
			RF[rd] = RF[rs] ^ (uint32_t)immediate;
			break;
		}
		case 0b001111: 	 //lui	   001111          Rdest, imm: Load Upper Immediate
		case 0b011001: { //lhi     011001  LoadI   HH ($t) = i 
		  	//RF[rt] = ((uint32_t)immediate) << 16;
#if DEBUGGING
			fprintf(stderr, "lui/lhi\n");
#endif
		  	RF[rt] = (((uint32_t)immediate) & 0xFFFF) << 16;
			break;
		}
		case 0b011000: { //llo     011000  LoadI   LH ($t) = i
#if DEBUGGING
			fprintf(stderr, "llo\n");
#endif
		  	//RF[rt] = immediate;
		  	RF[rt] = ((uint32_t)immediate) & 0xFFFF;
			break;
		}
		case 0b001010: { //slti    001010  ArithLogI       $t = ($s < SE(i))
#if DEBUGGING
			fprintf(stderr, "slti\n");
#endif
		  	RF[rt] = ((signed)RF[rs] < immediate)?1:0;
			break;
		}
		case 0b001011: { //sltiu   001011  ArithLogI       $t = ($s < SE(i)) 
#if DEBUGGING
			fprintf(stderr, "sltiu\n");
#endif
			RF[rt] = (RF[rs] < (unsigned)immediate)?1:0;
			break;
		}
		case 0b000001: {
			if (RF[rt] == 0b00001) //bgez Rsrc, offset: Branch on Greater Than Equal Zero
			{
#if DEBUGGING
			fprintf(stderr, "bgez\n");
#endif
				if ((int32_t)RF[rs] >= 0)
				{
					advance_pc(immediate << 2);
					return 0;
				}
				else
				{
					break;
				}
			}
			else if (RF[rt] == 0b10001) //bgezal Rsrc, offset: Branch on Greater Than Equal Zero And Link
			{
#if DEBUGGING
				fprintf(stderr, "bgezal\n");
#endif
				if ((int32_t)RF[rs] >= 0)
				{
					RF[31] = PC + 8;
					advance_pc(immediate << 2);
					return 0;
				}
				else
				{
					break;
				}
			}
			else if (RF[rt] == 0b00000) //bltz Rsrc, offset: Branch on Less Than Zero
			{
#if DEBUGGING
				fprintf(stderr, "bltz\n");
#endif
				if ((int32_t)RF[rs] < 0)
				{
					advance_pc(immediate << 2);
					return 0;
				}
				else
				{
					break;
				}
			}
			else if (RF[rt] == 0b10000) //bltzal Rsrc, offset: Branch on Less Than And Link
			{
#if DEBUGGING
				fprintf(stderr, "bltzal\n");
#endif
				if ((int32_t)RF[rs] < 0)
				{
					RF[31] = PC + 8;
					advance_pc(immediate << 2);
					return 0;
				}
				else
				{
					break;
				}
			}
			break;
		}
		case 0b000100: { //beq     000100  Branch  if ($s == $t) pc += i << 2
#if DEBUGGING
			fprintf(stderr, "beq\n");
#endif
			if (RF[rs] == RF[rt])
			{
				advance_pc(immediate << 2);
				return 0;
			}
			else
			{
				break;
			}
			break;
		}
		case 0b000111: { //bgtz    000111  BranchZ if ($s > 0) pc += i << 2
#if DEBUGGING
			fprintf(stderr, "bgtz\n");
#endif
			if ((int32_t)RF[rs] > 0)
			{
				advance_pc(immediate << 2);
				return 0;
			}
			else
			{
				break;
			}
			break;
		}
		case 0b000110: { //blez    000110  BranchZ if ($s <= 0) pc += i << 2
#if DEBUGGING
			fprintf(stderr, "blez\n");
#endif
			if ((int32_t)RF[rs] <= 0)
			{
				advance_pc(immediate << 2);
				return 0;
			}
			else
			{
				break;
			}
			break;
		}
		case 0b000101: { //bne     000101  Branch  if ($s != $t) pc += i << 2
#if DEBUGGING
			fprintf(stderr, "bne\n");
			fprintf(stderr, ">>RA:%x\tAddress:%x\n", RF[31], immediate<<2);
#endif
			if (RF[rs] != RF[rt])
			{
				advance_pc(immediate << 2);
				return 0;
			}
			else
			{
				break;
			}
			break;
		}
		case 0b100000: { //lb      100000  LoadStore       $t = SE (MEM [$s + i]:1)
#if DEBUGGING
			fprintf(stderr, "lb\n");
#endif
		  	RF[rt] = ((uint32_t)VM_memory[RF[rs] + immediate] & 0x7F)  | (uint32_t)(VM_memory[RF[rs] + immediate] & 0x80)<<24; //Load byte carrying signal to the register
			break;
		}
		case 0b100100: { //lbu     100100  LoadStore       $t = ZE (MEM [$s + i]:1)
#if DEBUGGING
			fprintf(stderr, "lbu\n");
#endif
		  	RF[rt] = ((uint32_t)VM_memory[RF[rs] + immediate]) & 0xFF;
			break;
		}
		case 0b100001: { //lh      100001  LoadStore       $t = SE (MEM [$s + i]:2)
#if DEBUGGING
			fprintf(stderr, "lh\n");
#endif
		  	RF[rt] = ((((uint32_t)(VM_memory[RF[rs] + immediate])<< 16) | ((uint32_t)VM_memory[RF[rs] + immediate + 1])) & 0x7FFF) | (uint32_t)((VM_memory[RF[rs] + immediate + 1]) & 0x8000) << 16;
			break;
		}
		case 0b100101: { //lhu     100101  LoadStore       $t = ZE (MEM [$s + i]:2)
#if DEBUGGING
			fprintf(stderr, "lhu\n");
#endif
			RF[rt] = ((((uint32_t)(VM_memory[RF[rs] + immediate])<<16) | ((uint32_t)VM_memory[RF[rs] + immediate + 1])) & 0xFFFF);
			break;
		}
		case 0b100011: { //lw      100011  LoadStore       $t = MEM [$s + i]:4
#if DEBUGGING
			fprintf(stderr, "lw\n");
#endif
		  	RF[rt] = ((uint32_t)(VM_memory[RF[rs] + immediate + 0]) << 24) |
		  			 ((uint32_t)(VM_memory[RF[rs] + immediate + 1]) << 16) |
		  			 ((uint32_t)(VM_memory[RF[rs] + immediate + 2]) <<  8) |
		  			 ((uint32_t)(VM_memory[RF[rs] + immediate + 3]) <<  0);
			break;
		}
		case 0b101000: { //sb      101000  LoadStore       MEM [$s + i]:1 = LB ($t)
#if DEBUGGING
			fprintf(stderr, "sb\n");
#endif
		  	VM_memory[RF[rs] + immediate] = (uint8_t)(RF[rt] & 0xFF);
			break;
		}
		case 0b101001: { //sh      101001  LoadStore       MEM [$s + i]:2 = LH ($t)
#if DEBUGGING
			fprintf(stderr, "sh\n");
#endif
		  	VM_memory[RF[rs] + immediate] = (uint8_t)((RF[rt] & 0xFF00) >> 8);
			VM_memory[RF[rs] + immediate + 1] = (uint8_t)(RF[rt] & 0xFF);
			break;
		}
		case 0b101011: { //sw      101011  LoadStore       MEM [$s + i]:4 = $t
#if DEBUGGING
			fprintf(stderr, "sw\n");
#endif
		  	VM_memory[RF[rs] + immediate + 0] = (uint8_t)((RF[rt] & 0xFF000000) >> 24);
			VM_memory[RF[rs] + immediate + 1] = (uint8_t)((RF[rt] & 0x00FF0000) >> 16);
		  	VM_memory[RF[rs] + immediate + 2] = (uint8_t)((RF[rt] & 0x0000FF00) >> 8);
			VM_memory[RF[rs] + immediate + 3] = (uint8_t)((RF[rt] & 0x000000FF) >> 0);
			break;
		}

		//Jump encoding
		
		case 0b000010: { //j       000010  Jump    pc = i << 2
#if DEBUGGING
			fprintf(stderr, "j\n");
#endif
			PC = nPC;
			nPC = address << 2;
			return 0;
		}
		case 0b000011: { //jal     000011  Jump    $31 = pc; pc = i << 2
#if DEBUGGING
			fprintf(stderr, "jal\n");
#endif
			RF[31] = PC + 8;
			PC = nPC;
			nPC = address << 2;
			return 0;
		}
		case 0b011010: { //trap    011010  Trap    Dependent on operating system; different values for immed26 specify different operations. See the list of traps for information on what the different trap codes do.
#if DEBUGGING
			fprintf(stderr, "trap\n");
#endif
			syscall((uint8_t)(address&0xFF));
			break;
		}

		case 0b101010: { //swl Rsrc1, imm(Rsrc2): Store Word Left
#if DEBUGGING
			fprintf(stderr, "swl\n");
#endif
			VM_memory[RF[rs] + immediate] = (uint8_t)((RF[rt] & 0xFF000000) >> 24);
			VM_memory[RF[rs] + immediate + 1] = (uint8_t)((RF[rt] & 0xFF0000) >> 16);
			break;
		}
		case 0b101110: { //swr Rsrc1, imm(Rsrc2): Store Word Right
#if DEBUGGING
			fprintf(stderr, "swr\n");
#endif
		  	VM_memory[RF[rs] + immediate] = (uint8_t)(RF[rt] & 0xFF);
			VM_memory[RF[rs] + immediate - 1] = (uint8_t)(RF[rt] & 0xFF00) >> 8;
			break;
		}
		case 0b100010: { //lwl Rdest, imm(Rsrc): Load Word Left
#if DEBUGGING
			fprintf(stderr, "lwl\n");
#endif
			RF[rt] = ((uint32_t)(VM_memory[RF[rs] + immediate]) << 24) | ((uint32_t)(VM_memory[RF[rs] + immediate + 1]) << 16);
			break;
		}
		case 0b100110: { //lwr Rdest, imm(Rsrc): Load Word Right
#if DEBUGGING
			fprintf(stderr, "lwr\n");
#endif
			RF[rt] = ((uint32_t)(VM_memory[RF[rs] + immediate -1]) << 8) | ((uint32_t)VM_memory[RF[rs] + immediate]);
			break;
		}

		// ------------------------ FLOATING POINT INSTRUCTIONS ------------------------

		case 0x11: {
			uint8_t ft = (instr >> 16) & 0x1F;
			uint8_t fs = (instr >> 11) & 0x1F;
			uint8_t fd = (instr >>  6) & 0x1F;
			uint8_t funct = (instr >>  0) & 0x3F;

			switch (rs) {
				case 0x0: { // mfc1 rt, fs
#if DEBUGGING
					fprintf(stderr, "mfc1\n");
#endif
					number_type num;
					num.as_float = RF_float[fs];
					RF[rt] = num.as_int32;
					break;
				}
				case 0x4: { // mtc1 rt, fs
#if DEBUGGING
					fprintf(stderr, "mtc1\n");
#endif
					number_type num;
					num.as_int32 = RF[rt];
					RF_float[fs] = num.as_float;
					break;
				}
				/*case 0x7: { // mthc1 rt, fs
#if DEBUGGING
					fprintf(stderr, "mthc1\n");
#endif
					number_type num_hi;
					num_hi.as_int32 = RF[rt];
					RF_float[fs] = num_hi.as_float;
					break;
				}*/
				case 0x8: { // bc1f cc label (value = 0) and bc1t cc label (value = 1)
#if DEBUGGING
					fprintf(stderr, "bc1f\n");
#endif
					uint8_t cc = (instr >> 18) & 0x7;
					uint8_t value = ((instr >> 16) & 0x3) << cc;

					if ((fpu_cc & (1 << cc)) == value) {
						advance_pc(immediate << 2);
						return 0;
					}

					break;
				}
				case 0x10: {
					switch (funct) {
						case 0x0: { // add.s fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "add.s\n");
#endif
							RF_float[fd] = RF_float[fs] + RF_float[ft];
							break;
						}
						case 0x1: { // sub.s fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "sub.s\n");
#endif
							RF_float[fd] = RF_float[fs] - RF_float[ft];
							break;
						}
						case 0x2: { // mul.s fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "mul.s\n");
#endif
							RF_float[fd] = RF_float[fs] * RF_float[ft];
							break;
						}
						case 0x3: { // div.s fd, fs, ft 
#if DEBUGGING
							fprintf(stderr, "div.s\n");
#endif
							RF_float[fd] = RF_float[fs] / RF_float[ft];
							break;
						}
						case 0x6: { // mov.s fd, fs
#if DEBUGGING
							fprintf(stderr, "mov.s\n");
#endif
							RF_float[fd] = RF_float[fs];
							break;
						}
						case 0x7: { // neg.s fd, fs
#if DEBUGGING
							fprintf(stderr, "neg.s\n");
#endif
							RF_float[fd] = - RF_float[fs];
							break;
						}
						case 0xd: { // trunc.w.s fd, fs
#if DEBUGGING
							fprintf(stderr, "truc.w.s\n");
#endif
							RF_float[fd] = (int32_t) RF_float[fs];
							break;
						}
						case 0x13: { // movn.s fd, fs, rt
#if DEBUGGING
							fprintf(stderr, "movn.s\n");
#endif
							if (RF[rt]) {
								RF_float[fd] = RF_float[fs];
							}
							break;
						}
						case 0x21: { // cvt.d.s fd, fs
#if DEBUGGING
							fprintf(stderr, "cvt.d.s\n");
#endif
							double_number_type num_fd;

							num_fd.as_double = (double) RF_float[fs];

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}
						case 0x32: { // c.eq.s cc fs, ft (FC (bits [5-4]) = 0b11 and bits [3-0] = 0x2)
#if DEBUGGING
							fprintf(stderr, "c.eq.s\n");
#endif
							uint8_t cc = (instr >> 8) & 0x7;
							uint8_t mask = 1;

							if (RF_float[fs] == RF_float[ft] && !(RF_float[fs] != RF_float[fs] || RF_float[ft] != RF_float[ft])) {
								fpu_cc |= (mask << cc);
							} else {
								fpu_cc &= ~(mask << cc);
							}

							break;
						}
						case 0x34: { // c.olt.s cc fs, ft (FC (bits [5-4]) = 0b11 and bits [3-0] = 0x4)
#if DEBUGGING
							fprintf(stderr, "c.olt.s\n");
#endif
							uint8_t cc = (instr >> 8) & 0x7;
							uint8_t mask = 1;

							if (RF_float[fs] < RF_float[ft] && !(RF_float[fs] != RF_float[fs] || RF_float[ft] != RF_float[ft])) {
								fpu_cc |= (mask << cc);
							} else {
								fpu_cc &= ~(mask << cc);
							}

							break;
						}
						case 0x37: { // c.ule.s cc fs, ft (FC (bits [5-4]) = 0b11 and bits [3-0] = 0x7)
#if DEBUGGING
							fprintf(stderr, "c.ule.s\n");
#endif
							uint8_t cc = (instr >> 8) & 0x7;
							uint8_t mask = 1;

							if (RF_float[fs] <= RF_float[ft]) {
								fpu_cc |= (mask << cc);
							} else {
								fpu_cc &= ~(mask << cc);
							}

							break;
						}
						default: {
							printf(">> Unimplemented instruction (op=0x%x, rs=0x%x, ft=0x%x, fs=0x%x, fd=0x%x, funct=0x%x, PC: 0x%x).\n", op, rs, ft, fs, fd, funct, PC);
							return -1;
						}
					}
					break;
				}
				case 0x11:{
					switch (funct) {
						case 0x0: { // add.d fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "add.d\n");
#endif
							double_number_type num_fs, num_ft, num_fd;
							number_type tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							tmp_ft0.as_float = RF_float[ft + 0];
							tmp_ft1.as_float = RF_float[ft + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
											  (((uint64_t) tmp_ft1.as_int32) <<  0);

							num_fd.as_double = num_fs.as_double + num_ft.as_double;

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}
						case 0x1: { // sub.d fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "sub.d\n");
#endif
							double_number_type num_fs, num_ft, num_fd;
							number_type tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							tmp_ft0.as_float = RF_float[ft + 0];
							tmp_ft1.as_float = RF_float[ft + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
											  (((uint64_t) tmp_ft1.as_int32) <<  0);

							num_fd.as_double = num_fs.as_double - num_ft.as_double;

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}
						case 0x2: { // mul.d fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "mul.d\n");
#endif
							double_number_type num_fs, num_ft, num_fd;
							number_type tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							tmp_ft0.as_float = RF_float[ft + 0];
							tmp_ft1.as_float = RF_float[ft + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
											  (((uint64_t) tmp_ft1.as_int32) <<  0);

							num_fd.as_double = num_fs.as_double * num_ft.as_double;

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}
						/*case 0x3: { // div.d fd, fs, ft
#if DEBUGGING
							fprintf(stderr, "div.d\n");
#endif
							double_number_type num_fs, num_ft, num_fd;
							number_type tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							tmp_ft0.as_float = RF_float[ft + 0];
							tmp_ft1.as_float = RF_float[ft + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
											  (((uint64_t) tmp_ft1.as_int32) <<  0);

							num_fd.as_double = num_fs.as_double / num_ft.as_double;

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}*/
/*						case 0x6: { // mov.d fd, fs
#if DEBUGGING
							fprintf(stderr, "mov.d\n");
#endif
							RF_float[fd + 0] = RF_float[fs + 0];
							RF_float[fd + 1] = RF_float[fs + 1];
							break;
						}*/
						case 0x7: { // neg.d fd, fs
#if DEBUGGING
							fprintf(stderr, "neg.d\n");
#endif
							double_number_type num_fs, num_fd;
							number_type tmp_fs0, tmp_fs1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							num_fd.as_double = - num_fs.as_double;

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}
						/*case 0xd: { // trunc.w.d fd, fs
#if DEBUGGING
							fprintf(stderr, "trunc.w.d\n");
#endif
							double_number_type num_fs, num_fd;
							number_type tmp_fs0, tmp_fs1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							num_fd.as_int64 = (int64_t) num_fs.as_double;

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}*/
						case 0x11: { // movf.d fd, fs (value = 0) and movt.d fd, fs (value = 1)
#if DEBUGGING
							fprintf(stderr, "movf.d/movt.d\n");
#endif
							uint8_t cc = (instr >> 18) & 0x7;
							uint8_t value = ((instr >> 16) & 0x3) << cc;

							if ((fpu_cc & (1 << cc)) == value) {
								RF_float[fd + 0] = RF_float[fs + 0];
								RF_float[fd + 1] = RF_float[fs + 1];
							}

							break;
						}
/*						case 0x12: { // movz.d fd, fs, rt
#if DEBUGGING
							fprintf(stderr, "movz.d\n");
#endif
							if (RF[rt] == 0) {
								RF_float[fd + 0] = RF_float[fs + 0];
								RF_float[fd + 1] = RF_float[fs + 1];
							}
							break;
						}*/
						case 0x20: { // cvt.s.d fd, fs
#if DEBUGGING
							fprintf(stderr, "cvt.s.d\n");
#endif
							double_number_type num_fs;
							number_type tmp_fs0, tmp_fs1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							RF_float[fd] = (float) num_fs.as_double;

							break;
						}
						/*case 0x32: { // c.eq.d cc fs, ft (FC (bits [5-4]) = 0b11 and bits [3-0] = 0x2)
#if DEBUGGING
							fprintf(stderr, "c.eq.d\n");
#endif
							double_number_type num_fs, num_ft;
							number_type tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							tmp_ft0.as_float = RF_float[ft + 0];
							tmp_ft1.as_float = RF_float[ft + 1];

							num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
											  (((uint64_t) tmp_ft1.as_int32) <<  0);


							uint8_t cc = (instr >> 8) & 0x7;
							uint8_t mask = 1;

							if (num_fs.as_double == num_ft.as_double &&
								!(num_fs.as_double != num_fs.as_double || num_ft.as_double != num_ft.as_double)) {
								fpu_cc |= (mask << cc);
							} else {
								fpu_cc &= ~(mask << cc);
							}

							break;
						}*/
						/*case 0x34: { // c.olt.d cc fs, ft (FC (bits [5-4]) = 0b11 and bits [3-0] = 0x4)
#if DEBUGGING
							fprintf(stderr, "c.olt.d\n");
#endif
							double_number_type num_fs, num_ft;
							number_type tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

							tmp_fs0.as_float = RF_float[fs + 0];
							tmp_fs1.as_float = RF_float[fs + 1];

							num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
											  (((uint64_t) tmp_fs1.as_int32) <<  0);

							tmp_ft0.as_float = RF_float[ft + 0];
							tmp_ft1.as_float = RF_float[ft + 1];

							num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
											  (((uint64_t) tmp_ft1.as_int32) <<  0);


							uint8_t cc = (instr >> 8) & 0x7;
							uint8_t mask = 1;

							if (num_fs.as_double < num_ft.as_double &&
								!(num_fs.as_double != num_fs.as_double || num_ft.as_double != num_ft.as_double)) {
								fpu_cc |= (mask << cc);
							} else {
								fpu_cc &= ~(mask << cc);
							}

							break;
						}*/
						default: {
							printf(">> Unimplemented instruction (op=0x%x, rs=0x%x, ft=0x%x, fs=0x%x, fd=0x%x, funct=0x%x, PC: 0x%x).\n", op, rs, ft, fs, fd, funct, PC);
							return -1;
						}
					}
					break;
				}
				case 0x14: {
					switch (funct) {
						case 0x20: { // cvt.s.w fd, fs
#if DEBUGGING
							fprintf(stderr, "cvt.s.w\n");
#endif
							number_type num;

							num.as_float = RF_float[fs];
							RF_float[fd] = (float) num.as_int32;

							break;
						}
						/*case 0x21: { // cvt.d.w fd, fs
#if DEBUGGING
							fprintf(stderr, "cvt.d.w\n");
#endif
							double_number_type num_fd;

							num_fd.as_int64 = (uint64_t) RF[fs];

							number_type num_hi, num_lo;
							num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
							num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

							RF_float[fd + 0] = num_hi.as_float;
							RF_float[fd + 1] = num_lo.as_float;

							break;
						}*/
						default: {
							printf(">> Unimplemented instruction (op=0x%x, rs=0x%x, ft=0x%x, fs=0x%x, fd=0x%x, funct=0x%x, PC: 0x%x).\n", op, rs, ft, fs, fd, funct, PC);
							return -1;
						}
					}
					break;
				}
				default: {
					printf(">> Unimplemented instruction (op=0x%x, rs=0x%x, rt=0x%x, rd=0x%x, immediate=0x%x, PC: 0x%x).\n", op, rs, rt, rd, immediate, PC);
					return -1;
				}
			}
			break;
		}
		case 0x13: {
			uint8_t fr = (instr >> 21) & 0x1F;
			uint8_t ft = (instr >> 16) & 0x1F;
			uint8_t fs = (instr >> 11) & 0x1F;
			uint8_t fd = (instr >>  6) & 0x1F;
			uint8_t funct = (instr >>  0) & 0x3F;

			switch (funct) {
				case 0x0: { // lwxc1 fd, index(base) (index = ft, base = fr)
#if DEBUGGING
					fprintf(stderr, "lwxc1\n");
#endif
					number_type num;

					num.as_int32 = ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 0]) << 24) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 1]) << 16) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 2]) <<  8) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 3]) <<  0);

					RF_float[fd] = num.as_float;

					break;
				}
				case 0x1: { // ldxc1 fd, index(base) (index = ft, base = fr)
#if DEBUGGING
					fprintf(stderr, "ldxc1\n");
#endif
					number_type num;

					num.as_int32 = ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 0]) << 24) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 1]) << 16) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 2]) <<  8) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 3]) <<  0);

					RF_float[fd] = num.as_float;

					num.as_int32 = ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 4]) << 24) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 5]) << 16) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 6]) <<  8) |
								   ((uint32_t)(VM_memory[RF[fr] + RF[ft] + 7]) <<  0);

					RF_float[fd+ 1] = num.as_float;

					break;
				}
				case 0x8: { // swxc1 fs, index(base) (index = ft, base = fr)
#if DEBUGGING
					fprintf(stderr, "swxc1\n");
#endif
					number_type num;

					num.as_float = RF_float[fs];

					VM_memory[RF[fr] + RF[ft] + 0] = (uint8_t) ((num.as_int32 >> 24) & 0xFF);
					VM_memory[RF[fr] + RF[ft] + 1] = (uint8_t) ((num.as_int32 >> 16) & 0xFF);
					VM_memory[RF[fr] + RF[ft] + 2] = (uint8_t) ((num.as_int32 >>  8) & 0xFF);
					VM_memory[RF[fr] + RF[ft] + 3] = (uint8_t) ((num.as_int32 >>  0) & 0xFF);

					break;
				}
				case 0x20: { // madd.s fd, fr, fs, ft
#if DEBUGGING
					fprintf(stderr, "madd.s\n");
#endif
					RF_float[fd] = (RF_float[fs] * RF_float[ft]) + RF_float[fr];
					break;
				}
				case 0x21: { // madd.d fd, fr, fs, ft
#if DEBUGGING
					fprintf(stderr, "madd.d\n");
#endif
					double_number_type num_fr, num_fs, num_ft, num_fd;
					number_type tmp_fr0, tmp_fr1, tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

					tmp_fr0.as_float = RF_float[fr + 0];
					tmp_fr1.as_float = RF_float[fr + 1];

					tmp_fs0.as_float = RF_float[fs + 0];
					tmp_fs1.as_float = RF_float[fs + 1];

					tmp_ft0.as_float = RF_float[ft + 0];
					tmp_ft1.as_float = RF_float[ft + 1];

					num_fr.as_int64 = (((uint64_t) tmp_fr0.as_int32) << 32) |
									  (((uint64_t) tmp_fr1.as_int32) <<  0);

					num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
									  (((uint64_t) tmp_fs1.as_int32) <<  0);

					num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
									  (((uint64_t) tmp_ft1.as_int32) <<  0);

					num_fd.as_double = (num_fs.as_double * num_ft.as_double) + num_fr.as_double;

					number_type num_hi, num_lo;
					num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
					num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

					RF_float[fd + 0] = num_hi.as_float;
					RF_float[fd + 1] = num_lo.as_float;

					break;
				}
				case 0x28: { // msub.s fd, fr, fs, ft
#if DEBUGGING
					fprintf(stderr, "msub.s\n");
#endif
					RF_float[fd] = (RF_float[fs] * RF_float[ft]) - RF_float[fr];
					break;
				}
				case 0x29: { // msub.d fd, fr, fs, ft
#if DEBUGGING
					fprintf(stderr, "msub.d\n");
#endif
					double_number_type num_fr, num_fs, num_ft, num_fd;
					number_type tmp_fr0, tmp_fr1, tmp_fs0, tmp_fs1, tmp_ft0, tmp_ft1;

					tmp_fr0.as_float = RF_float[fr + 0];
					tmp_fr1.as_float = RF_float[fr + 1];

					tmp_fs0.as_float = RF_float[fs + 0];
					tmp_fs1.as_float = RF_float[fs + 1];

					tmp_ft0.as_float = RF_float[ft + 0];
					tmp_ft1.as_float = RF_float[ft + 1];

					num_fr.as_int64 = (((uint64_t) tmp_fr0.as_int32) << 32) |
									  (((uint64_t) tmp_fr1.as_int32) <<  0);

					num_fs.as_int64 = (((uint64_t) tmp_fs0.as_int32) << 32) |
									  (((uint64_t) tmp_fs1.as_int32) <<  0);

					num_ft.as_int64 = (((uint64_t) tmp_ft0.as_int32) << 32) |
									  (((uint64_t) tmp_ft1.as_int32) <<  0);

					num_fd.as_double = (num_fs.as_double * num_ft.as_double) - num_fr.as_double;

					number_type num_hi, num_lo;
					num_hi.as_int32 = ((num_fd.as_int64 >> 32) & 0xFFFFFFFF);
					num_lo.as_int32 = ((num_fd.as_int64 >>  0) & 0xFFFFFFFF);

					RF_float[fd + 0] = num_hi.as_float;
					RF_float[fd + 1] = num_lo.as_float;

					break;
				}
				default: {
					printf(">> Unimplemented instruction (op=0x%x, fr=0x%x, ft=0x%x, fs=0x%x, fd=0x%x, funct=0x%x, PC: 0x%x).\n", op, fr, ft, fs, fd, funct, PC);
					return -1;
				}
			}

			break;
		}
		case 0x31: { // lwc1 ft, address
#if DEBUGGING
			fprintf(stderr, "lwc1\n");
#endif
			number_type num;

			num.as_int32 = ((uint32_t)(VM_memory[RF[rs] + immediate + 0]) << 24) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 1]) << 16) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 2]) <<  8) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 3]) <<  0);

			RF_float[rt] = num.as_float;

			break;
		}
		case 0x35: { // ldc1 ft, address
#if DEBUGGING
			fprintf(stderr, "ldc1\n");
#endif
			number_type num;

			num.as_int32 = ((uint32_t)(VM_memory[RF[rs] + immediate + 0]) << 24) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 1]) << 16) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 2]) <<  8) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 3]) <<  0);

			RF_float[rt] = num.as_float;

			num.as_int32 = ((uint32_t)(VM_memory[RF[rs] + immediate + 4]) << 24) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 5]) << 16) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 6]) <<  8) |
						   ((uint32_t)(VM_memory[RF[rs] + immediate + 7]) <<  0);

			RF_float[rt + 1] = num.as_float;

			break;
		}
		case 0x39: { // swc1 ft, address
#if DEBUGGING
			fprintf(stderr, "swc1\n");
#endif
			number_type num;

			num.as_float = RF_float[rt];

			VM_memory[RF[rs] + immediate + 0] = (uint8_t) ((num.as_int32 >> 24) & 0xFF);
			VM_memory[RF[rs] + immediate + 1] = (uint8_t) ((num.as_int32 >> 16) & 0xFF);
			VM_memory[RF[rs] + immediate + 2] = (uint8_t) ((num.as_int32 >>  8) & 0xFF);
			VM_memory[RF[rs] + immediate + 3] = (uint8_t) ((num.as_int32 >>  0) & 0xFF);

			break;
		}
		case 0x3d: { // sdc1 ft, address
#if DEBUGGING
			fprintf(stderr, "sdc1\n");
#endif
			number_type num;

			num.as_float = RF_float[rt];

			VM_memory[RF[rs] + immediate + 0] = (uint8_t) ((num.as_int32 >> 24) & 0xFF);
			VM_memory[RF[rs] + immediate + 1] = (uint8_t) ((num.as_int32 >> 16) & 0xFF);
			VM_memory[RF[rs] + immediate + 2] = (uint8_t) ((num.as_int32 >>  8) & 0xFF);
			VM_memory[RF[rs] + immediate + 3] = (uint8_t) ((num.as_int32 >>  0) & 0xFF);

			num.as_float = RF_float[rt + 1];

			VM_memory[RF[rs] + immediate + 4] = (uint8_t) ((num.as_int32 >> 24) & 0xFF);
			VM_memory[RF[rs] + immediate + 5] = (uint8_t) ((num.as_int32 >> 16) & 0xFF);
			VM_memory[RF[rs] + immediate + 6] = (uint8_t) ((num.as_int32 >>  8) & 0xFF);
			VM_memory[RF[rs] + immediate + 7] = (uint8_t) ((num.as_int32 >>  0) & 0xFF);

			break;
		}
		//uint8_t op = (instr >> 26) & 0x3F;
		//break; //op
		default:
		{
			printf(">> Unimplemented instruction (op=0x%x, rs=0x%x, rt=0x%x, rd=0x%x, immediate=0x%x, PC: 0x%x).\n", op, rs, rt, rd, immediate, PC);
			return -1;
		}
	}
	advance_pc(offset);//Advances the PC

	return 0;
}

uint32_t fetch(uint32_t PC)
{
	// printf(">>count :\t %u\n", count++);
	// printf(">>PC    :\t %u\n", PC);
	// printf(">>VM[PC]:\t %u\n", VM_memory[PC]);
	uint32_t ret_val  = ((uint32_t)VM_memory[PC + 0] << 24);
	ret_val	= ret_val | ((uint32_t)VM_memory[PC + 1] << 16);
	ret_val	= ret_val | ((uint32_t)VM_memory[PC + 2] <<  8);
	ret_val	= ret_val | ((uint32_t)VM_memory[PC + 3] <<  0);
	return ret_val;
}
