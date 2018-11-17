#include "algorithm.h"
#include "math.h"
#include "Windows.h"
#include "stdio.h"
struct mypoint
{
	int x;
	int y;
};
imgAlgorithm::imgAlgorithm()
{
	imgBufOut=0;
}
imgAlgorithm::~imgAlgorithm()
{
	if(imgBufOut!=0)
		delete []imgBufOut;
}
void imgAlgorithm::regionGrowing(int seed_x, int seed_y, int thresh)
{
	if(byteCount!=1)
		return;
	//循环变量
	int i, j,k;
	//申请空间，存放生长结果
	if(imgBufOut!=0){
		delete []imgBufOut;
		imgBufOut=0;
	}
	imgBufOut=new unsigned char[width*height];
	//将输出图像初始化置255,用0代表像素的生长标记
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			*(imgBufOut+i*width+j)=255;
		}
	}

	//二维数组direction代表中心像素点8邻域坐标与该点在x和y方向上的偏移,
	//其中第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//栈申请，此处假定进栈的像素最多为图像总像素数
	struct mypoint *stack=new struct mypoint[width*height];
	int top;//栈顶指针

	//当前正处理的点和弹出的点
	struct mypoint currentPoint, popPoint;
	
	int temp1, temp2;//临时变量,存放种子点以及待生长像素的灰度值

	//记录种子像素的灰度值
	temp1=*(imageBuf+seed_y*width+seed_x);

	//将给定种子点置标记0,入栈
	*(imgBufOut+seed_y*width+seed_x)=0;
	top=0;
	stack[top].x=seed_x;
	stack[top].y=seed_y;

	//堆栈
	while(top>-1){
		//弹出栈顶元素,该元素已经生长过
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//考察弹出像素周围是否有没有生长的像素
		for(k=0;k<8;k++){
			//待考察的邻域点
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//如果待考察的点不在图像内，则跳过
			if(currentPoint.x<0||currentPoint.x>width-1||
				currentPoint.y<0||currentPoint.y>height-1)
				continue;
			
			//该点的像素值
			int gray=*(imgBufOut+currentPoint.y*width+currentPoint.x);

			//弹出的点周围有尚没生长的点
			if(gray==255){
				temp2=*(imageBuf+currentPoint.y*width+currentPoint.x);
				//如果当前被考察的像素灰度值与种子点灰度值之差小于给定的阈值,
				//则认为相似,将其进栈处理
				if(abs(temp1-temp2)<thresh){
					//给该点置生长标记0
					*(imgBufOut+currentPoint.y*width+currentPoint.x)=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}
		
	}
	
	//清除缓冲区
	delete []stack; 
	
}


//写文件，子类中的数据，算法结果存盘
void imgAlgorithm::saveFile(char *filePath)
{
	if(!imgBufOut)
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
      *(buf+i*lineByte+j)=*(imgBufOut+i*width*byteCount+j);
  }
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;//清理空间

  fclose(fp);
}

//大津阈值计算
int imgAlgorithm::threshOtsu(int histArray[256])
{
	
	float u0,u1;//c0和c1组的均值

	float w0,w1;//c0和c1组产生的概率
	
	int count0;//c0组的像素数

	int i,t;//循环变量

	//统计直方图中像素的个数，存放在sum中
	int sum=0;
	for(i=0;i<256;i++)
		sum = sum+histArray[i];

    float devi[255];//存放每个阈值对应的方差，共255个可能的阈值

	//阈值t从0至254变化，计算对应的方差
	for(t=0;t<255;t++){
		//计算阈值为t时，c0组的均值和产生的概率
		u0=0;
		count0=0;//c0组的像素数
		for(i=0; i<=t;i++){
			u0 += i*histArray[i];
			count0 += histArray[i];
		}
		if(count0==0||count0==sum){//C0组没有像素或者所有的像素都分到了C0组
			devi[t]=-1;//
			continue;
		}
		u0=u0/count0;
		w0=(float)count0/sum;//c0组的概率

		//计算阈值为t时，c1组的均值和产生的概率
		u1=0;
		for(i=t+1; i<256;i++)
			u1+=i*histArray[i];
		
		u1=u1/(sum-count0);//C0组像素数与C1组像素数之和为图像总像素数。
		w1=1-w0;

		//两组间的方差
		devi[t]=w0*w1*(u1-u0)*(u1-u0);

	}
	//在255个阈值中，搜索最大方差对应的阈值
	float maxDevi=devi[0];
	int maxT=0;
	for(t=1;t<255;t++){
		if(devi[t]>maxDevi){
			maxDevi=devi[t];
			maxT=t;
		}
	}

	//返回最佳阈值
	return maxT;
}

//大津阈值分割
void imgAlgorithm::binaryByOtsu()
{
	int i, j, gray;

	//直方图统计
	int hist[256];
	for(i=0;i<256;i++)
		hist[i]=0;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			gray=imageBuf[i*width+j];
			hist[gray]++;
		}
	}
	//输入直方图，计算大津阈值
	int thresh=threshOtsu(hist);

	//申请空间，存放生长结果
	if(imgBufOut!=0){
		delete []imgBufOut;
		imgBufOut=0;
	}
	imgBufOut=new unsigned char[width*height];
	//利用大津阈值对图像二值化
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			gray=imageBuf[i*width+j];
			if(gray<=thresh)
				imgBufOut[i*width+j]=0;
			else
				imgBufOut[i*width+j]=255;
		}
	}
}