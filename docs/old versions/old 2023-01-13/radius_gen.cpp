#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const std::string OUT_FILE = "radius.txt";

bool Command(char& command)
{
	std::cout << "c)hange_radius e)xit: ";
	std::cin >> command;
	if(command == 'e')
	{
		return false;
	}
	
	return true;
}

int main()
{
	int n, m;
	std::cout << "Input the number of [rows] and [columns]: ";
	std::cin >> n >> m;
	
	float default_fill;
	std::cout << "Input default fill: ";
	std::cin >> default_fill;
	
	std::vector<std::vector<float>> v(n, std::vector<float>(m, default_fill));
	
	
	char command;
	while(Command(command))
	{
		std::cout << "Enter range to change [row_from], [row_to], [col_from], [col_to], [new_val]: ";
		int row_from, row_to, col_from, col_to;
		float new_val;
		
		std::cin >> row_from >> row_to >> col_from >> col_to >> new_val;
		
		int r1 = std::min(row_from, row_to);
		int r2 = std::max(row_from, row_to);
		int c1 = std::min(col_from, col_to);
		int c2 = std::max(col_from, col_to);
		
		for(int i = r1; i < r2; ++ i)
		{
			for(int j = c1; j < c2; ++ j)
			{
				v[i][j] = new_val;
			}
		}
	}
	
	std::ofstream fout(OUT_FILE);
	
	fout << n << ' ' << m << '\n';
	
	for(const auto& row: v)
	{
		for(const auto& x: row)
		{
			fout << x << ' ';
		}
		
		fout << '\n';
	}
	
	return 0;
}

