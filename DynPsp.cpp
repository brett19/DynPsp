#include <iostream>
#include "pspMemory.hpp"
#include "ElfLoader.hpp"
#include "ExecDynarec.hpp"

int main( int argc, char* argv[] )
{
	pmInit( );

	elfLoad( "D:\\pspapps\\PSPLife\\DATA.PSP" );

	unsigned int uElfEntryPoint = elfEntryPoint();
	unsigned int* pMipsEntryPoint = (unsigned int*)pmPtr( uElfEntryPoint );
	unsigned int uMipsEntryPoint = *pMipsEntryPoint;



	return 0;
}