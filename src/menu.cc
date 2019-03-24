#include <stdio.h>
#include "array.hpp"

void test_array()
{
	using sdizo::Array;

	Array array;
	array.find(2);
	array.insert(1, 0);
	array.find(2);
	array.insert(2, 1);
	array.find(2);
	array.insert(3, 2);
	array.insert(4, 1);
	array.find(2);
	array.remove(3);
	array.find(2);
	array.remove(0);
	array.find(2);
	array.remove(0);
	array.find(2);
	array.remove(0);
	array.find(2);

	array.generate(-20, 74, 20);
	array.add(1337, 0);
}

int main()
{
	test_array();
	return 0;
}
