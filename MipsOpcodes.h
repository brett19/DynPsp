#pragma once

#pragma pack(push)
#pragma pack(1)

struct mips_opcode_jmp
{
	union
	{
		struct
		{
			unsigned int imm : 26;
			unsigned int op : 6;
		};
		struct
		{
			int simm : 26;
			int sop : 6;
		};
	};
};

struct mips_opcode_imm
{
	union
	{
		struct
		{
			unsigned int imm : 16;
			unsigned int rt : 5;
			unsigned int rs : 5;
			unsigned int op : 6;
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

struct mips_opcode_br
{
	union
	{
		struct
		{
			unsigned int imm : 16;
			unsigned int brop : 5;
			unsigned int rs : 5;
			unsigned int op : 6;
		};
		struct
		{
			int simm : 16;
			int sbrop : 5;
			int srs : 5;
			int sop : 6;
		};
	};
};

struct mips_opcode_sp
{
	unsigned int spop : 6;
	unsigned int sa : 5;
	unsigned int rd : 5;
	unsigned int rt : 5;
	unsigned int rs : 5;
	unsigned int op : 6;
};

struct mips_opcode_sys
{
	unsigned int spop : 6;
	unsigned int code : 20;
	unsigned int op : 6;
};

#pragma pack(pop)