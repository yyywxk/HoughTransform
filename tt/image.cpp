#include"image.h"
#include "Windows.h"
#include "stdio.h"
//构造函数完成初始化工作
imageClass::imageClass()
{
	width=0, height=0, byteCount=0;
	imageBuf=0;
}
//析构函数清理现场释放资源
imageClass::~imageClass()
{
	
	if(imageBuf!=0)
		delete []imageBuf;
}
//读文件进来
void imageClass::readFile(char *filePath)
{

	//打开文件
    FILE *fp=fopen(filePath,"rb");
	 if(fp==0) 
		 return ;
  //跳过文件头
  fseek(fp, sizeof(BITMAPFILEHEADER),0);

  //读入信息头
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
  width = head.biWidth;
  height = head.biHeight;
  byteCount = head.biBitCount/8;
  int lineByte=(width * byteCount+3)/4*4;

  //跳过颜色表
  if(byteCount==1)
    fseek(fp, 1024,1);

  //图像数据
  if(imageBuf!=0){//说明前面打开的文件申请过空间，要先释放掉，然后再分配新的
	  delete []imageBuf;
	  imageBuf=0;
  }
  imageBuf=new unsigned char[width * height * byteCount];
  for(int i=0;i<height;i++)
  {
    fread(imageBuf+i*width*byteCount,width*byteCount, 1,fp);
    fseek(fp, lineByte-width*byteCount, 1);
  }
  fclose(fp);

}

//写文件
void imageClass::saveFile(char *filePath)
{
	if(!imageBuf)
    return;

  //灰度图像颜色表空间1024，彩色图像没有颜色表
  int palettesize=0;
  if(byteCount==1) palettesize=1024;

  //一行象素子节数为4的倍数
  int lineByte=(width * byteCount+3)/4*4;

  FILE *fp=fopen(filePath,"wb");
  if(fp==0) return;

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
      *(buf+i*lineByte+j)=*(imageBuf+i*width*byteCount+j);
  }
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;//清理空间

  fclose(fp);
}

//获取图像宽
int imageClass::getWidth()
{
	return width;
}
//获取图像高
int imageClass::getHeight()
{
	return height;
}
//获取图像每像素字节数
int imageClass::getByteCount()
{
	return byteCount;
}
//获取图像数据指针
unsigned char * imageClass::getImgData()
{
	return imageBuf;
}