#include <fstream>
#include <vector>
#include <string>
#include <iostream>

std::ofstream fout("in_radius_distribution.txt");

const std::vector<std::string> v{
	"NxN-grid-size",
	"multiplier",
	"leak-radius",
};

void Fill(int n, int m, double fill)
{
	fout << '\n';
	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < m; ++ j)
		{
			fout << fill << ' ';
		}
		fout << '\n';
	}
}

int main()
{
	int n;
	
	for(const auto& x: v)
	{
		std::cout << "Enter, " << x << ": ";
		double y;
		std::cin >> y;
		
		if(x == v.front())
		{
			n = y;
		}
		
		fout << x << ' ' << y << '\n';
	}
	
	std::cout << "Enter radius to fill with: ";
	double radius_fill;
	std::cin >> radius_fill;
	
	Fill(n - 1, n, radius_fill);
	Fill(n, n - 1, radius_fill);
		
	return 0;
}


