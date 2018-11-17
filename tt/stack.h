class point
{
public:
	int x, y;
};
class stackArray
{
private:
	point s[1000];
	int top;
	int maxtop;
public:
	stackArray();
	~stackArray();
	point pop();
	void push(point p);
	bool IsEmpty();
	bool IsFull();
	void disply();
};

//����ʵ��ջ
class stacknode
{
public:
	point data;
	stacknode* link;
};
class stackList
{
private:
  stacknode *head;
  
public:
  stackList();
  ~stackList();
  bool empty();
  point pop();
  void push(point data);
  void display();
};