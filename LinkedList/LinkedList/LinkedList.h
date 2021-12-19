#pragma once

struct INode 
{
	int iData;				// int �����͸� �޴� ����
	INode* NextNode;		// ���� ��带 �������� ����
};

struct IList
{
	INode* HeadNode;		// List�� ù��带 ����Ű�� ����
	INode* TailNode;		// List�� ��������带 ����Ű�� ����
	int CurCount;			// ���� ������ ����
};

void Init(IList* _List);
void push_back(IList* _List,int _iData); // ���� �ڿ� ��带 �߰��ϴ� �Լ�
void push_front(IList* _List, int _iData); // ���� �տ� ��带 �߰��ϴ� �Լ�

void pop_back(IList* _List); //���� ���� ��带 �����ϴ� �Լ�
void pop_front(IList* _List); //���� ���� ��带 �����ϴ� �Լ�

void release(IList* _List); //�޸� ����
void show(IList* _List);
