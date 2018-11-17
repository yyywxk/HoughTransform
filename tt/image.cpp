#include"image.h"
#include "Windows.h"
#include "stdio.h"
//���캯����ɳ�ʼ������
imageClass::imageClass()
{
	width=0, height=0, byteCount=0;
	imageBuf=0;
}
//�������������ֳ��ͷ���Դ
imageClass::~imageClass()
{
	
	if(imageBuf!=0)
		delete []imageBuf;
}
//���ļ�����
void imageClass::readFile(char *filePath)
{

	//���ļ�
    FILE *fp=fopen(filePath,"rb");
	 if(fp==0) 
		 return ;
  //�����ļ�ͷ
  fseek(fp, sizeof(BITMAPFILEHEADER),0);

  //������Ϣͷ
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
  width = head.biWidth;
  height = head.biHeight;
  byteCount = head.biBitCount/8;
  int lineByte=(width * byteCount+3)/4*4;

  //������ɫ��
  if(byteCount==1)
    fseek(fp, 1024,1);

  //ͼ������
  if(imageBuf!=0){//˵��ǰ��򿪵��ļ�������ռ䣬Ҫ���ͷŵ���Ȼ���ٷ����µ�
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

//д�ļ�
void imageClass::saveFile(char *filePath)
{
	if(!imageBuf)
    return;

  //�Ҷ�ͼ����ɫ��ռ�1024����ɫͼ��û����ɫ��
  int palettesize=0;
  if(byteCount==1) palettesize=1024;

  //һ�������ӽ���Ϊ4�ı���
  int lineByte=(width * byteCount+3)/4*4;

  FILE *fp=fopen(filePath,"wb");
  if(fp==0) return;

  //��д�ļ�ͷ
  BITMAPFILEHEADER fileHead;
  fileHead.bfType = 0x4D42;
  fileHead.bfSize= 
    sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ palettesize + lineByte*height;
  fileHead.bfReserved1 = 0;
  fileHead.bfReserved2 = 0;
  fileHead.bfOffBits=54+palettesize;
  fwrite(&fileHead, sizeof(BITMAPFILEHEADER),1, fp);
 
  // ��д��Ϣͷ
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

  //��ɫ����  
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
  
  //׼�����ݲ�д�ļ�
  unsigned char *buf=new unsigned char[height*lineByte];
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width*byteCount; j++)
      *(buf+i*lineByte+j)=*(imageBuf+i*width*byteCount+j);
  }
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;//����ռ�

  fclose(fp);
}

//��ȡͼ���
int imageClass::getWidth()
{
	return width;
}
//��ȡͼ���
int imageClass::getHeight()
{
	return height;
}
//��ȡͼ��ÿ�����ֽ���
int imageClass::getByteCount()
{
	return byteCount;
}
//��ȡͼ������ָ��
unsigned char * imageClass::getImgData()
{
	return imageBuf;
}