
//k 均值动态聚类，效果比简单算法稍好，不明显，两类灰度
//灰度K均值,标准函数，类数可选,书中正常步骤，速度稍慢，
void KjunzhiGray(unsigned char *grayImgBuf, int width, int height);

//两类， k 均值动态聚类简单快速算法
void Kjunzhi_simple(unsigned char * grayImgBuf, int width, int height);