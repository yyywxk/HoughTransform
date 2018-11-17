#include "julei.h"

#include "math.h"

//两类， k 均值动态聚类简单快速算法
void Kjunzhi_simple(unsigned char * grayImgBuf, int width, int height)
{

	int i,j;
	int t=128;//如果图像灰度值都小于t或者大于t，该算法将出现错误
	int mark1=180, mark2=190;//随便设置两个初始聚类点

	//最近邻方法分成两类
	unsigned char *buf=new unsigned char[width*height];
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
				if(abs(*(grayImgBuf+i*width+j)-mark1)<abs(*(grayImgBuf+i*width+j)-mark2))
					*(buf+i*width+j)=mark1;
				else
					*(buf+i*width+j)=mark2;
		}
	}
	//初始重心位置及像素数统计
	int s1=0,m1=0, s2=0, m2=0;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			if(*(buf+i*width+j)==mark1){
				m1+=*(grayImgBuf+i*width+j);
				s1++;
			}
			else if(*(buf+i*width+j)==mark2){
				m2+=*(grayImgBuf+i*width+j);
				s2++;
			}
		}
	}
	mark1=m1/s1;//重新计算聚类中心
	mark2=m2/s2;
	int  newMark1, newMark2;
	//动态聚类
	while(1){
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				if(abs(*(grayImgBuf+i*width+j)-mark1)<abs(*(grayImgBuf+i*width+j)-mark2))
					*(buf+i*width+j)=mark1;
				else
					*(buf+i*width+j)=mark2;
			}
		}
		s1=0,m1=0, s2=0, m2=0;
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				if(*(buf+i*width+j)==mark1){
					m1+=*(grayImgBuf+i*width+j);
					s1++;
				}
				else if(*(buf+i*width+j)==mark2){
					m2+=*(grayImgBuf+i*width+j);
					s2++;
				}
			}
		}
		newMark1=m1/s1;//计算一次迭代后的聚类中心
		newMark2=m2/s2;
		int t=abs(newMark1-mark1)+abs(newMark2-mark2);
		if(t<5)//如果新的聚类中心距离旧的聚类中心很近，说明调整的很少，算法可以结束
			break;
		else{
			mark1=newMark1;
			mark2=newMark2;
		}
	}
	//回写
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			grayImgBuf[i*width+j]=buf[i*width+j];
		}
	}

	delete []buf;

}


//标准的k 均值动态聚类

//灰度图像
//分类标号为类的下标值，从0开始，C-1结束
//m数组存放C个聚类中心，s数组存放每个聚类的像素个数
void KJunzhiDiedaiGray(unsigned char * imgBuf, unsigned char *lableBuf,
		    int width, int height, float m[], int s[], int C)
{

	int i, j, k;
	float je[10];//误差平方和,最多10个聚类，此处
	//计算C个聚类的误差平方和
	for(k=0;k<C;k++)
		je[k]=0;
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			for(k=0;k<C;k++){
				if(*(lableBuf+i*width+j)==k){
					je[k] += pow(*(imgBuf+i*width+j)-m[k],2);
				}
			}
		}
	}

	int min, mark;
	float oldM[10], oldJe[10],t[10] ;
	//动态聚类
	while(1){
		for(k=0;k<C;k++){//当前的聚类中心和误差平方存入oldM和oldJe中备份
			oldM[k]=m[k];
			oldJe[k]=je[k];
		}
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				for(k=0;k<C;k++){
					
					if(*(lableBuf+i*width+j)!=k){//试探往其他类中调入这个像素，重新计算误差公式
						t[k]=pow(*(imgBuf+i*width+j)-m[k],2);
						t[k]=s[k]*t[k]/(s[k]+1);
					}
					else{//从自己类中调出，重新计算误差公式
						t[k]=pow(*(imgBuf+i*width+j)-m[k],2);
						t[k]=s[k]*t[k]/(s[k]-1);
					}
				}
				//计算调入的最佳类
				min=0;
				for(k=1;k<C;k++){
					if(t[k]<t[min])
						min=k;
				}
				//当有更好的类存在时，调整这个像素的所属类别，并计算误差的变化
				if(min!=*(lableBuf+i*width+j)){
					mark=*(lableBuf+i*width+j);
					m[mark] +=(m[mark]-*(imgBuf+i*width+j))/(s[mark]-1);
					s[mark]--;
					je[mark]-=t[mark];

					m[min] +=(*(imgBuf+i*width+j)-m[min])/(s[min]+1);
					s[min]++;
					je[min]+=t[min];

					*(lableBuf+i*width+j)=min;//置调整后的标记
				}
				
			}
		}
		
		//计算误差平方和的变化，当变化很小时，终止迭代
		float det=0;
		for(k=0;k<C;k++)
			det+=abs(oldJe[k]-je[k])+abs(oldJe[k]-je[k]);
		if(det<5)
			break;
	}
}


//灰度K均值
void KjunzhiGray(unsigned char *grayImgBuf, int width, int height)
{
	int i, j, k, min;
	int daibiao[3]={50,150,250};//设置代表点
	int C=3;//两类
	unsigned char * buf=new unsigned char[width*height];//存放初始聚类标号
	//最近邻划分，计算初始聚类标号
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			min=0;
			for(k=1; k<C; k++){
				if(abs(*(grayImgBuf+i*width+j)-daibiao[k])<abs(*(grayImgBuf+i*width+j)-daibiao[min]))
					min=k;
			}
			buf[i*width+j]=min;
		}
	}
	//初始重心位置，将m数组重新计算，作为初始聚类中心
	int s[10];
	float m[10];
	for(k=0;k<C;k++){
		m[k]=0;
		s[k]=0;
	}
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			for(k=0;k<C;k++){
				if(*(buf+i*width+j)==k){
					m[k]+= *(grayImgBuf+i*width+j);
					s[k]++;
				}
			}
		}
	}
	for(k=0;k<C;k++)//每类样本聚类中心
		m[k] /= s[k];

	//将初始聚类中心以及对应像素数传进函数，进行迭代聚类
	KJunzhiDiedaiGray(grayImgBuf, buf,width, height, m, s, C);

	//回写显示
	for(i=0;i<width*height;i++){
    		grayImgBuf[i]=buf[i]*80;
	}

	//释放空间
	delete []buf;
}
