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

#pragma pack(pop)