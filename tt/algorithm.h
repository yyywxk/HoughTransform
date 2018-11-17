#include"image.h"
#include  "stack.h"

class imgAlgorithm: public imageClass
{
private:
	unsigned char *imgBufOut;
	//大津阈值计算
	int threshOtsu(int histogramArray[256]);
public:
	imgAlgorithm();
	~imgAlgorithm();
	//指定种子点位置，并给定生长阈值，对图像进行生长
	void regionGrowing(int seed_x, int seed_y, int thresh);
	//大津阈值分割
	void binaryByOtsu();
	void saveFile(char* filePath);//存子类中处理的结果
};