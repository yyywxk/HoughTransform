#include<stdio.h>
#include "stack.h"

//以下是用数组实现栈
stackArray::stackArray()
{
	top=-1, maxtop=9999;
}

stackArray::~stackArray()
{
	
}

//判断栈空
bool stackArray::IsEmpty()
{
	if(top==-1)
		return 1;
	else
		return 0;
}
//判断栈满
bool stackArray::IsFull()
{
	if(top==maxtop)
		return 1;
	else
		return 0;
}
//弹出
point stackArray::pop()
{
	point p=s[top];
	top--;
	return p;
}
//压入
void stackArray::push(point p)
{
	top++;
	s[top]=p;
}


void stackArray::disply()
{
	if(top==-1)
		printf("没有元素\n");
	for(int i=top;i>=0;i--){
		printf("%4d%4d\n", s[i].x, s[i].y);
	}
}





//以下是用链表实现栈
stackList::stackList()//构造函数，初始化
{
  head=0;
}
stackList::~stackList()//析构函数，释放资源
{
  stacknode *p;
  while(head!=0)
  {
    p=head;
    head=head->link;
    delete p;
  }
}
point stackList::pop()//弹出
{
  stacknode *p=head;
  head=head->link;
  point data=p->data;
  delete p;
  return data;
}
void stackList::push(point data)//压入
{
  stacknode *p=new stacknode;
  p->data.x=data.x;
  p->data.y=data.y;
  p->link=head;
  head=p;
}
bool stackList::empty()//是否空
{
  if(head==0)
    return 1;
  else return 0;
}
void stackList::display()
{
	if(head==0){
		printf("栈空\n");
		return;
	}
  stacknode *p=head;
  while(p!=0)
  {
    printf("%4d%4d", p->data.x,p->data.y);
    p=p->link;
  }
  printf("\n");
}
