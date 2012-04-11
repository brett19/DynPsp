#include "DynarecExec.hpp"
#include <stddef.h>
#include "MipsDisassembler.hpp"
#include "pspMemory.hpp"

using namespace AsmJit;

enum GeneratorFlags {
	GF_NONE = 0,
	GF_STOPDISASM = 1,
	GF_DELAYSLOT = 2
};

// ee General Helpers
#define _eeGetAddressIdx( addr ) ((addr-0x08000000)>>2)

// ee Context Offset Helpers
#define _eeoCtxReg( reg_idx ) dword_ptr(esi,(offsetof(dynarec_context,regs[reg_idx])))

Label* _eeGetLabel( dynarec_context *context ) {
	dynarec_context::asm_label *label = new dynarec_context::asm_label;
	label->data = context->asmblr->newLabel( );
	label->next = context->labels;
	context->labels = label;
	return &label->data;
}

void __eeGenerateReturn( dynarec_context *context, unsigned int next_address )
{
	AsmJit::Assembler*& asmb = context->asmblr;
	asmb->mov( eax, Imm(next_address) );
	asmb->ret( );
}

void _eeGenerateJump( dynarec_context *context, unsigned int address )
{
	AsmJit::Assembler*& asmb = context->asmblr;
	void *jmploc = context->state->transtable[_eeGetAddressIdx(address)];
	if( jmploc != nullptr ) {
		asmb->jmp( jmploc );
	} else {
		__eeGenerateReturn( context, address );
	}
}

// Returns the X86 Memory Address in EAX of > %($reg_idx) + $offset <
void _eeGenerateRegOffPtrToEax( dynarec_context *context, unsigned char reg_idx, int offset )
{
	AsmJit::Assembler*& asmb = context->asmblr;
	asmb->mov( eax, _eeoCtxReg(reg_idx) );
	asmb->add( eax, Imm(offset) );
	asmb->push( eax );
	asmb->call( &pmPtr );
}

bool _eeGenerateOne( dynarec_context *context, unsigned int address );

void _eeInitDelaySlot( dynarec_context *context )
{
	context->delayslotlabel = _eeGetLabel( context );
}
bool _eeGenerateDelaySlot( dynarec_context *context, unsigned int address )
{
	return _eeGenerateOne( context, address );
}

