#ifndef _IMG_H_
#define _IMG_H_
#include <stdio.h>
#include <stdlib.h>
#ifndef LONG
typedef long int LONG;
#endif
#ifndef DWORD
typedef unsigned long int DWORD;
#endif
#ifndef BYTE
typedef unsigned char BYTE;
#endif
#ifndef WORD
typedef unsigned short WORD;
#endif
#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif


class CImage{
public:
	CImage(const int w,const int h):m_buffer(NULL){Resize(w,h);}
	CImage():m_buffer(NULL){Resize(1,1);}
	int Width() const {return m_width;}
	int Height() const {return m_height;}
	void SetBuffer(void* src){memcpy(m_buffer,src,m_width*m_height*4);}
	const void* Buffer() const {return m_buffer;}
	void* Buffer(){return m_buffer;}
	//virtual void Resize(const int width,const int height);
	virtual void Free(){
		if(m_buffer != NULL)free(m_buffer);
		m_buffer = NULL;
	}
	virtual void Resize(const int width,const int height){
		Free();
		m_width=width;
		m_height=height;
		m_buffer=malloc(width*height*4);
	}
protected:
	void* m_buffer;
	int m_width,m_height;
	int putcLittleEndian2(unsigned short int d, FILE *s){
		putc(d & 0xFF, s);
		return putc(d >> CHAR_BIT, s);
	}
	int putcLittleEndian4(unsigned long d, FILE *s){
		putc(d & 0xFF, s);
		putc((d >> CHAR_BIT) & 0xFF, s);
		putc((d >> CHAR_BIT * 2) & 0xFF, s);
		return putc((d >> CHAR_BIT * 3) & 0xFF, s);
	}
	int putBmpHeader(FILE *s, int x, int y, int c) {
		unsigned long int bfOffBits=14 + 40;
		if (x <= 0 || y <= 0) { return 0; }
		if (s == NULL || ferror(s)) { return 0; }
		//BITMAP FILE HEADER(14byte)
		fputs("BM", s);
		putcLittleEndian4(bfOffBits + (unsigned long)(x * y), s);
		putcLittleEndian2(0, s);
		putcLittleEndian2(0, s);
		putcLittleEndian4(bfOffBits, s);

		putcLittleEndian4(40,s); // biSize
		putcLittleEndian4((unsigned long)x, s); // biWidth
		putcLittleEndian4((unsigned long)y, s); // biHeight
		putcLittleEndian2(1, s); // biPlanes
		putcLittleEndian2(c, s); // biBitCount
		putcLittleEndian4(0, s); // biCompression
		putcLittleEndian4(0, s); // biSizeImage
		putcLittleEndian4(0, s); // biXPelsPerMeter
		putcLittleEndian4(0, s); // biYPelsPerMeter
		putcLittleEndian4(0, s); // biClrUsed
		putcLittleEndian4(0, s); // biClrImportant
		if (ferror(s)) { return 0; }
		return 1;
	}
};

class CImage8:public CImage{

};

class CImage24:public CImage{

};

#endif