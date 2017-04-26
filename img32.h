#ifndef _IMG32_H_
#define _IMG32_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
};

class CImage8:public CImage{

};

class CImage24:public CImage{

};

class CImage32:public CImage{
private:
	int putBmpHeader(FILE *s, int x, int y, int c);
	
public:
	union TARGB{
		DWORD ARGB;
		struct { BYTE B,G,R,A; };
	};
	CImage32();
	CImage32(char* file);
	CImage32(const int width,const int height);
	virtual ~CImage32();
	virtual void Free();
	virtual void Resize(const int width,const int height);
	virtual void load(const char* fname);
	virtual void save(const char* fname);

	void* PixelAddress(const int x,const int y);
	const void* PixelAddress(const int x,const int y) const;
	void* PixelAddressNC (const int x,const int y) { return (BYTE*)m_buffer + (m_width*y + x)*sizeof(DWORD); }
	const void* PixelAddressNC(const int x,const int y) const{ return (BYTE*)m_buffer+(m_width*y+x)*sizeof(DWORD);}
	void PixelSet(const int x,const int y,const DWORD color);
	void PixelSetNC(const int x,const int y,const DWORD color);
	DWORD PixelGet(const int x,const int y)const;
	DWORD PixelGetNC(const int x,const int y)const;
};

#endif