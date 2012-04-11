#include "pspMemory.hpp"
#include <memory.h>

unsigned char* g_pMemory[4] = { nullptr };
unsigned char* g_pMemLookup[ 0x3F ] = { nullptr };
unsigned int g_pHeapTop = 0;
psp_heap_block *g_pHeapFreedList = nullptr;

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

	g_pHeapTop = 0x0a000000;

	// Test to match JPSCP
	g_pHeapTop -= 0x200;
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

psp_heap_block *pmHeapAlloc( unsigned int size )
{
	psp_heap_block *block = new psp_heap_block( );

	block->start = g_pHeapTop;
	block->size = size;
	block->next = nullptr;

	g_pHeapTop -= size;

	// Fill the stack with garbage to help with debugging
	memset( pmPtr(block->start-size), 0xBD, size );

	return block;
}

void pmHeapFree( psp_heap_block *block )
{

}

void* __stdcall pmPtr( unsigned int addr )
{
	return g_pMemLookup[ addr >> 26 ] + ( addr & 0x01FFFFFF );
}