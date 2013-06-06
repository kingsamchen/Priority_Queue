/************************************
** Edition:	v1.0.0 Demo
** Author:	Kingsley Chen	
** Date:	2013/06/05
** Purpose:	Kingsley Chen
************************************/

#include "stdafx.h"
#include <iostream>
#include "priorityqueue.h"
#include <conio.h>

using std::cout;
using std::endl;

int _tmain(int, _TCHAR*)
{
    using namespace KCSTL;
    
    int ary[] = {23,12,3,89,32,64,127};
    PriorityQueue<int> pq(ary, ary + _countof(ary));
    
    PriorityQueue<int> pq2(20);
    pq2 = pq;

    while (!pq2.empty())
    {
        cout<<pq2.top()<<endl;
        pq2.ExtractTop();
    }
    _getch();
	return 0;
}

