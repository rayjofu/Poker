#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include "Poker.h"
#include <iostream>

using namespace std;

int main() {
	Poker *p = new Poker();
	delete p;

	_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}