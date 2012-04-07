#include "MipsDisassembler.hpp"
#include <iostream>

static const char* REG_NAME[] = {
	"zr","at","v0","v1","a0","a1","a2","a3",
	"t0","t1","t2","t3","t4","t5","t6","t7",
	"s0","s1","s2","s3","s4","s5","s6","s7",
	"t8","t9","k0","k1","gp","sp","fp","ra"
};

static const mips_instruction g_xUnknownIns = { 0x00000000, 0x00000000, XIT_UNKNOWN, "UNK" };
static const mips_instruction g_xMipsIns[] = {
	{ 0xFFFFFFFF, 0x00000000, XIT_NOP,			"NOP",		XOU_NONE },
	{ 0xFC00003F, 0x00000020, XIT_SPECIAL,		"ADD",		XOU_RS|XOU_RT|XOU_RD },
	{ 0xFC000000, 0x20000000, XIT_IMMEDIATE,	"ADDI",		XOU_RS|XOU_RT|XOU_IMM },
	{ 0xFC000000, 0x24000000, XIT_IMMEDIATE,	"ADDIU",	XOU_RS|XOU_RT|XOU_IMM },
	{ 0xFC00003F, 0x00000021, XIT_SPECIAL,		"ADDU",		XOU_RS|XOU_RT|XOU_RD },
	{ 0xFC00003F, 0x00000024, XIT_SPECIAL,		"AND",		XOU_RS|XOU_RT|XOU_RD },
	{ 0xFC000000, 0x30000000, XIT_IMMEDIATE,	"ANDI",		XOU_RS|XOU_RT|XOU_IMM },
	// BCzF
	// BCzFL	// BCzT
	// BCzTL
	{ 0xFC000000, 0x10000000, XIT_IMMEDIATE,	"BEQ",		XOU_RS|XOU_RT|XOU_IMMJMP },
	{ 0xFC000000, 0x50000000, XIT_IMMEDIATE,	"BEQL",		XOU_RS|XOU_RT|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04010000, XIT_BRANCH,		"BGEZ",		XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04110000, XIT_BRANCH,		"BGEZAL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04130000, XIT_BRANCH,		"BGEZALL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04030000, XIT_BRANCH,		"BGEZL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC000000, 0x1c000000, XIT_BRANCH,		"BGTZ",		XOU_RS|XOU_IMMJMP },
	{ 0xFC000000, 0x5c000000, XIT_BRANCH,		"BGTZL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC000000, 0x18000000, XIT_BRANCH,		"BLEZ",		XOU_RS|XOU_IMMJMP },
	{ 0xFC000000, 0x58000000, XIT_BRANCH,		"BLEZL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04000000, XIT_BRANCH,		"BLTZ",		XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04100000, XIT_BRANCH,		"BLTZAL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04120000, XIT_BRANCH,		"BLTZALL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC1F0000, 0x04020000, XIT_BRANCH,		"BLTZL",	XOU_RS|XOU_IMMJMP },
	{ 0xFC000000, 0x14000000, XIT_IMMEDIATE,	"BNE",		XOU_RS|XOU_RT|XOU_IMMJMP },
	{ 0xFC000000, 0x54000000, XIT_IMMEDIATE,	"BNEL",		XOU_RS|XOU_RT|XOU_IMMJMP },
	{ 0xFC00003F, 0x0000000D, XIT_SYS,			"BREAK",	XOU_CODE },
	// CACHE
	// CFCz
	// COPz
	// CTCz
	// DADD - 64 bit
	// DADDI - 64 bit
	// DADDIU - 64 bit
	// DADDU - 64 bit
	// DDIV - 64 bit
	// DDIVU - 64 bit
	{ 0xFC00003F, 0x0000001A, XIT_SPECIAL,		"DIV",		XOU_RS|XOU_RT },
	{ 0xFC00003F, 0x0000001B, XIT_SPECIAL,		"DIVU",		XOU_RS|XOU_RT },
	// DMFC0 - 64 bit
	// DMTC0 - 64 bit
	// DMULT - 64 bit
	// DMULTU - 64 bit
	// DSLL - 64 bit
	// DSLLV - 64 bit
	// DSLL32 - 64 bit
	// DSRA - 64 bit
	// DSRAV - 64 bit
	// DSRA32 - 64 bit
	// DSRL - 64 bit
	// DRSLV - 64 bit
	// DSRL32 - 64 bit
	// DSUB - 64 bit
	// DSUBU - 64 bit
	// ERET
	{ 0xFC000000, 0x08000000, XIT_JUMP,			"J",		XOU_IMMJMP },
	{ 0xFC000000, 0x0C000000, XIT_JUMP,			"JAL",		XOU_IMMJMP },
	{ 0xFC00003F, 0x00000009, XIT_SPECIAL,		"JALR",		XOU_RS|XOU_RD },
	{ 0xFC00003F, 0x00000008, XIT_SPECIAL,		"JR",		XOU_RS },
	{ 0xFC000000, 0x80000000, XIT_IMMEDIATE,	"LB",		XOU_RSBASE|XOU_RT|XOU_IMM },
	{ 0xFC000000, 0x90000000, XIT_IMMEDIATE,	"LBU",		XOU_RSBASE|XOU_RT|XOU_IMM },
	// LD - 64 bit
	// LDCz
	// LDL - 64 bit
	// LDR - 64 bit
	{ 0xFC000000, 0x84000000, XIT_IMMEDIATE,	"LH",		XOU_RSBASE|XOU_RT|XOU_IMM },
	{ 0xFC000000, 0x94000000, XIT_IMMEDIATE,	"LHU",		XOU_RSBASE|XOU_RT|XOU_IMM },
	{ 0xFC000000, 0xC0000000, XIT_IMMEDIATE,	"LL",		XOU_RSBASE|XOU_RT|XOU_IMM },
	// LLD - 64 bit
	{ 0xFC000000, 0x3C000000, XIT_IMMEDIATE,	"LUI",		XOU_RT|XOU_IMM },
	{ 0xFC000000, 0x8C000000, XIT_IMMEDIATE,	"LW",		XOU_RSBASE|XOU_RT|XOU_IMM },
	// LWCz
	{ 0xFC000000, 0x88000000, XIT_IMMEDIATE,	"LWL",		XOU_RSBASE|XOU_RT|XOU_IMM },
	{ 0xFC000000, 0x98000000, XIT_IMMEDIATE,	"LWR",		XOU_RSBASE|XOU_RT|XOU_IMM },
	{ 0xFC000000, 0x9C000000, XIT_IMMEDIATE,	"LWU",		XOU_RSBASE|XOU_RT|XOU_IMM },
	// MFC0
	// MFCz
	{ 0xFC00003F, 0x00000010, XIT_SPECIAL,		"MFHI",		XOU_RD },
	{ 0xFC00003F, 0x00000012, XIT_SPECIAL,		"MFLO",		XOU_RD },
	// MTC0
	// MTCz
	{ 0xFC00003F, 0x00000011, XIT_SPECIAL,		"MTHI",		XOU_RS },
	{ 0xFC00003F, 0x00000013, XIT_SPECIAL,		"MTLO",		XOU_RS },
	{ 0xFC00003F, 0x00000018, XIT_SPECIAL,		"MULT",		XOU_RS|XOU_RT },
	{ 0xFC00003F, 0x00000019, XIT_SPECIAL,		"MULTU",	XOU_RS|XOU_RT },
	{ 0xFC00003F, 0x00000027, XIT_SPECIAL,		"NOR",		XOU_RS|XOU_RT|XOU_RD },
	{ 0xFC00003F, 0x00000025, XIT_SPECIAL,		"OR",		XOU_RS|XOU_RT|XOU_RD },



	{ 0xFC000000, 0xAC000000, XIT_IMMEDIATE,	"SW",		XOU_RSBASE|XOU_RT|XOU_IMM },
	
	
};
static const unsigned int g_iNumMipsInst = sizeof(g_xMipsIns) / sizeof(g_xMipsIns[0]);

const mips_instruction& mdDisassemble( unsigned int inst )
{
	for( int i = 0; i < g_iNumMipsInst; ++i ) {
		const mips_instruction& cmp_inst = g_xMipsIns[i];
		unsigned int uMaskedIns = inst & cmp_inst.mask;
		if( uMaskedIns == cmp_inst.compare ) {
			return cmp_inst;
		}
	}
	return g_xUnknownIns;
}

void mdPrint( const mips_instruction& ins_data, unsigned int inst, unsigned int loc )
{
	printf( "[%08x:%08x] ", loc, inst );
	if( ins_data.type == XIT_IMMEDIATE ) {
		mips_opcode_imm* dinst = (mips_opcode_imm*)&inst;
		if( ins_data.opusage == (XOU_RT|XOU_RS|XOU_IMM) ) {
			printf( "%s $%s,$%s,%d\n", ins_data.name, REG_NAME[dinst->rt], REG_NAME[dinst->rs], dinst->simm );
		} else if( ins_data.opusage == (XOU_RSBASE|XOU_RT|XOU_IMM) ) {
			printf( "%s $%s,%d($%s)\n", ins_data.name, REG_NAME[dinst->rt], dinst->simm, REG_NAME[dinst->rs] );
		} else if( ins_data.opusage == (XOU_RT|XOU_IMM) ) {
			printf( "%s $%s,%d\n", ins_data.name, REG_NAME[dinst->rt], dinst->simm );
		} else if( ins_data.opusage == (XOU_RT|XOU_RS|XOU_IMMJMP) ) {
			printf( "%s $%s,$%s,%08x\n", ins_data.name, REG_NAME[dinst->rt], REG_NAME[dinst->rs], loc+4+(dinst->simm<<2) );
		} else {
			printf( "%s - Unknown Decoding\n", ins_data.name );
		}
	} else if( ins_data.type == XIT_SPECIAL ) {
		mips_opcode_sp* dinst = (mips_opcode_sp*)&inst;
		if( ins_data.opusage == (XOU_RS|XOU_RT|XOU_RD) ) {
			printf( "%s $%s,$%s,$%s\n", ins_data.name, REG_NAME[dinst->rd], REG_NAME[dinst->rs], REG_NAME[dinst->rt] );
		} else if( ins_data.opusage == (XOU_RS) ) {
			printf( "%s $%s\n", ins_data.name, REG_NAME[dinst->rs] );
		} else {
			printf( "%s - Unknown Decoding\n", ins_data.name );
		}
	} else if( ins_data.type == XIT_JUMP ) {
		mips_opcode_jmp* dinst = (mips_opcode_jmp*)&inst;
		if( ins_data.opusage == (XOU_IMMJMP) ) {
			printf( "%s %08x\n", ins_data.name, dinst->imm << 2 );
		} else {
			printf( "%s - Unknown Decoding\n", ins_data.name );
		}
	} else if( ins_data.type == XIT_NOP ) {
		printf( "NOP\n" );
	} else if( ins_data.type == XIT_UNKNOWN ) {
		printf( "Unknown Instruction\n", inst );
	}
}
