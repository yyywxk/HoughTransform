#include "stdio.h"
#include "Windows.h"
#include"hough.h"
#include "math.h"
//����һ��ͼ���ļ�����·��������ͼ�����ݡ� 
unsigned char *readBmp(char *bmpName, int *width, int *height, int *byteCount)
{
	//���ļ���
  FILE *fp=fopen(bmpName,"rb");
  if(fp==0) return 0;
  //�����ļ�ͷ
  fseek(fp, sizeof(BITMAPFILEHEADER),0);

  //������Ϣͷ
  int w, h, b;
  BITMAPINFOHEADER head;
  fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
  w = head.biWidth;
  h = head.biHeight;
	b = head.biBitCount/8;
  int lineByte=(w * b+3)/4*4;

  //������ɫ��
  if(b==1)
    fseek(fp, 1024,1);

  //ͼ������
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

  //�Ҷ�ͼ����ɫ��ռ�1024����ɫͼ��û����ɫ��
  int palettesize=0;
  if(byteCount==1) palettesize=1024;

  //һ�������ӽ���Ϊ4�ı���
  int lineByte=(width * byteCount+3)/4*4;

  FILE *fp=fopen(bmpName,"wb");
  if(fp==0) return 0;

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
      *(buf+i*lineByte+j)=*(imgBuf+i*width*byteCount+j);
  }
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;

  fclose(fp);

 return 1;
}

void main()
{

	
	char *pathIn="F:\\ѧϰ\\ͼ�����ݽṹ���㷨\\ʵ��3\\�ߴ�������30��_�Ҷ�.bmp";
	int width, height, byteCount;
	unsigned char *buf=readBmp(pathIn, &width, &height, &byteCount);
	unsigned char *greybuf=new[height][width];
	for(int i=0;i<width;i++){
		for(int j=0;j<height;j++){
		
		}
	}
	//����ͼ�����ݣ����ֱ��б��
    float f=skewByHough(buf, width, height);//��ֵͼ��
	printf("��=%d����=%d���ֽ�=%d\n",width, height, byteCount);
	printf("б��%.2f\n",f);
	
	//�ú���ʵ���˼�����ϵ�߼�⣬�ֱ�����float radiusResolution, float angleResolution����
    int  houghWidth=sqrt(width*width+height*height);
	int houghHeight=181;
	int *houghBuf=new int[houghWidth*houghHeight];
	float radiusResolution=1,  angleResolution=1; 
	float radius, angle;
	HoughTransform(buf, width,height, houghBuf, houghWidth, houghHeight,
		    radiusResolution, angleResolution, &radius, &angle);
	printf("��=%d����=%d���ֽ�=%d\n",width, height, byteCount);
	printf("�Ƕ�=%.2f  ���뾶=%.2f\n",angle, radius);

	//��hough�任�ռ���ͼ��
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
  char *bmpName="F:\\��ѧ���\\ͼ�����ݽṹ���㷨\\2017��ͼ�����ݽṹ���㷨�μ�\\ʵ��3\\hough.bmp";
  saveBmp(bmpName, buf1, houghWidth, houghHeight, 1);
  float t=(angle+90)*2*3.1415926/360;
  float ff=sin(t)/cos(t);
  printf("�Ƕ�=%.2f \n ",ff);//1657,1002
}
