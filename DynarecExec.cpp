#include "DynarecExec.hpp"

#define _eeGetAddressIdx( addr ) ((addr-0x08000000)>>2)

void _eeGenerateReturn( X86Assembler *asmb, unsigned int next_address )
{
	asmb->aMOV_EAX_IMM( next_address );
	asmb->aRETN( );
}

bool _eeGenerateOne( dynarec_context *context, unsigned int address )
{
	_eeGenerateReturn( context->assembler, address + 4 );

	return false;

}

bool _eeGenerateCode( dynarec_context *context, unsigned int address )
{
	while( true ) {
		void * start_location = context->assembler->getCursor( );
		bool bContinue = _eeGenerateOne( context, address );
		context->state->transtable[_eeGetAddressIdx(address)] = start_location;
		address += 4;

		if( !bContinue ) {
			break;
		}

		// Test
		break;
	}
	return true;
}

bool _eeEnsureAddress( dynarec_context *context, unsigned int address )
{
	if( context->state->transtable[_eeGetAddressIdx(address)] == nullptr ) {
		_eeGenerateCode( context, address );
	}
	return true;
}

unsigned int _eeExecute( dynarec_context *context, unsigned int address )
{
	void* this_address = context->state->transtable[_eeGetAddressIdx(address)];
	unsigned int next_address;
	__asm {
		MOV esi, context
		CALL this_address
		MOV next_address, EAX
	};
	return next_address;
}

bool _eeExecAddress( dynarec_context *context, unsigned int address )
{
	void **transtable = context->state->transtable;

	unsigned int next_address = address;
	while( next_address ) {
		_eeEnsureAddress( context, next_address );
		next_address = _eeExecute( context, next_address );
	}
	return true;
}


bool eeExecute( dynarec_context *context, unsigned int entrypoint )
{
	_eeExecAddress( context, entrypoint );
	return true;
}