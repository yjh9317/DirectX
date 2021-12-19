#pragma once

struct INode 
{
	int iData;				// int 데이터를 받는 변수
	INode* NextNode;		// 다음 노드를 연결해줄 변수
};

struct IList
{
	INode* HeadNode;		// List의 첫노드를 가르키는 변수
	INode* TailNode;		// List의 마지막노드를 가르키는 변수
	int CurCount;			// 현재 데이터 갯수
};

void Init(IList* _List);
void push_back(IList* _List,int _iData); // 가장 뒤에 노드를 추가하는 함수
void push_front(IList* _List, int _iData); // 가장 앞에 노드를 추가하는 함수

void pop_back(IList* _List); //가장 뒤의 노드를 삭제하는 함수
void pop_front(IList* _List); //가장 앞의 노드를 삭제하는 함수

void release(IList* _List); //메모리 해제
void show(IList* _List);
