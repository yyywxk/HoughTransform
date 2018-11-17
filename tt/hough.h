//输入图像数据，获得直线斜率
float skewByHough(unsigned char *imgBuf, int width, int height);
//该函数实现了极坐标系线检测，分辨率由float radiusResolution, float angleResolution决定
void HoughTransform(unsigned char *imgBinaryIn, int width, 
			int height, int *houghBuf, int  houghWidth, int houghHeight,
		    float radiusResolution, float angleResolution, 
		    float *radius, float *angle);