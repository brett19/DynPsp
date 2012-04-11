#include "DynarecContext.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <malloc.h>
#include <memory.h>

dynarec_state * esCreate( )
{
	dynarec_state *state = (dynarec_state*)malloc( sizeof(dynarec_state) );
	if( state ) {
		size_t tablesize = ( PSP_MEM_SIZE<< 2 );
		state->transtable = (void**)_aligned_malloc( tablesize, ATOMIC_ALIGN );
		memset( state->transtable, 0, tablesize );
	}
	return state;
}

void esDestroy( dynarec_state *&state )
{
	if( state ) {
		_aligned_free( state->transtable );
		free( state );
		state = nullptr;
	}
}

dynarec_context * ecCreate( dynarec_state *state, unsigned int stacksize )
{
	dynarec_context *context = (dynarec_context*)malloc( sizeof(dynarec_context) );
	if( context ) {
		context->state = state;
		context->stack = pmHeapAlloc( stacksize );

		for( int i = 0; i < 35; ++i ) {
			context->regs[i] = 0xBAADF00D;
		}
		context->regs[PREG_ZR] = 0x00000000;
		context->regs[PREG_SP] = context->stack->start;

		context->asmblr = new AsmJit::Assembler( );
		context->labels = nullptr;
		context->delayslotlabel = nullptr;
	}
	return context;
}

void ecDestroy( dynarec_context *&context )
{
	pmHeapFree( context->stack );
	free( context );
	context = nullptr;
}