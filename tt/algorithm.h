#include"image.h"
#include  "stack.h"

class imgAlgorithm: public imageClass
{
private:
	unsigned char *imgBufOut;
	//�����ֵ����
	int threshOtsu(int histogramArray[256]);
public:
	imgAlgorithm();
	~imgAlgorithm();
	//ָ�����ӵ�λ�ã�������������ֵ����ͼ���������
	void regionGrowing(int seed_x, int seed_y, int thresh);
	//�����ֵ�ָ�
	void binaryByOtsu();
	void saveFile(char* filePath);//�������д���Ľ��
};