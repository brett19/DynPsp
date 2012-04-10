#include <iostream>
#include "pspMemory.hpp"
#include "ElfLoader.hpp"
#include "DynarecContext.hpp"
#include "DynarecExec.hpp"
#include "MipsDisassembler.hpp"

int main( int argc, char* argv[] )
{
	pmInit( );

	elfLoad( "D:\\pspapps\\PSPLife\\DATA.PSP" );

	unsigned int uElfEntryPoint = elfEntryPoint();

	dynarec_state *state = esCreate( );
	dynarec_context *context = ecCreate( state );
	eeExecute( context, uElfEntryPoint );

	/*
	unsigned int* pCurInst = (unsigned int*)pmPtr( 0x08804000 );

	for( int i = 0x08804000 ; i <= 0x08804000 + 0x24BB8; i += 4 ) {
		const mips_instruction_info* xInst = mdDisassemble( *pCurInst );
		
		if( xInst == 0 ) {
			printf( "Unknown instruction %08x @ %08x\n", *pCurInst, i );
		}

		pCurInst++;
	}
	*/

	system( "PAUSE" );
	return 0;
}