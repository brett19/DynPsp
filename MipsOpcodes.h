#pragma once

#pragma pack(push)
#pragma pack(1)

struct mips_opcode_alimm
{
	union
	{
		struct
		{
			unsigned int uimm : 16;
			unsigned int urt : 5;
			unsigned int urs : 5;
			unsigned int uop : 6;
		};
		struct
		{
			int simm : 16;
			int srt : 5;
			int srs : 5;
			int sop : 6;
		};
	};
};

struct mips_opcode_alreg
{
	unsigned int uspop : 6;
	unsigned int usa : 5;
	unsigned int urd : 5;
	unsigned int urt : 5;
	unsigned int urs : 5;
	unsigned int uop : 6;
};

struct mips_opcode_alsys
{
	unsigned int uspop : 6;
	unsigned int ucode : 20;
	unsigned int uop : 6;
};

struct mips_opcode_aljmp
{
	union
	{
		struct
		{
			unsigned int uimm : 26;
			unsigned int uop : 6;
		};
		struct
		{
			int simm : 26;
			int sop : 6;
		};
	};
};

struct mips_opcode
{
	mips_opcode( unsigned int _val ) : val( _val ) { }
	union {
		unsigned int val;
		mips_opcode_alimm alimm;
		mips_opcode_alreg alreg;
		mips_opcode_alsys alsys;
		mips_opcode_aljmp aljmp;
	};
};

#pragma pack(pop)