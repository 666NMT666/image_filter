#include <iostream>
#include "img32.h"


int min(int a,int b,int c){
	if(a<b && a<c) return a;
	if(b<a && b<c) return b;
	return c;
}


BYTE* disision(BYTE* img,int w,int h){
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	tmp_max=0
	cmax=0
	th=0
	for (int i=1;i<255;i++){
		int ww=0;
		int wb=0;
		int mw=0;
		int mb=0;
		for(int y=0; y<h; y++){
			for(int x=0; x<w; x++){
				for(int i=0;i<3;i++){
					if (img[y*w*4+x*4+i] >border){
						ww=ww+1;
						mw=mw+img[y*w*4+x*4+i];
					} else {
						wb=wb+1;
						mb=mb+img[y*w*4+x*4+i];
					}
				}
			}
		}
		if (wb!=0 && ww!=0) tmp_max=(wb*ww)*((mw*wb-mb*ww)/(ww*wb))*((mw*wb-mb*ww)/(ww*wb));
		if (tmp_max>cmax){
			cmax=tmp_max;
			th=border;
		}
	}
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for(int i=0;i<3;i++){
				if (img[y*w*4+x*4+i] < th) dest[y*w*4+x*4+i]=0;
				else dest[y*w*4+x*4+i]=255;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* normalize(BYTE* img,int w,int h){
	int minimum=255;
	int maximum=0;
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for (int i=0;i<3;i++){
				if (img[y*w*3+x*3+i] > maximum) maximum=img[y*w*4+x*4+i];
				if (img[y*w*4+x*4+i] < minimum) minimum=img[y*w*4+x*4+i];
		    }
		}
	}

	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for (int i=0;i<3;i++){
				dest[y*w*4+x*4+i]=255*(img[y*w*4+x*4+i]-minimum)/(maximum-minimum);
			}
		}
	}
	return dest;
}

