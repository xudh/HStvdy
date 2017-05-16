// 函数模板
#include <iostream>

using std::cout;
using std::endl;

template<typename T> T min(T x, T y)	// 这里typename也可用class
{
	return (x < y) ? x : y;
}

int main()
{
	int n1 = 2, n2 = 10;
	double d1 = 1.5, d2 = 5.6;
	cout<<"较小整数："<<min(n1, n2)<<endl;
	cout<<"较小实数："<<min(d1, d2)<<endl;
	return 0;
}

