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

dynarec_context * ecCreate( dynarec_state *state )
{
	dynarec_context *context = (dynarec_context*)malloc( sizeof(dynarec_context) );
	if( context ) {
		context->state = state;

		for( int i = 0; i < 34; ++i ) {
			context->regs[i] = 0xBAADF00D;
		}
		context->regs[0] = 0x00000000;

		context->starttransmem = (unsigned char*)VirtualAlloc( 0, 0x2000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE );
		context->transmem = context->starttransmem;

		context->assembler = new X86Assembler( context->transmem, 0x2000 );
	}
	return context;
}

void ecDestroy( dynarec_context *&context )
{
	free( context );
	context = nullptr;
}