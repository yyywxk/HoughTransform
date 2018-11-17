class imageClass
{
protected:
	int width, height, byteCount;
	unsigned char *imageBuf;
public:
	imageClass();//构造函数
	~imageClass();//析构函数
	void readFile(char* filePath);//读入一个bmp文件
	void saveFile(char* filePath);//存文件
	int getWidth();//获取图像宽
	int getHeight();//图像高
	int getByteCount();//图像灰度还是彩色
	unsigned char *getImgData();//获取图像数据指针

};
