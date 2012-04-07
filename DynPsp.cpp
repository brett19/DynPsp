#include <iostream>
#include "pspMemory.hpp"
#include "ElfLoader.hpp"
#include "ExecDynarec.hpp"
#include "MipsDisassembler.hpp"

int main( int argc, char* argv[] )
{
	pmInit( );

	elfLoad( "D:\\pspapps\\PSPLife\\DATA.PSP" );

	unsigned int uElfEntryPoint = elfEntryPoint();
	unsigned int* pCurInst = (unsigned int*)pmPtr( uElfEntryPoint );

	for( int i = 0; i < 22; ++i ) {
		mips_instruction xInst = mdDisassemble( *pCurInst );
		mdPrint( xInst, *pCurInst, uElfEntryPoint + (i<<2) );

		pCurInst++;
	}

	system( "PAUSE" );
	return 0;
}