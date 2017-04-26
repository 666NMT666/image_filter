#include <stdio.h>
#include <stdint.h>
#include "img32.h"

#ifndef _IMG32_CPP_
#define _IMG32_CPP_


CImage32::CImage32(char* file){
	if(file==NULL)return;
	else {
		m_buffer=NULL;
		load(file);
	}
}
CImage32::CImage32(const int w,const int h):CImage(w,h){}
CImage32::CImage32():CImage(1,1){}
CImage32::~CImage32(){Free();}
void CImage32::Free(){
	if(m_buffer != NULL)free(m_buffer);
	m_buffer = NULL;
}
void CImage32::Resize(const int width,const int height){
	Free();
	m_width=width; m_height=height;
	m_buffer=malloc(width*height*4);
}
void* CImage32::PixelAddress(const int x,const int y){
	return (x<0 || x>=m_width || y<0 || y>=m_height) ? NULL : PixelAddressNC(x,y);
}
const void* CImage32::PixelAddress(const int x,const int y) const {
	return (x<0 || x>=m_width || y<0 || y>=m_height) ? NULL : PixelAddressNC(x,y);
}
void CImage32::PixelSet(const int x,const int y,const DWORD color){
	DWORD* ptr = (DWORD*)PixelAddress(x,y);
	if(ptr!=NULL)*ptr=color;
}
void CImage32::PixelSetNC(const int x,const int y, const DWORD color){
	DWORD* ptr = (DWORD*)PixelAddress(x,y);
	*ptr=color;
}
DWORD CImage32::PixelGet(const int x,const int y) const{
	DWORD* ptr = (DWORD*)PixelAddress(x,y);
	return (ptr==NULL) ? 0:*ptr;
}
DWORD CImage32::PixelGetNC(const int x,const int y) const{return *(DWORD*)PixelAddress(x,y);}

void CImage32::save(const char* fname){
	int imgsize=(m_width*4)*m_height;
	BYTE *buf,*buf_top;
	FILE* fp;
 	
 	#ifndef fopen_s
	if ((fp = fopen(fname, "wb")) == NULL) {return;}
 	#else
	errno_t err;
	if ((err = fopen_s(&fp, fname, "wb")) != 0) { return; }
	#endif
	
	putBmpHeader(fp,m_width, m_height,32);
	buf_top = buf = (BYTE*)malloc(imgsize);
	if(buf_top == NULL) return;
	for(int i=m_height-1;i>=0;i--){
		for(int j=0;j<m_width;j++){
			TARGB c;
			c.ARGB=PixelGet(j,i);
			*(buf++)=(BYTE)c.B;
			*(buf++)=(BYTE)c.G;
			*(buf++)=(BYTE)c.R;
			*(buf++)=(BYTE)c.A;
		}
	}
	if(fwrite((void*)buf_top,1,imgsize,fp) != imgsize){
		free(buf_top);
		return;
	}
	free(buf_top);
	fclose(fp);
}
//#include <windows.h>
// void CImage32::load(char* fname,TImageType t){}
void CImage32::load(const char* fname){
	FILE *fin;
	BYTE *p;
    WORD biBitCount;
	DWORD dummy;
	#ifndef fopen_s
	if ((fin = fopen(fname, "rb")) == NULL) {exit(0);}
	#else
	errno_t err;
	if((err=fopen_s(&fin ,fname, "rb"))!=0){
		perror(fname);
		exit(1);
	}
	#endif
	//BITMAPFILEHEADER
	fread(&dummy, sizeof(WORD), 1, fin);
	fread(&dummy, sizeof(DWORD), 1, fin);
	fread(&dummy, sizeof(WORD), 1, fin);
	fread(&dummy, sizeof(WORD), 1, fin);
	fread(&dummy, sizeof(DWORD), 1, fin);
	//BITMAPINFOHEADER
	fread(&dummy, sizeof(DWORD), 1, fin);
	fread(&m_width, sizeof(LONG), 1, fin);
	fread(&m_height, sizeof(LONG), 1, fin);
	fread(&dummy, sizeof(WORD), 1, fin);
	fread(&biBitCount, sizeof(WORD), 1, fin);
	fread(&dummy, sizeof(DWORD), 1, fin);
	fread(&dummy, sizeof(DWORD), 1, fin);
	fread(&dummy, sizeof(LONG), 1, fin);
	fread(&dummy, sizeof(LONG), 1, fin);
	fread(&dummy, sizeof(DWORD), 1, fin);
	fread(&dummy, sizeof(DWORD), 1, fin);
	// free
	if(m_buffer != NULL)free(m_buffer);
	m_buffer = (void*)malloc(m_width*m_height*4);
	// read data
	p = (BYTE*)m_buffer+m_width*(m_height-1)*4;
	int pad=4-(m_width*3)%4;
	if(pad==4)pad=0;
	if (biBitCount == 24) {
		BYTE rgb[3];
		for (int i = 0; i < m_height; i++) {
			p=(BYTE*)m_buffer+m_width*(m_height-1-i)*4;
			for (int j=0;j<m_width;j++) {
				fread(&rgb, 3*sizeof(BYTE),1,fin);
				for(int k=0;k<3;k++){*(p++)=rgb[k];}
				*(p++) = 0xff;
			}
			if(m_width%4!=0){
				for (int j=0;j<pad;j++)fread(&dummy,1,1,fin);
			}
		}
	}else if(biBitCount == 32){
		BYTE rgb[4];
		for (int i = 0; i < m_height; i++) {
			p = (BYTE*)m_buffer+m_width*(m_height-1-i)*4;
			for (int j = 0; j < m_width; j++) {
				fread(&rgb, 4 * sizeof(BYTE), 1, fin);
				for (int k = 0; k<3; k++) { *(p++) = rgb[k]; }
				*(p++) = 0xff;
				//fread(&rgb, 4*sizeof(BYTE), 1, fin);
				//for(int k=0;k<4;k++)*(p++)=rgb[k];
			}
		}
	}
	fclose(fin);
}

#endif
