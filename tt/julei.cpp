#include "julei.h"

#include "math.h"

//���࣬ k ��ֵ��̬����򵥿����㷨
void Kjunzhi_simple(unsigned char * grayImgBuf, int width, int height)
{

	int i,j;
	int t=128;//���ͼ��Ҷ�ֵ��С��t���ߴ���t�����㷨�����ִ���
	int mark1=180, mark2=190;//�������������ʼ�����

	//����ڷ����ֳ�����
	unsigned char *buf=new unsigned char[width*height];
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
				if(abs(*(grayImgBuf+i*width+j)-mark1)<abs(*(grayImgBuf+i*width+j)-mark2))
					*(buf+i*width+j)=mark1;
				else
					*(buf+i*width+j)=mark2;
		}
	}
	//��ʼ����λ�ü�������ͳ��
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
	mark1=m1/s1;//���¼����������
	mark2=m2/s2;
	int  newMark1, newMark2;
	//��̬����
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
		newMark1=m1/s1;//����һ�ε�����ľ�������
		newMark2=m2/s2;
		int t=abs(newMark1-mark1)+abs(newMark2-mark2);
		if(t<5)//����µľ������ľ���ɵľ������ĺܽ���˵�������ĺ��٣��㷨���Խ���
			break;
		else{
			mark1=newMark1;
			mark2=newMark2;
		}
	}
	//��д
	for(i=0;i<height;i++){
		for(j=0;j<width;j++){
			grayImgBuf[i*width+j]=buf[i*width+j];
		}
	}

	delete []buf;

}


//��׼��k ��ֵ��̬����

//�Ҷ�ͼ��
//������Ϊ����±�ֵ����0��ʼ��C-1����
//m������C���������ģ�s������ÿ����������ظ���
void KJunzhiDiedaiGray(unsigned char * imgBuf, unsigned char *lableBuf,
		    int width, int height, float m[], int s[], int C)
{

	int i, j, k;
	float je[10];//���ƽ����,���10�����࣬�˴�
	//����C����������ƽ����
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
	//��̬����
	while(1){
		for(k=0;k<C;k++){//��ǰ�ľ������ĺ����ƽ������oldM��oldJe�б���
			oldM[k]=m[k];
			oldJe[k]=je[k];
		}
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				for(k=0;k<C;k++){
					
					if(*(lableBuf+i*width+j)!=k){//��̽���������е���������أ����¼�����ʽ
						t[k]=pow(*(imgBuf+i*width+j)-m[k],2);
						t[k]=s[k]*t[k]/(s[k]+1);
					}
					else{//���Լ����е��������¼�����ʽ
						t[k]=pow(*(imgBuf+i*width+j)-m[k],2);
						t[k]=s[k]*t[k]/(s[k]-1);
					}
				}
				//�������������
				min=0;
				for(k=1;k<C;k++){
					if(t[k]<t[min])
						min=k;
				}
				//���и��õ������ʱ������������ص�������𣬲��������ı仯
				if(min!=*(lableBuf+i*width+j)){
					mark=*(lableBuf+i*width+j);
					m[mark] +=(m[mark]-*(imgBuf+i*width+j))/(s[mark]-1);
					s[mark]--;
					je[mark]-=t[mark];

					m[min] +=(*(imgBuf+i*width+j)-m[min])/(s[min]+1);
					s[min]++;
					je[min]+=t[min];

					*(lableBuf+i*width+j)=min;//�õ�����ı��
				}
				
			}
		}
		
		//�������ƽ���͵ı仯�����仯��Сʱ����ֹ����
		float det=0;
		for(k=0;k<C;k++)
			det+=abs(oldJe[k]-je[k])+abs(oldJe[k]-je[k]);
		if(det<5)
			break;
	}
}


//�Ҷ�K��ֵ
void KjunzhiGray(unsigned char *grayImgBuf, int width, int height)
{
	int i, j, k, min;
	int daibiao[3]={50,150,250};//���ô����
	int C=3;//����
	unsigned char * buf=new unsigned char[width*height];//��ų�ʼ������
	//����ڻ��֣������ʼ������
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
	//��ʼ����λ�ã���m�������¼��㣬��Ϊ��ʼ��������
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
	for(k=0;k<C;k++)//ÿ��������������
		m[k] /= s[k];

	//����ʼ���������Լ���Ӧ�������������������е�������
	KJunzhiDiedaiGray(grayImgBuf, buf,width, height, m, s, C);

	//��д��ʾ
	for(i=0;i<width*height;i++){
    		grayImgBuf[i]=buf[i]*80;
	}

	//�ͷſռ�
	delete []buf;
}
