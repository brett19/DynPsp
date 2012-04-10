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
	void _writeInt( unsigned int val1 ) {
		*((unsigned int*&)_cursor)++ = val1;
	}

protected:
	unsigned char *_buffer;
	unsigned char *_cursor;
	unsigned int _length;

// INSTRUCTIONS //
public:
	void aRETN( ) { _writeBytes(0xC3); };
	void aMOV_EAX_IMM( unsigned int imm ) { _writeBytes(0xB8); _writeInt(imm); }

};