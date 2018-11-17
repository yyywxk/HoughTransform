#include<stdio.h>
#include "stack.h"

//������������ʵ��ջ
stackArray::stackArray()
{
	top=-1, maxtop=9999;
}

stackArray::~stackArray()
{
	
}

//�ж�ջ��
bool stackArray::IsEmpty()
{
	if(top==-1)
		return 1;
	else
		return 0;
}
//�ж�ջ��
bool stackArray::IsFull()
{
	if(top==maxtop)
		return 1;
	else
		return 0;
}
//����
point stackArray::pop()
{
	point p=s[top];
	top--;
	return p;
}
//ѹ��
void stackArray::push(point p)
{
	top++;
	s[top]=p;
}


void stackArray::disply()
{
	if(top==-1)
		printf("û��Ԫ��\n");
	for(int i=top;i>=0;i--){
		printf("%4d%4d\n", s[i].x, s[i].y);
	}
}





//������������ʵ��ջ
stackList::stackList()//���캯������ʼ��
{
  head=0;
}
stackList::~stackList()//�����������ͷ���Դ
{
  stacknode *p;
  while(head!=0)
  {
    p=head;
    head=head->link;
    delete p;
  }
}
point stackList::pop()//����
{
  stacknode *p=head;
  head=head->link;
  point data=p->data;
  delete p;
  return data;
}
void stackList::push(point data)//ѹ��
{
  stacknode *p=new stacknode;
  p->data.x=data.x;
  p->data.y=data.y;
  p->link=head;
  head=p;
}
bool stackList::empty()//�Ƿ��
{
  if(head==0)
    return 1;
  else return 0;
}
void stackList::display()
{
	if(head==0){
		printf("ջ��\n");
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
