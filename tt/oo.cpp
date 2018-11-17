#include "stdio.h"
#include "Windows.h"
//给定一个图像文件及其路径，读入图像数据。 
unsigned char *readBmp(char *bmpName, int *width, int *height, int *biBitCount)
{
  FILE *fp=fopen(bmpName,"rb");
  if(fp==0) return 0;
  fseek(fp, sizeof(BITMAPFILEHEADER),0);

  int w, h, b;
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
  w = head.biWidth;
  h = head.biHeight;
	b = head.biBitCount/8;
  int lineByte=(w * b+3)/4*4;

  if(b==1)
    fseek(fp, 1024,1);
  unsigned char *imgBuf=new unsigned char[w * h * b];
  for(int i=0;i<h;i++)
  {
    fread(imgBuf+(h-i-1)*w*b,w*b, 1,fp);
    fseek(fp, lineByte-w*b, 1);
  }
  fclose(fp);

  *width=w,  *height=h, *biBitCount=b;

  return imgBuf;
}


bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount)
{
  if(!imgBuf)
    return 0;
  int palettesize=0;
  if(biBitCount==1) palettesize=1024;

  int lineByte=(width * biBitCount+3)/4*4;

  FILE *fp=fopen(bmpName,"wb");
  if(fp==0) return 0;

  BITMAPFILEHEADER fileHead;
  fileHead.bfType = 0x4D42;
  fileHead.bfSize= 
    sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ palettesize + lineByte*height;
  fileHead.bfReserved1 = 0;
  fileHead.bfReserved2 = 0;
  fileHead.bfOffBits=54+palettesize;
  fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);
 
  // 填写文件头
  BITMAPINFOHEADER head; 
  head.biBitCount=biBitCount*8;
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

  //调色板拷贝  
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
  
  unsigned char *buf=new unsigned char[height*lineByte];
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width*biBitCount; j++)
      *(buf+i*lineByte+j)=*(imgBuf+(height-1-i)*width*biBitCount+j);
  }
  
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;

  fclose(fp);

 return 1;
}

void main()
{
//	clock_t start,finish;
//  start=clock();
	unsigned char *imgbuf;
	int width, height, biBitCount;
	char readPath[]="E:\\autocell\\CHEN2-7.BMP";
	imgbuf=readBmp(readPath, &width, &height, &biBitCount);
	for(int i=0;i<height;i++){
		for(int j=0;j<width*biBitCount;j++)
			*(imgbuf+i*width*biBitCount+j)=0;
	}
	printf("width=%d,height=%d,byte=%d\n",width,height,biBitCount);
	char writePath[]="E:\\autocell\\CHEN2-7cpy.BMP";
	saveBmp(writePath, imgbuf, width, height, biBitCount);

}
