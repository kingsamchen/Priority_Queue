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
    
    int ary[] = {10,7,6,5,2};
    PriorityQueue<int> pq(ary, ary + _countof(ary));
    pq.Delete(pq.find(7));
    
    _getch();
	return 0;
}

