#include "ElfLoader.hpp"
#include <iostream>
#include "ElfHeaders.h"
#include "pspMemory.hpp"

unsigned int g_uiBaseAddress = 0x08804000;
unsigned int g_uiEntryPoint = 0x00000000;

bool elfLoad( FILE *fh )
{
	elf_header elf;
	fread( &elf, sizeof(elf_header), 1, fh );

	g_uiEntryPoint = g_uiBaseAddress + elf.mEntryPoint;

	if( elf.mType == 0xFFA0 ) {
		// Needs Relocation
	}

	for( int i = 0; i < elf.mPHEntryCount; i++ ) {
		fseek( fh, elf.mPHOffset + i * elf.mPHEntrySize, SEEK_SET );

		elf_programheader ph;
		fread( &ph, sizeof(elf_programheader), 1, fh );

		fseek( fh, ph.mOffset, SEEK_SET );
		fread( pmPtr(g_uiBaseAddress + ph.mVAddress), ph.mFileSize, 1, fh );
	}

	for( int i = 0; i < elf.mSHEntryCount; i++ ) {
		fseek( fh, elf.mSHOffset + i * elf.mSHEntrySize, SEEK_SET );

		elf_sectionheader sh;
		fread( &sh, sizeof(elf_sectionheader), 1, fh );

		if( sh.mType == SHT_NULL ) {
		} else if( sh.mType == SHT_PROGBITS ) {
		} else if( sh.mType == SHT_STRTAB ) {
		} else if( sh.mType == SHT_NOBITS ) {
		} else if( sh.mType == SHT_PRXREL ) {
			fseek( fh, sh.mOffset, SEEK_SET );
			int relCount = sh.mSize / sizeof(elf_relocation);

			//printf( "Relocating %d entries\n", relCount );

			for( int j = 0; j < relCount; ++j ) {
				elf_relocation rel;
				fread( &rel, sizeof(elf_relocation), 1, fh );

				if( rel.mType == 2 ) {
					// R_MIPS_32
					unsigned int* pRel = (unsigned int*)pmPtr( g_uiBaseAddress + rel.mOffset );
					*pRel += g_uiBaseAddress;
				} else if( rel.mType == 4 ) {
					// R_MIPS_26
					struct relHelper {
						int address : 26;
						int x : 6;
					} *pRel = (relHelper*)pmPtr( g_uiBaseAddress + rel.mOffset );
					pRel->address += g_uiBaseAddress >> 2;
				} else if( rel.mType == 5 ) {
					// R_MIPS_HI16
					struct relHelper {
						int address : 16;
						int x : 16;
					} *pRel = (relHelper*)pmPtr( g_uiBaseAddress + rel.mOffset );
					pRel->address += g_uiBaseAddress >> 16;
				} else if( rel.mType == 6 ) {
					// R_MIPS_LO16
					struct relHelper {
						int address : 16;
						int x : 16;
					} *pRel = (relHelper*)pmPtr( g_uiBaseAddress + rel.mOffset );
					pRel->address += g_uiBaseAddress && 0xFFFF;
				} else if( rel.mType == 7 ) {
					// Silenced for now!
				} else {
					printf( "Relocation %d - Unknown Type %d\n", j, rel.mType );
				}
			}
		} else {
			printf( "Section %d - Unknown Type %d\n", i, sh.mType );
		}
		
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