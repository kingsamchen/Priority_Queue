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
    PriorityQueue<int> pq(2);
    pq.insert(32);
    pq.insert(8);
    pq.insert(16);
    pq.Delete(pq.find(32));
    while (!pq.empty())
    {
        cout<<pq.top()<<endl;
        pq.ExtractTop();
    }
    _getch();
	return 0;
}

