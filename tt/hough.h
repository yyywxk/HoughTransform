//����ͼ�����ݣ����ֱ��б��
float skewByHough(unsigned char *imgBuf, int width, int height);
//�ú���ʵ���˼�����ϵ�߼�⣬�ֱ�����float radiusResolution, float angleResolution����
void HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    float *radius, float *angle);