bool _eeGenerateOne( dynarec_context *context, unsigned int address )
{
	bool bContDisasm = true;
	AsmJit::Assembler*& asmb = context->asmblr;

	mips_opcode op( *(unsigned int*)pmPtr(address) );
	const mips_instruction_info *info = mdDisassemble( op.val );
	
	if( info->type == ALGXINS_LUI ) {
		asmb->mov( _eeoCtxReg(op.alimm.urt), Imm(op.alimm.uimm << 16) );
	} else if( info->type == ALGXINS_ADDIU ) {
		asmb->add( _eeoCtxReg(op.alimm.urt), Imm(op.alimm.simm) );
	} else if( info->type == ALGXINS_SW ) {
		_eeGenerateRegOffPtrToEax( context, op.alimm.urs, op.alimm.simm );
		asmb->mov( ecx, _eeoCtxReg(op.alimm.urt) );
		asmb->mov( dword_ptr(eax), ecx );
	} else if( info->type == ALGXINS_LW ) {
		_eeGenerateRegOffPtrToEax( context, op.alimm.urs, op.alimm.simm );
		asmb->mov( ecx, dword_ptr(eax) );
		asmb->mov( _eeoCtxReg(op.alimm.urt), ecx );
	} else if( info->type == ALGXINS_ADDU ) {
		asmb->mov( eax, _eeoCtxReg(op.alreg.urt) );
		asmb->add( eax, _eeoCtxReg(op.alreg.urs) );
		asmb->mov( _eeoCtxReg(op.alreg.urd), eax );


	} else if( info->type == ALGXINS_SLL ) {

		asmb->mov( eax, _eeoCtxReg(op.alreg.urt) );
		asmb->shl( eax, Imm(op.alreg.usa) );
		asmb->mov( _eeoCtxReg(op.alreg.urd), eax );


	} else if( info->type == ALGXINS_BEQ ) {
		_eeInitDelaySlot( context );
		_eeGenerateDelaySlot( context, address + 4 );

		asmb->mov( eax, _eeoCtxReg(op.alimm.urs) );
		asmb->cmp( eax, _eeoCtxReg(op.alimm.urt) );
		asmb->jne( *context->delayslotlabel );
		_eeGenerateJump( context, address + 4 + ( op.alimm.simm << 2 ) );

	} else if( info->type == ALGXINS_BEQL ) {
		_eeInitDelaySlot( context );

		asmb->mov( eax, _eeoCtxReg(op.alimm.urs) );
		asmb->cmp( eax, _eeoCtxReg(op.alimm.urt) );
		asmb->jne( *context->delayslotlabel );
		_eeGenerateDelaySlot( context, address + 4 );
		_eeGenerateJump( context, address + 4 + ( op.alimm.simm << 2 ) );

	} else if( info->type == ALGXINS_J ) {
		_eeInitDelaySlot( context );
		_eeGenerateDelaySlot( context, address + 4 );
		_eeGenerateJump( context, ( address & 0xF0000000 ) | ( op.aljmp.uimm << 2 ) );

	} else if( info->type == ALGXINS_JAL ) {
		_eeInitDelaySlot( context );
		_eeGenerateDelaySlot( context, address + 4 );
		asmb->mov( _eeoCtxReg(PREG_RA), address + 8 );
		_eeGenerateJump( context, ( address & 0xF0000000 ) | ( op.aljmp.uimm << 2 ) );

	} else if( info->type == ALGXINS_JR ) {
		_eeInitDelaySlot( context );
		_eeGenerateDelaySlot( context, address + 4 );
		
		// Jump back to emulator to determine where to go next!
		asmb->mov( eax, _eeoCtxReg(op.alreg.urs) );
		asmb->ret( );

	} else if( info->type == ALGXINS_NOP || info->type == ALGXINS_VNOP ) {
		asmb->nop( );
	} else {
		// Unknown Instruction!
		printf( "Failed to decode - [%08x] %s\n", address, info->name );
		asmb->int3( );
		bContDisasm = false;
	}

	return bContDisasm;
}

bool _eeGenerateCode( dynarec_context *context, unsigned int address )
{
	unsigned int start_address = address;

	static const int MAX_DISASM = 40;
	sysint_t ins_location[MAX_DISASM+2];

	AsmJit::Assembler*& asmb = context->asmblr;
	asmb->clear( );

	Label *delayslot = nullptr;

	int inscnt = 0;
	while( true ) {
		ins_location[inscnt] = asmb->getOffset();

		// Put this PSP Address into context->pc
		asmb->mov( _eeoCtxReg(PREG_PC), address );

		bool bContinue = _eeGenerateOne( context, address );
		
		if( delayslot && context->delayslotlabel ) {
			// Two delay slots in a row?  BROKEN!
			throw 0;
		} else if( delayslot ) {
			asmb->bind( *delayslot );
			delayslot = nullptr;
		} else if( context->delayslotlabel ) {
			delayslot = context->delayslotlabel;
			context->delayslotlabel = nullptr;
		}

		address += 4;
		inscnt++;

		// We have to force delay slot generation
		if( !delayslot ) {
			if( !bContinue || inscnt >= MAX_DISASM ) {
				// Return to the emulator!
				__eeGenerateReturn( context, address + 4 );
				break;
			}
		}
	}

	// GENERATE CODE!
	void *new_code = asmb->make( );

	// Update translation table
	address = start_address;
	for( int i = 0; i < inscnt; ++i ) {
		context->state->transtable[_eeGetAddressIdx(address)] = (unsigned char*)new_code + ins_location[i];
		address += 4;
	}

	// Clean up labels
	while( context->labels ) {
		dynarec_context::asm_label *label = context->labels;
		context->labels = context->labels->next;
		delete label;
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
	void * stack_top = pmPtr( context->stack->start );
	__asm {
		// Test Code :: Move the top of the stack's pointer into EDI
		MOV edi, stack_top

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