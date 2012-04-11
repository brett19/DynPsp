#pragma once

struct psp_heap_block {
	unsigned int start;
	unsigned int size;

	psp_heap_block *next;
};

void pmInit( );
void pmDestroy( );
void* __stdcall pmPtr( unsigned int addr );

psp_heap_block *pmHeapAlloc( unsigned int size );
void pmHeapFree( psp_heap_block *block );