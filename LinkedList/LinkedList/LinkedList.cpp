#include<iostream>
#include<assert.h>
#include "LinkedList.h"

using std::cout;
using std::endl;

void Init(IList* _List)
{
	_List->CurCount = 0;
	_List->HeadNode = nullptr;
	_List->TailNode = nullptr;
}



void push_back(IList* _List, int _iData)
{
	// ������ ��� �����Ҵ��ϰ� �ʱ�ȭ
	INode* NewNode =(INode*)malloc(sizeof(INode));
	NewNode->iData = _iData;
	NewNode->NextNode = nullptr;

	
	if (nullptr == _List->HeadNode)				// ���� List���� ����ִٸ�
	{
		_List->HeadNode = NewNode;				
		_List->TailNode = NewNode;
	}
	else
	{
		//TailNode�� �ڿ� Newnode�� ����
		_List->TailNode->NextNode = NewNode;
		//TailNode ����
		_List->TailNode = NewNode;
	}

	++_List->CurCount; // �����Ͱ� �߰������Ƿ� ���� ������ ���� �ϳ�����
}





void push_front(IList* _List, int _iData)
{
	INode* NewNode = (INode*)malloc(sizeof(INode));
	NewNode->iData = _iData;

	if (nullptr == _List->HeadNode)
	{
		_List->HeadNode = NewNode;
		_List->TailNode = NewNode;
	}
	else
	{
		// HeadNode�� NewNode�� ���� ������ ��� ������ �־��� HeadNode->NextNode�� �������� ������Ƿ�
		// ���� NewNode�� Nextnode�� ������ �ִ� HeadNode->NextNode�� ������ �������ش� 
		NewNode->NextNode = _List->HeadNode;
		_List->HeadNode = NewNode;
	}
	++_List->CurCount;
}



void pop_back(IList* _List)
{
	// ���� List�� ����ִٸ� _List->HeadNode�� nullptr�̹Ƿ� �����߻�
	assert(_List->HeadNode);


	//��������� nextnode�� nullptr���� ������־���ϹǷ� ������带 ����.
	//������ ����� ������� ī����
	int Prev = _List->CurCount - 1;
	
	// Prev�� 0�̸� �����Ͱ� �ϳ���� ��
	if (Prev==0)
	{
		free(_List->HeadNode);
		_List->HeadNode = nullptr;
		_List->TailNode = nullptr;
	}
	else
	{
		INode* TmpNode = _List->HeadNode;
		while (1)
		{
			//TailNode�� ���������� �̵�
			if (TmpNode->NextNode == _List->TailNode)
			{
				break;
			}
			TmpNode = TmpNode->NextNode;
		}
		//���� TailNode�� free�ϰ� TailNode�� ������带 Tailnode�� ����鼭 NextNode�� ���������Ƿ� nullptr�� �־��ش�.
		TmpNode->NextNode = nullptr;
		free(_List->TailNode);
		_List->TailNode = TmpNode;
	}
	--_List->CurCount;
}



void pop_front(IList* _List)
{
	assert(_List->HeadNode); 
	
	int Prev = _List->CurCount - 1;

	if (Prev == 0)
	{
		free(_List->HeadNode);
		_List->HeadNode = nullptr;
		_List->TailNode = nullptr;
	}
	else
	{
		//�Ǿ��� ������ ��带 �޴� Node
		INode* Node = _List->HeadNode;

		//�����ϱ����� �̸� Head�� ������ ����� ���� ��� ����
		_List->HeadNode = Node->NextNode;

		//�����ϱ����� �����ּҸ� nullptr
		Node->NextNode = nullptr;

		//�����ҳ�带 ���� Node ����
		free(Node);
	}
	--_List->CurCount;
}

void release(IList* _List)
{
	INode* Node = _List->HeadNode;
	for (int i = 0; i < _List->CurCount; ++i)
	{
		INode* NextNode = Node->NextNode;
		free(Node);
		Node = NextNode;
	}
}

void show(IList* _List)
{
	INode* Node = _List->HeadNode;
	for (int i = 0; i < _List->CurCount; i++)
	{
		cout << Node->iData << endl;
		Node = Node->NextNode;
	}
}