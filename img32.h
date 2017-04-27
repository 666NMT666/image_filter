#ifndef _IMG32_H_
#define _IMG32_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"
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



class CImage32:public CImage{
private:
	
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