#pragma once

#pragma pack(push)
#pragma pack(1)

struct elf_header {
	struct {
		unsigned int mMagic;
		unsigned char mClass;
		unsigned char mData;
		unsigned char mVersion;
		unsigned char mPadding[9];
	} mIdent;

	unsigned short mType;
	unsigned short mMachine;
	unsigned int mVersion;
	unsigned int mEntryPoint;
	unsigned int mPHOffset;
	unsigned int mSHOffset;
	unsigned int mFlags;
	unsigned short mEHSize;
	unsigned short mPHEntrySize;
	unsigned short mPHEntryCount;
	unsigned short mSHEntrySize;
	unsigned short mSHEntryCount;
	unsigned short mSHStringIndex;//String Table

	static const int MAGIC = 0x464C457F;
};

struct elf_programheader {
	unsigned int mType;
	long mOffset;
	unsigned int mVAddress;
	unsigned int mPAddress;
	unsigned int mFileSize;
	unsigned int mMemorySize;
	unsigned int mFlags;
	unsigned int mAlign;
};

enum elf_sectionheader_type {
	SHT_NULL = 0x00000000,
	SHT_PROGBITS = 0x00000001,
	SHT_STRTAB = 0x00000003,
	SHT_NOBITS = 0x00000008,
	SHT_PRXREL = 0x700000A0,
};

struct elf_sectionheader {
	unsigned int mName;
	unsigned int mType;
	unsigned int mFlags;
	unsigned int mAddress;
	unsigned int mOffset;
	unsigned int mSize;
	unsigned int mLink;
	unsigned int mInfo;
	unsigned int mAddressAlign;
	unsigned int mEntrySize;
};

struct elf_relocation {
	unsigned int mOffset;
	unsigned char mType;
	unsigned char mOffsetBase;
	unsigned char mAddrBase;
	unsigned char mUnknown;	
};

#pragma pack(pop)