#pragma once

#include "MipsOpcodes.h"

enum mips_instruction_type
{
	XIT_UNKNOWN,
	XIT_SPECIAL,
	XIT_IMMEDIATE,
	XIT_BRANCH,
	XIT_JUMP,
	XIT_SYS,
	XIT_NOP
};

enum mips_instruction_opusage
{
	XOU_NONE = 1,
	XOU_RS  = 2,
	XOU_RSBASE = 4,
	XOU_RT  = 8,
	XOU_RD = 16,
	XOU_SA = 32,
	XOU_IMM = 64,
	XOU_IMMJMP = 128,
	XOU_CODE = 256
};

struct mips_instruction
{
	unsigned int mask;
	unsigned int compare;
	mips_instruction_type type;
	const char* name;
	unsigned int opusage;
};

const mips_instruction& mdDisassemble( unsigned int inst );
void mdPrint( const mips_instruction& ins_data, unsigned int inst, unsigned int loc );