#pragma once

enum x86regs {
	XREG_EAX,
	XREG_ECX,
	XREG_ESI
};

class X86Assembler
{
public:
	X86Assembler( unsigned char *buffer, unsigned int length )
		: _buffer(buffer), _cursor(buffer), _length(length)
	{
	}

	void * getCursor( ) { return _cursor; }

private:
	void _writeBytes( unsigned char val1 ) {
		*_cursor++ = val1;
	}
	void _writeBytes( unsigned char val1, unsigned char val2 ) {
		*_cursor++ = val1;
		*_cursor++ = val2;
	}
	void _writeBytes( unsigned char val1, unsigned char val2, unsigned char val3 ) {
		*_cursor++ = val1;
		*_cursor++ = val2;
		*_cursor++ = val3;
	}
	void _writeUint( unsigned int val1 ) {
		*((unsigned int*&)_cursor)++ = val1;
	}
	void _writeInt( int val1 ) {
		*((int*&)_cursor)++ = val1;
	}

protected:
	unsigned char *_buffer;
	unsigned char *_cursor;
	unsigned int _length;

// INSTRUCTIONS //
public:
	void aNOP( ) { _writeBytes(0x90); };
	void aINT3( ) { _writeBytes(0xCC); };
	void aRETN( ) { _writeBytes(0xC3); };
	
	void aJMP_IMM( unsigned int imm ) { _writeBytes(0xE9); _writeInt((int)imm-(int)_cursor-4); }
	void aCALL_IMM( unsigned int imm ) { _writeBytes(0xE8); _writeInt((int)imm-(int)_cursor-4); }

	void aPUSH_EAX( ) { _writeBytes(0x50); };

	void aMOV_EAX_ECX( ) { _writeBytes(0x89,0xc1); }
	void aMOV_EAX_uIMM( unsigned int imm ) { _writeBytes(0xB8); _writeUint(imm); }
	void aMOV_EAX_pESIoIMM( int off ) { _writeBytes(0x8b,0x86); _writeInt(off); }
	void aMOV_ECX_pESIoIMM( int off ) { _writeBytes(0x8b,0x8e); _writeInt(off); }
	void aMOV_pEAX_ECX( ) { _writeBytes(0x89,0x08); }
	void aMOV_pESIoIMM_EAX( int off ) { _writeBytes(0x89,0x86); _writeInt(off); }
	void aMOV_pESIoIMM_uIMM( int off, unsigned int imm ) { _writeBytes(0xC7,0x86); _writeInt(off); _writeUint(imm); }

	void aADD_EAX_iIMM( int imm ) { _writeBytes(0x05); _writeInt(imm); }
	void aADD_EAX_pESIoIMM( int off ) { _writeBytes(0x03,0x86); _writeInt(off); }
	void aADD_pESIoIMM_iIMM( int off, int imm ) { _writeBytes(0x81,0x86); _writeInt(off); _writeInt(imm); }
};