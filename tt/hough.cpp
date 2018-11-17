#include "Hough.h"
#include "math.h"
#include "Windows.h"
#include "stdio.h"

//该函数实现了极坐标系线检测，分辨率由float radiusResolution, float angleResolution决定
void HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    float *radius, float *angle)
{
	//申请循环变量
	int i, j;

	//清理变换空间累加数组
	for(i=0;i<houghHeight;i++)
	{
		for(j=0;j<houghWidth;j++)
		{
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//循环变量
	int r, a;
	//中间变量
	float tempR, tempA; 

	//遍历图像数据
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			//出现一个目标点    
			if(*(imgBinaryIn+i*width+j)==0)
			{

				//a代表角度的循环变量，在变换空间累加数组的垂直方向上
				for(a=0;a<houghHeight;a++)
				{

					//按照给定变换角度的分辨率，求取角度
					tempA=(a-houghHeight/2)*angleResolution;

					//根据当前遍历的角度及x,y值求取对应极半径
					tempR=(j-width/2)*cos(tempA*2*3.1415926/360)+(i-height/2)*sin(tempA*2*3.1415926/360);
					r=tempR/radiusResolution;

					//累加数组累加
					*(houghBuf+a*houghWidth+ r+houghWidth/2)+=1;
				}
			}
		}
	}
	//求累加数组的极大值，并记录此时的数组坐标
	int max, maxR, maxA;
	max=*(houghBuf+0*houghWidth+0);
	maxR=0;
	maxA=0;
	for(a=0;a<houghHeight;a++)
	{
		for(r=0;r<houghWidth;r++)
		{
			if(max<=*(houghBuf+a*houghWidth+r))
			{
				max=*(houghBuf+a*houghWidth+r);
				maxR=r;
				maxA=a;
			}
		}
	}
	//将极大值位置转换成极坐标半径和角度,并通过参数返回
	*radius=(maxR-houghWidth/2)*radiusResolution;
	*angle=(maxA-houghHeight/2)*angleResolution;
}

//该函数实现了直角坐标系角度在[-45，45]的线目标检测，斜率分辨率为0.1，截距为1个像素
void HoughTransform1(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float *skew, float *jieju)
{
	//申请循环变量
	int i, j;

	//清理变换空间累加数组
	for(i=0;i<houghHeight;i++)
	{
		for(j=0;j<houghWidth;j++)
		{
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//循环变量
	int p0, q0;
	//中间变量
	float p, q; 

	//遍历图像数据
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			//出现一个目标点    
			if(*(imgBinaryIn+i*width+j)==0)
			{

				//p0代表斜率循环变量，在变换空间累加数组的垂直方向上
				for(p0=0;p0<houghHeight;p0++)
				{

					//按照给定变换角度的分辨率，求取角度
					p=(p0-houghHeight/2)*0.1;

					//根据当前遍历的角度及x,y值求取对应极半径
					q=-p*j+i;
					q0=q;

					//累加数组累加
					*(houghBuf+p0*houghWidth+ q0+houghWidth/2)+=1;
				}
			}
		}
	}
	//求累加数组的极大值，并记录此时的数组坐标
	int max, maxP, maxQ;
	max=*(houghBuf+0*houghWidth+0);
	maxP=0;
	maxQ=0;
	for(p0=0;p0<houghHeight;p0++)
	{
		for(q0=0;q0<houghWidth;q0++)
		{
			if(max<=*(houghBuf+p0*houghWidth+q0))
			{
				max=*(houghBuf+p0*houghWidth+q0);
				maxP=p0;
				maxQ=q0;
			}
		}
	}
	//将极大值位置转换成极坐标半径和角度,并通过参数返回
	*skew=(maxP-houghHeight/2)*0.1;
	*jieju=maxQ-houghWidth/2;

}

//输入图像数据，获得直线斜率
float skewByHough(unsigned char *imgBuf, int width, int height)
{
	//累加数组宽，截距坐标
   int houghWidth=2*height+1;

	//线的角度在[-45,45]之间,斜率分辨率为0.1，所以申请的累加数组高度为
	int houghHeight=2001;

	//申请累加数组缓冲区
  int *houghBuf=new int[houghWidth*houghHeight];
  float slop; 
  float jieju;
  HoughTransform1(imgBuf, width, height, houghBuf, houghWidth, houghHeight,
		    &slop,&jieju);
  return slop;
}