#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */
int sign_extend_number(unsigned int field, unsigned int n) {
  /* YOUR CODE HERE */
  //Shift the field 32-n to the left and then shift 32-n to right
  //This will automatically sign extend the number
  return (int)field << (32-n)>>(32-n);
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */
Instruction parse_instruction(uint32_t instruction_bits) {
  /* YOUR CODE HERE */
  
  Instruction instruction;
  // add x8, x0, x0     hex : 00000433  binary = 0000 0000 0000 0000 0000 01000
  // Opcode: 0110011 (0x33) Get the Opcode by &ing 0x1111111, bottom 7 bits
  instruction.opcode = instruction_bits & ((1U << 7) - 1);
  // Shift right to move to pointer to interpret next fields in instruction.
  instruction_bits >>= 7;
  
  switch (instruction.opcode) {
  // R-Type
  case 0x33:
    // instruction: 0000 0000 0000 0000 0000 destination : 01000
    instruction.rtype.rd = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 0000 0000 0000 0 func3 : 000
    instruction.rtype.funct3 = instruction_bits & ((1U << 3) - 1);
    instruction_bits >>= 3;

    // instruction: 0000 0000 0000  src1: 00000
    instruction.rtype.rs1 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // instruction: 0000 000        src2: 00000
    instruction.rtype.rs2 = instruction_bits & ((1U << 5) - 1);
    instruction_bits >>= 5;

    // funct7: 0000 000
    instruction.rtype.funct7 = instruction_bits & ((1U << 7) - 1);
    break;

  // case for I-type 
  case 0x13:
  case 0x03:
  case 0x73:
  case 0x67:
    //Instruction
    instruction.itype.rd = instruction_bits & ((1U << 5)-1);
    instruction_bits >>= 5;
    
    instruction.itype.funct3 = instruction_bits & ((1U << 3)-1);
    instruction_bits >>= 3;
    instruction.itype.rs1 = instruction_bits & ((1U << 5)-1);
    instruction_bits >>=5;
    instruction.itype.imm = instruction_bits & ((1U << 12)-1);
    break;
//case for J-type
  case 0x6f:
    instruction.ujtype.rd = instruction_bits & ((1U << 5)-1);
    instruction_bits >>=5;
    instruction.ujtype.imm = instruction_bits & ((1U << 20)-1);
    break;

//case for U-type
  case 0x37:
  case 0x17:
    instruction.utype.rd = instruction_bits & ((1U << 5)-1);
    instruction_bits >>=5;
    instruction.utype.imm = instruction_bits & ((1U << 20)-1);
    break;

//case for S-type
  case 0x23:
    instruction.stype.imm5 = instruction_bits & ((1U << 5)-1);
    instruction_bits >>=5;
    instruction.stype.funct3 = instruction_bits & ((1U << 3)-1);
    instruction_bits >>=3;
    instruction.stype.rs1 = instruction_bits & ((1U << 5)-1);
    instruction_bits >>=5;
    instruction.stype.rs2 = instruction_bits & ((1U << 5)-1);
    instruction_bits >>=5;
    instruction.stype.imm7 = instruction_bits & ((1U << 7)-1);
    break;

//case for B-type
    case 0x63:
      instruction.sbtype.imm5 = instruction_bits & ((1U << 5)-1);
      instruction_bits >>=5;
      instruction.sbtype.funct3 = instruction_bits & ((1U << 3)-1);
      instruction_bits >>=3;
      instruction.sbtype.rs1 = instruction_bits & ((1U << 5)-1);
      instruction_bits >>=5;
      instruction.sbtype.rs2 = instruction_bits & ((1U << 5)-1);
      instruction_bits >>=5;
      instruction.sbtype.imm7 = instruction_bits & ((1U << 7)-1);
      break;
  default:
    exit(EXIT_FAILURE);
  }
  return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using
 * the given branch instruction */
int get_branch_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  int offset = 0x00000000;
  //imm[12|10:5]     imm[4:1|11] 

  //Getting mask for bits 1 to 4
  //Set 1 to 4 bits
  offset|= (instruction.sbtype.imm5) & 0x0000001e;

  //Getting mask for 11th bit
  //Shifting imm5 left by 11 to match the index
  //Set 11th bit
  offset|= (instruction.sbtype.imm5<<11) & 0x00000800;

  //Getting mask for 12 bit
  //Shift the number by 12
  //Set 12th bit of offset
  offset|= (instruction.sbtype.imm7<<12) & 0x000001000;

  //Getting mask for bits 10 to 5
  //Shift the number by 5
  //Set 10 to 5 bits of the offset
  offset |= (instruction.sbtype.imm7<<5)& 0x7e0;

  return sign_extend_number(offset,13);
}

/* Returns the number of bytes (from the current PC) to the jump label using the
 * given jump instruction */
int get_jump_offset(Instruction instruction) {
  /* YOUR CODE HERE */
//   int offset = 0x00000000000000000000;
//   
//  //Get the value from the first 10 bits
//  offset|=(instruction.ujtype.imm) & 0x000003FF;
//  offset|=(instruction.ujtype.imm>>10)& 0x400;
//  offset|=(instruction.ujtype.imm)
//  //Get the value from the 20th bit
//   offset |= (instruction.ujtype.imm) & 0x80000;
//   offset|= (instruction.ujtype.imm)&

//   // imm
//   return 0;
   // Initialize the offset to 0
    int offset = 0x00000000;
//imm[20|10:1|11|19:12]

    // imm[20]
    //Mask the 20th bit
    // Set the 20th bit
    offset |= (instruction.ujtype.imm<<1 & 0x00100000);
    // imm[10:1]
    //Mask the next 10 bits by shifting imm 8 to left
    // Extract bits 1 to 10 
    offset |= (instruction.ujtype.imm>>8 & 0x000007fe);
    // imm[11]
    //Left shift imm by 3 to mask the next bit
    // Bitwise shift and set the 11th bit
    offset |= (instruction.ujtype.imm<<3 & 0x00000800);
    // imm[19:12]
    //Left shift imm by 12 to mask the next 8 bits
    // Extract bits 12 to 19 
    offset |=(instruction.ujtype.imm<<12 & 0x000ff000);
    // Sign-extend to 32 bits (optional if needed)
    return sign_extend_number(offset, 21);

}


int get_store_offset(Instruction instruction) {
  /* YOUR CODE HERE */
  int offset = 0x00000000;
  //imm[0:4]
  //Make the bottom 5 bits 1 and create a mask
  offset |= (instruction.stype.imm5)&(0x0000001f);

  //imm[11:5] = 6 bits
  //Shift imm7 left by 5 bits
  //Create a mask by making bits 5 to 11 = 1
  offset |= (instruction.stype.imm7<<5)&(0x00000fe0);
  return sign_extend_number(offset,12);
  //return 0;
}

void handle_invalid_instruction(Instruction instruction) {
  printf("Invalid Instruction: 0x%08x\n", instruction.bits);
}

void handle_invalid_read(Address address) {
  printf("Bad Read. Address: 0x%08x\n", address);
  exit(-1);
}

void handle_invalid_write(Address address) {
  printf("Bad Write. Address: 0x%08x\n", address);
  exit(-1);
}
