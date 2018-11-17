#include "stdio.h"
#include "Windows.h"
#include"hough.h"
#include "math.h"
//给定一个图像文件及其路径，读入图像数据。 
unsigned char *readBmp(char *bmpName, int *width, int *height, int *byteCount)
{
	//打开文件，
  FILE *fp=fopen(bmpName,"rb");
  if(fp==0) return 0;
  //跳过文件头
  fseek(fp, sizeof(BITMAPFILEHEADER),0);

  //读入信息头
  int w, h, b;
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
  w = head.biWidth;
  h = head.biHeight;
	b = head.biBitCount/8;
  int lineByte=(w * b+3)/4*4;

  //跳过颜色表
  if(b==1)
    fseek(fp, 1024,1);

  //图像数据
  unsigned char *imgBuf=new unsigned char[w * h * b];
  for(int i=0;i<h;i++)
  {
    fread(imgBuf+i*w*b,w*b, 1,fp);
    fseek(fp, lineByte-w*b, 1);
  }
  fclose(fp);

  *width=w,  *height=h, *byteCount=b;

  return imgBuf;
}


bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int byteCount)
{
  if(!imgBuf)
    return 0;

  //灰度图像颜色表空间1024，彩色图像没有颜色表
  int palettesize=0;
  if(byteCount==1) palettesize=1024;

  //一行象素子节数为4的倍数
  int lineByte=(width * byteCount+3)/4*4;

  FILE *fp=fopen(bmpName,"wb");
  if(fp==0) return 0;

  //填写文件头
  BITMAPFILEHEADER fileHead;
  fileHead.bfType = 0x4D42;
  fileHead.bfSize= 
    sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ palettesize + lineByte*height;
  fileHead.bfReserved1 = 0;
  fileHead.bfReserved2 = 0;
  fileHead.bfOffBits=54+palettesize;
  fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);
 
  // 填写信息头
  BITMAPINFOHEADER head; 
  head.biBitCount=byteCount*8;
  head.biClrImportant=0;
  head.biClrUsed=0;
  head.biCompression=0;
  head.biHeight=height;
  head.biPlanes=1;
  head.biSize=40;
  head.biSizeImage=lineByte*height;
  head.biWidth=width;
  head.biXPelsPerMeter=0;
  head.biYPelsPerMeter=0;
  fwrite(&head, sizeof(BITMAPINFOHEADER),1, fp);

  //颜色表拷贝  
  if(palettesize==1024)
  {
    unsigned char palette[1024];
    for(int i=0;i<256;i++)
    {
      *(palette+i*4+0)=i;
      *(palette+i*4+1)=i;
      *(palette+i*4+2)=i;
      *(palette+i*4+3)=0;     
    }
    fwrite(palette, 1024,1, fp);
  }
  
  //准备数据并写文件
  unsigned char *buf=new unsigned char[height*lineByte];
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width*byteCount; j++)
      *(buf+i*lineByte+j)=*(imgBuf+i*width*byteCount+j);
  }
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;

  fclose(fp);

 return 1;
}

void main()
{

	
	char *pathIn="F:\\学习\\图像数据结构与算法\\实验3\\线带轻噪声30度_灰度.bmp";
	int width, height, byteCount;
	unsigned char *buf=readBmp(pathIn, &width, &height, &byteCount);
	unsigned char *greybuf=new[height][width];
	for(int i=0;i<width;i++){
		for(int j=0;j<height;j++){
		
		}
	}
	//输入图像数据，获得直线斜率
    float f=skewByHough(buf, width, height);//二值图像
	printf("宽=%d，高=%d，字节=%d\n",width, height, byteCount);
	printf("斜率%.2f\n",f);
	
	//该函数实现了极坐标系线检测，分辨率由float radiusResolution, float angleResolution决定
    int  houghWidth=sqrt(width*width+height*height);
	int houghHeight=181;
	int *houghBuf=new int[houghWidth*houghHeight];
	float radiusResolution=1,  angleResolution=1; 
	float radius, angle;
	HoughTransform(buf, width,height, houghBuf, houghWidth, houghHeight,
		    radiusResolution, angleResolution, &radius, &angle);
	printf("宽=%d，高=%d，字节=%d\n",width, height, byteCount);
	printf("角度=%.2f  及半径=%.2f\n",angle, radius);

	//把hough变换空间存成图像
  int i,j, m=0;
  for(i=0;i<houghHeight;i++){
	  for(j=0;j<houghWidth;j++){
		  if(m<houghBuf[i*houghWidth+j])
			  m=houghBuf[i*houghWidth+j];
	  }
  }
  unsigned char *buf1=new unsigned char[houghWidth*houghHeight];
  for(i=0;i<houghHeight;i++){
	  for(j=0;j<houghWidth;j++){
		  buf1[i*houghWidth+j]=houghBuf[i*houghWidth+j]*255/m;
	  }
  }
  char *bmpName="F:\\教学相关\\图像数据结构与算法\\2017年图像数据结构与算法课件\\实验3\\hough.bmp";
  saveBmp(bmpName, buf1, houghWidth, houghHeight, 1);
  float t=(angle+90)*2*3.1415926/360;
  float ff=sin(t)/cos(t);
  printf("角度=%.2f \n ",ff);//1657,1002
}
