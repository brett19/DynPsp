#include "pspMemory.hpp"

unsigned char* g_pMemory[4] = { nullptr };
unsigned char* g_pMemLookup[ 0x3F ] = { nullptr };

void pmInit( )
{
	// Scratch RAM
	g_pMemory[0] = new unsigned char[ 0x00004000 ];
	g_pMemLookup[0x00] = g_pMemory[0];

	// V RAM
	g_pMemory[1] = new unsigned char[ 0x00200000 ];
	g_pMemLookup[0x01] = g_pMemory[1];

	// Main RAM
	g_pMemory[2] = new unsigned char[ 0x02000000 ];
	g_pMemLookup[0x02] = g_pMemory[2];

	// UV RAM
	g_pMemory[3] = new unsigned char[ 0x00200000 ];
	g_pMemLookup[0x11] = g_pMemory[3];
}

void pmDestroy( ) {
	for( int i = 0; i < 4; ++i ) {
		delete[] g_pMemory[i];
		g_pMemory[i] = nullptr;
	}
	for( int i = 0x00; i < 0x3F; ++i ) {
		g_pMemLookup[i] = nullptr;
	}
}

void* pmPtr( unsigned int addr )
{
	return g_pMemLookup[ addr >> 26 ] + ( addr & 0x01FFFFFF );
}