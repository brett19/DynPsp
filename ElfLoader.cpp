#include "ElfLoader.hpp"
#include <iostream>
#include "ElfHeaders.h"
#include "pspMemory.hpp"

unsigned int g_uiBaseAddress = 0x08004000;
unsigned int g_uiEntryPoint = 0x00000000;

bool elfLoad( FILE *fh )
{
	elf_header elf;
	fread( &elf, sizeof(elf_header), 1, fh );

	g_uiEntryPoint = g_uiBaseAddress + elf.mEntryPoint;

	for( int i = 0; i < elf.mSHEntryCount; i++ ) {
		fseek( fh, elf.mSHOffset + i * elf.mSHEntrySize, SEEK_SET );

		elf_sectionheader sh;
		fread( &sh, sizeof(elf_sectionheader), 1, fh );
	}

	for( int i = 0; i < elf.mPHEntryCount; i++ ) {
		fseek( fh, elf.mPHOffset + i * elf.mPHEntrySize, SEEK_SET );

		elf_programheader ph;
		fread( &ph, sizeof(elf_programheader), 1, fh );

		fseek( fh, ph.mOffset, SEEK_SET );
		fread( pmPtr(g_uiBaseAddress + ph.mVAddress), ph.mFileSize, 1, fh );
	}

	return true;
}

bool elfLoad( const char *path ) 
{
	FILE *fh = fopen( path, "rb" );
	if( !fh ) return false;
	bool bRetVal = elfLoad( fh );
	fclose( fh );
	return bRetVal;
}

unsigned int elfEntryPoint( )
{
	return g_uiEntryPoint;
}