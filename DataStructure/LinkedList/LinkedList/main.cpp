#include<iostream>
#include "LinkedList.h"

using std::cout;
using std::endl;

int main()
{
	IList List;
	Init(&List);

	push_back(&List,10);
	push_front(&List,20);
	push_back(&List,30);
	push_front(&List, 40);

	show(&List);

	cout << " " << endl;

	pop_back(&List);
	pop_front(&List);

	show(&List);

	release(&List);
}