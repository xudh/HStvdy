// 类模板
#include <iostream>

using std::cout;
using std::endl;

#ifndef __ClassTemplate_H__
#define __ClassTemplate_H__

template<typename T1, typename T2> class myClass
{
public:
	myClass(T1 a, T2 b);
	void show();

private:
	T1 I;
	T2 J;

};

template <typename T1, typename T2> myClass<T1, T2>::myClass(T1 a, T2 b)
: I(a), J(b)
{
}

template <typename T1, typename T2> void myClass<T1, T2>::show()
{
	cout<<"I="<<I<<", J="<<J<<endl;
}

#endif

int main()
{
	myClass<int, int> class1(3, 5);
	class1.show();
	myClass<int, char> class2(3, 'a');
	class2.show();
	myClass<double, int> class3(2.9, 10);
	class3.show();

	return 0;
}

