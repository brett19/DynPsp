#pragma once

#include "X86Assembler.hpp"

const static int PSP_MEM_SIZE = 0x2000000;
const static int ATOMIC_ALIGN = 4;

struct dynarec_state
{
	void** transtable;
};

struct dynarec_context
{
	unsigned int regs[ 34 ];
	unsigned int pc;

	unsigned char *starttransmem;
	unsigned char *transmem;
	X86Assembler *assembler;

	dynarec_state *state;
};

dynarec_state * esCreate( );
void esDestroy( dynarec_state *&state );

dynarec_context * ecCreate( dynarec_state *state );
void ecDestroy( dynarec_context *&context );