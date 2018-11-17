#include "zuixiaoercheng.h"

//简单的最小二乘，返回斜率
float slopeDrawOutLineZuiXiaoErCheng(unsigned char* imgBuf, int width, int height, int threshold)
{

  int *bufX=new int[width];
  int *bufY=new int[width];

  int i, j, n=0;
  int stepX=20;
  if(stepX<1) stepX=1;
  for(j=0;j<width;j+=stepX){
    for(i=0;i<height;i++){
      if(*(imgBuf+i*width+j)<threshold){
        bufX[n]=j;
        bufY[n]=i;
        n++;
        break;
      }
    }
  }
  int c1=0, c2=0, c3=0, c4=0;
  for(j=0;j<n;j++)
    c1 += bufX[j]*bufY[j];
  c1 *= n;

  int t1=0, t2=0;
  for(j=0; j<n;j++){
    t1 += bufX[j];
    t2 += bufY[j];
  }
  c2=t1*t2;
  c4= t1*t1;

  for(j=0;j<n;j++)
    c3 += bufX[j]*bufX[j];
  c3 *=n;
  float slope=(float)(c1-c2)/(c3-c4);

  delete []bufX;
  delete []bufY;
  return slope;
}