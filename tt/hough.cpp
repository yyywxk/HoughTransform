#include "Hough.h"
#include "math.h"
#include "Windows.h"
#include "stdio.h"

//�ú���ʵ���˼�����ϵ�߼�⣬�ֱ�����float radiusResolution, float angleResolution����
void HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    float *radius, float *angle)
{
	//����ѭ������
	int i, j;

	//����任�ռ��ۼ�����
	for(i=0;i<houghHeight;i++)
	{
		for(j=0;j<houghWidth;j++)
		{
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//ѭ������
	int r, a;
	//�м����
	float tempR, tempA; 

	//����ͼ������
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			//����һ��Ŀ���    
			if(*(imgBinaryIn+i*width+j)==0)
			{

				//a����Ƕȵ�ѭ���������ڱ任�ռ��ۼ�����Ĵ�ֱ������
				for(a=0;a<houghHeight;a++)
				{

					//���ո����任�Ƕȵķֱ��ʣ���ȡ�Ƕ�
					tempA=(a-houghHeight/2)*angleResolution;

					//���ݵ�ǰ�����ĽǶȼ�x,yֵ��ȡ��Ӧ���뾶
					tempR=(j-width/2)*cos(tempA*2*3.1415926/360)+(i-height/2)*sin(tempA*2*3.1415926/360);
					r=tempR/radiusResolution;

					//�ۼ������ۼ�
					*(houghBuf+a*houghWidth+ r+houghWidth/2)+=1;
				}
			}
		}
	}
	//���ۼ�����ļ���ֵ������¼��ʱ����������
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
	//������ֵλ��ת���ɼ�����뾶�ͽǶ�,��ͨ����������
	*radius=(maxR-houghWidth/2)*radiusResolution;
	*angle=(maxA-houghHeight/2)*angleResolution;
}

//�ú���ʵ����ֱ������ϵ�Ƕ���[-45��45]����Ŀ���⣬б�ʷֱ���Ϊ0.1���ؾ�Ϊ1������
void HoughTransform1(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float *skew, float *jieju)
{
	//����ѭ������
	int i, j;

	//����任�ռ��ۼ�����
	for(i=0;i<houghHeight;i++)
	{
		for(j=0;j<houghWidth;j++)
		{
			*(houghBuf+i*houghWidth+j)=0;
		}
	}

	//ѭ������
	int p0, q0;
	//�м����
	float p, q; 

	//����ͼ������
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			//����һ��Ŀ���    
			if(*(imgBinaryIn+i*width+j)==0)
			{

				//p0����б��ѭ���������ڱ任�ռ��ۼ�����Ĵ�ֱ������
				for(p0=0;p0<houghHeight;p0++)
				{

					//���ո����任�Ƕȵķֱ��ʣ���ȡ�Ƕ�
					p=(p0-houghHeight/2)*0.1;

					//���ݵ�ǰ�����ĽǶȼ�x,yֵ��ȡ��Ӧ���뾶
					q=-p*j+i;
					q0=q;

					//�ۼ������ۼ�
					*(houghBuf+p0*houghWidth+ q0+houghWidth/2)+=1;
				}
			}
		}
	}
	//���ۼ�����ļ���ֵ������¼��ʱ����������
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
	//������ֵλ��ת���ɼ�����뾶�ͽǶ�,��ͨ����������
	*skew=(maxP-houghHeight/2)*0.1;
	*jieju=maxQ-houghWidth/2;

}

//����ͼ�����ݣ����ֱ��б��
float skewByHough(unsigned char *imgBuf, int width, int height)
{
	//�ۼ�������ؾ�����
   int houghWidth=2*height+1;

	//�ߵĽǶ���[-45,45]֮��,б�ʷֱ���Ϊ0.1������������ۼ�����߶�Ϊ
	int houghHeight=2001;

	//�����ۼ����黺����
  int *houghBuf=new int[houghWidth*houghHeight];
  float slop; 
  float jieju;
  HoughTransform1(imgBuf, width, height, houghBuf, houghWidth, houghHeight,
		    &slop,&jieju);
  return slop;
}