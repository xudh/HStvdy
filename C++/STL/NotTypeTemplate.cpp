/*
	非类型模板
	一般来说，非类型模板参数可以是常整数（包括枚举）或者指向外部链接对象的指针。
	那么就是说，浮点数是不行的，指向内部链接对象的指针是不行的。
*/

#include <iostream>

using std::cout;
using std::endl;

template<typename T, int MAXSIZE> class Stack
{
private:
       T elems[MAXSIZE];

};

int main()
{
	Stack<int, 20> int20Stack;
	Stack<int, 40> int40Stack;

	return 0;
}

