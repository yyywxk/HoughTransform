class imageClass
{
protected:
	int width, height, byteCount;
	unsigned char *imageBuf;
public:
	imageClass();//���캯��
	~imageClass();//��������
	void readFile(char* filePath);//����һ��bmp�ļ�
	void saveFile(char* filePath);//���ļ�
	int getWidth();//��ȡͼ���
	int getHeight();//ͼ���
	int getByteCount();//ͼ��ҶȻ��ǲ�ɫ
	unsigned char *getImgData();//��ȡͼ������ָ��

};
