#pragma once

#include "X86Assembler.hpp"
#include "AsmJit/Assembler.h"
#include "pspMemory.hpp"

const static int PSP_MEM_SIZE = 0x2000000;
const static int ATOMIC_ALIGN = 4;

enum pspregs {
	PREG_ZR = 0,
	PREG_AT = 1,
	PREG_V0 = 2,
	PREG_V1 = 3,
	PREG_K0 = 26,
	PREG_K1 = 27,
	PREG_GP = 28,
	PREG_SP = 29,
	PREG_RA = 31,
	PREG_LO = 32,
	PREG_HI = 33,
	PREG_PC = 34
};

struct dynarec_state
{
	void** transtable;
};

struct dynarec_context
{
	unsigned int regs[ 35 ];

	AsmJit::Assembler *asmblr;

	struct asm_label {
		AsmJit::Label data;
		asm_label *next;
	} *labels;
	AsmJit::Label *delayslotlabel;

	psp_heap_block *stack;
	dynarec_state *state;
};

dynarec_state * esCreate( );
void esDestroy( dynarec_state *&state );

dynarec_context * ecCreate( dynarec_state *state, unsigned int stacksize = 0x200 );
void ecDestroy( dynarec_context *&context );