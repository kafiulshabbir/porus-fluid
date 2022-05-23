#include "dec.h"
#include "dat_radius.h"
#include "dat_fill.h"

// g++ -Wall -std=c++17 main2.cpp stc_node.cpp dat_tube.cpp dat_fill.cpp dat_radius.cpp -o main2
int main()
{
	const int n = 5;
	dat::cls_fill fill("in-fill-distribution.txt", n, 0.5, 99, 99);
	
	fill.read_file();
	fill.print_data();
	
	dat::cls_radius radius("in-radius-distribution.txt", n, 2, 99, 99);
	radius.read_file();
	radius.print_data();
	return 0;
}
