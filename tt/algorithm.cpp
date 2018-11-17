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
	//ѭ������
	int i, j,k;
	//����ռ䣬����������
	if(imgBufOut!=0){
		delete []imgBufOut;
		imgBufOut=0;
	}
	imgBufOut=new unsigned char[width*height];
	//�����ͼ���ʼ����255,��0�������ص��������
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			*(imgBufOut+i*width+j)=255;
		}
	}

	//��ά����direction�����������ص�8����������õ���x��y�����ϵ�ƫ��,
	//���е�һ��Ϊx�����ƫ��,�ڶ���Ϊy�����ƫ��
	int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

	//ջ���룬�˴��ٶ���ջ���������Ϊͼ����������
	struct mypoint *stack=new struct mypoint[width*height];
	int top;//ջ��ָ��

	//��ǰ������ĵ�͵����ĵ�
	struct mypoint currentPoint, popPoint;
	
	int temp1, temp2;//��ʱ����,������ӵ��Լ����������صĻҶ�ֵ

	//��¼�������صĻҶ�ֵ
	temp1=*(imageBuf+seed_y*width+seed_x);

	//���������ӵ��ñ��0,��ջ
	*(imgBufOut+seed_y*width+seed_x)=0;
	top=0;
	stack[top].x=seed_x;
	stack[top].y=seed_y;

	//��ջ
	while(top>-1){
		//����ջ��Ԫ��,��Ԫ���Ѿ�������
		popPoint.x=stack[top].x;
		popPoint.y=stack[top].y;
		top--;

		//���쵯��������Χ�Ƿ���û������������
		for(k=0;k<8;k++){
			//������������
			currentPoint.x=popPoint.x+direction[k][0];
			currentPoint.y=popPoint.y+direction[k][1];

			//���������ĵ㲻��ͼ���ڣ�������
			if(currentPoint.x<0||currentPoint.x>width-1||
				currentPoint.y<0||currentPoint.y>height-1)
				continue;
			
			//�õ������ֵ
			int gray=*(imgBufOut+currentPoint.y*width+currentPoint.x);

			//�����ĵ���Χ����û�����ĵ�
			if(gray==255){
				temp2=*(imageBuf+currentPoint.y*width+currentPoint.x);
				//�����ǰ����������ػҶ�ֵ�����ӵ�Ҷ�ֵ֮��С�ڸ�������ֵ,
				//����Ϊ����,�����ջ����
				if(abs(temp1-temp2)<thresh){
					//���õ����������0
					*(imgBufOut+currentPoint.y*width+currentPoint.x)=0;
					top++;
					stack[top].x=currentPoint.x;
					stack[top].y=currentPoint.y;
				}
			}
		}
		
	}
	
	//���������
	delete []stack; 
	
}


//д�ļ��������е����ݣ��㷨�������
void imgAlgorithm::saveFile(char *filePath)
{
	if(!imgBufOut)
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
      *(buf+i*lineByte+j)=*(imgBufOut+i*width*byteCount+j);
  }
  fwrite(buf, height*lineByte, 1, fp);
    
  delete []buf;//����ռ�

  fclose(fp);
}

//�����ֵ����
int imgAlgorithm::threshOtsu(int histArray[256])
{
	
	float u0,u1;//c0��c1��ľ�ֵ

	float w0,w1;//c0��c1������ĸ���
	
	int count0;//c0���������

	int i,t;//ѭ������

	//ͳ��ֱ��ͼ�����صĸ����������sum��
	int sum=0;
	for(i=0;i<256;i++)
		sum = sum+histArray[i];

    float devi[255];//���ÿ����ֵ��Ӧ�ķ����255�����ܵ���ֵ

	//��ֵt��0��254�仯�������Ӧ�ķ���
	for(t=0;t<255;t++){
		//������ֵΪtʱ��c0��ľ�ֵ�Ͳ����ĸ���
		u0=0;
		count0=0;//c0���������
		for(i=0; i<=t;i++){
			u0 += i*histArray[i];
			count0 += histArray[i];
		}
		if(count0==0||count0==sum){//C0��û�����ػ������е����ض��ֵ���C0��
			devi[t]=-1;//
			continue;
		}
		u0=u0/count0;
		w0=(float)count0/sum;//c0��ĸ���

		//������ֵΪtʱ��c1��ľ�ֵ�Ͳ����ĸ���
		u1=0;
		for(i=t+1; i<256;i++)
			u1+=i*histArray[i];
		
		u1=u1/(sum-count0);//C0����������C1��������֮��Ϊͼ������������
		w1=1-w0;

		//�����ķ���
		devi[t]=w0*w1*(u1-u0)*(u1-u0);

	}
	//��255����ֵ�У�������󷽲��Ӧ����ֵ
	float maxDevi=devi[0];
	int maxT=0;
	for(t=1;t<255;t++){
		if(devi[t]>maxDevi){
			maxDevi=devi[t];
			maxT=t;
		}
	}

	//���������ֵ
	return maxT;
}

//�����ֵ�ָ�
void imgAlgorithm::binaryByOtsu()
{
	int i, j, gray;

	//ֱ��ͼͳ��
	int hist[256];
	for(i=0;i<256;i++)
		hist[i]=0;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			gray=imageBuf[i*width+j];
			hist[gray]++;
		}
	}
	//����ֱ��ͼ����������ֵ
	int thresh=threshOtsu(hist);

	//����ռ䣬����������
	if(imgBufOut!=0){
		delete []imgBufOut;
		imgBufOut=0;
	}
	imgBufOut=new unsigned char[width*height];
	//���ô����ֵ��ͼ���ֵ��
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