BYTE* diff(BYTE* img1,BYTE* img2,int w,int h){
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			int tmps[3]={0,0,0};
			for (int i=0;i<3;i++){
				tmps[i] = 255-(int)(abs(img1[y*w*4+x*4+i]-img2[y*w*4+x*4+i]));
			}
			int tmp=min(tmps[0],tmps[1],tmps[2]);
			if (tmp<0)tmp=0;
			for (int i=0;i<3;i++){
				dest[y*w*4+x*4+i]=tmp;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* average(BYTE* img,int w,int h,int s){
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for (int i=0;i<3;i++){
				int tmp=0;
				for (int y2=y-s/2; y2 <= y+s/2; y2++){
					for (int x2=x-s/2; x2 <= x+s/2; x2++){
						if (y2>=0 && y2 < h && x2>=0 && x2<w){
							tmp+=img[y*w*4+x*4+i];
						}
					}
				}
				dest[y*w*4+x*4+i]=tmp/(s*s);
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* laplace2(BYTE* img,int w,int h) {
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=1; y<h-1; y++){
		for(int x=1; x<w-1; x++){
			int tmps[3]={0,0,0};
			for (int i=0;i<3;i++){
				int tmp1 = img[y*w*4+x*4+i];
				int tmp2 = img[y*w*4+(x-1)*4+i];
				int tmp3 = img[y*w*4+(x+1)*4+i];
				int tmp4 = img[(y-1)*w*4+x*4+i];
				int tmp5 = img[(y+1)*w*4+x*4+i];
				tmps[i] = 255-(int)(abs(tmp2+tmp3+tmp4+tmp5-tmp1*4));
			}
			int tmp = min(tmps[0],tmps[1],tmps[2]);
			if (tmp<0) tmp=0;
			for (int i=0;i<3;i++){
				dest[y*w*4+x*4+i]=tmp;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
    return dest;
}

BYTE* min_filter(BYTE* img,int w,int h,int s){
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for (int i=0;i<3;i++){
				int tmp=255;
				for (int y2=y-s/2; y2<=y+s/2;y2++){
					for (int x2=x-s/2; x2<=x+s/2;x2++){
						if (y2>=0 && y2 < h && x2>=0 && x2<w && tmp>img[y2*w*4+x2*4+i])tmp=img[y2*w*4+x2*4+i];
					}
				}
				dest[y*w*4+x*4+i]=tmp;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* max_filter(BYTE* img,int w,int h,int s){
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for (int i=0;i<3;i++){
				int tmp=0;
				for (int y2=y-s/2; y2<=y+s/2;y2++){
					for (int x2=x-s/2; x2<=x+s/2;x2++){
						if (y2>=0 && y2 < h && x2>=0 && x2<w && tmp<img[y2*w*4+x2*4+i])tmp=img[y2*w*4+x2*4+i];
					}
				}
				dest[y*w*4+x*4+i]=tmp;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* median(BYTE* img,int w,int h,int s){
	BYTE* dest=(BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for (int i=0;i<3;i++){
				int *tmps=new int[s*s];
				int count=0;
				for (int y2=y-s/2; y2<=y+s/2;y2++){
					for (int x2=x-s/2; x2<=x+s/2;x2++){
						if (y2>=0 && y2 < h && x2>=0 && x2<w)tmps[count]=img[y2*w*4+x2*4+i];
						count=count+1;
					}
				}
				for (int j=0;j<s*s-1;j++){
					for (int k=j;k<s*s;k++){
						if(tmps[j]<tmps[k]){
							int t=tmps[j];
							tmps[j]=tmps[k];
							tmps[k]=t;
						}
					}
				}
				dest[y*w*4+x*4+i]=tmps[(s*s)/2];
				delete[] tmps;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* edge(BYTE* img,int w,int h){
	BYTE* dest = (BYTE*)malloc(4*w*h*sizeof(BYTE));
	BYTE* dest2 = (BYTE*)malloc(4*w*h*sizeof(BYTE));

	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			int tmp = 0;
			for (int i=0;i<3;i++){
				tmp += img[4*y*w+4*x+i];	
			}
			tmp=tmp/3;
			for (int i=0;i<3;i++){
				dest[y*w*3+x*3+i]=tmp;
			}
		}
	}
	for (int y=1;y<h-1;y++){
		for (int x=1;x<w-1;x++){
			int tmp = 255-(int)(fabs(dest[y*w*3+(x-1)*3]+dest[y*w*3+(x+1)*3]+dest[(y-1)*w*3+x*3]+dest[(y+1)*w*3+x*3]-dest[y*w*3+x*3]*4));
			if (tmp<0)tmp=0;
			for (int i=0;i<3;i++){
				dest2[y*w*4+x*4+i]=tmp;
			}
			dest2[y*w*4+x*4+3]=255;
		}	
	}
	return dest2;
}

BYTE* hist(BYTE* img,int w,int h){
	BYTE* dest = (BYTE*)malloc(4*w*h*sizeof(BYTE));
	int hn[256][3];
	int ehn[256][3];
	for (int j=0;j<256;j++){
		for (int i=0;i<3;i++){
			hn[j][i]=0;
			ehn[j][i]=0;
		}
	}
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for(int i=0;i<3;i++){
				for(int j=0;j<256;j++){
					if (j==img[4*y*w+4*x+i]){hn[j][i]=hn[j][i]+1;}
				}
			}
		}
	}
	int s[3]={0,0,0};
	for(int i=0;i<3;i++){
		for(int j=0;j<256;j++){
			s[i]=s[i]+hn[j][i];
			ehn[j][i]=s[i]*255/(w*h);
		}
	}
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for(int i=0;i<3;i++){
				for(int j=0;j<256;j++){
					if (j==img[4*y*w+4*x+i])dest[y*w*4+x*4+i]=ehn[j][i];
				}
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* contrast(BYTE* img,int w,int h,int a,int b){
	BYTE* dest = (BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for(int i=0;i<3;i++){
				int tmp=img[4*y*w+4*x+i];
				if (tmp<a)dest[y*w*4+x*4+i]=0;
				else if (tmp>b)dest[y*w*4+x*4+i]=255;
				else dest[y*w*4+x*4+i]=255*(tmp-a)/(b-a);
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* postarize(BYTE* img,int w,int h,int q){
	BYTE* dest = (BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for(int i=0;i<3;i++){
				int tmp=img[4*y*w+4*x+i];
				for(int k=0;k<q;k++){
					int t1=k*(255/q);
					int t2=(k+1)*(255/q);
					if (t1<=tmp && tmp<t2){
						tmp=(t1+t2)/2;
					}
				}
				dest[y*w*4+x*4+i]=tmp;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

BYTE* sola(BYTE* img,int w,int h){
	BYTE* dest = (BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			for(int i=0;i<3;i++){
				int tmp=img[4*y*w+4*x+i];
				if (tmp<128) tmp=2*tmp;
				else tmp=2*255-tmp*2;
				dest[y*w*4+x*4+i]=tmp;
			}
			dest[y*w*4+x*4+3]=255;
        }
    }
	return dest;
}

BYTE* gray(BYTE* img,int w,int h){
	BYTE* dest = (BYTE*)malloc(4*w*h*sizeof(BYTE));
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			int tmp=0;
			for(int i=0;i<3;i++){
				tmp+=img[4*y*w+4*x+i];
			}
			for(int i=0;i<3;i++){
				dest[y*w*4+x*4+i]=tmp/3;
			}
			dest[y*w*4+x*4+3]=255;
		}
	}
	return dest;
}

int main(){
	CImage32 src("saba.bmp");
	BYTE* buf=postarize((BYTE*)src.Buffer(),src.Width(),src.Height(),3);
	BYTE* buf2=median(edge(buf,src.Width(),src.Height()),src.Width(),src.Height(),3);
	src.SetBuffer(buf2);
	src.save("pos-edge-median.bmp");
	return 0;
}