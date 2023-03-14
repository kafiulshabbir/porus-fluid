#ifndef DEF_FIO
#define DEF_FIO

struct InputFiles
{
	Tfloat radius;
	Tmns mnsc;
	Diamension diamension;
};








void TFPrintMatrix(const std::string& s, const Tfloat& matrix)
{
	std::cout << "\n\n------------------------------------\n";
	std::cout << s << '\n';
	
	std::cout << std::setw(7) << -1 << " | ";
	for(int j = 0; j < matrix.front().size(); ++ j)
	{
		std::cout << std::setw(7) << (float)j << ' ';
	}
	std::cout << '\n';
	for(int i = 0; i < matrix.size(); ++ i)
	{
		std::cout << std::setw(7) << (float)i << " | ";
		for(int j = 0; j < matrix.front().size(); ++ j)
		{
			std::cout << std::setw(7) << matrix[i][j] << ' ';
		}
		std::cout << '\n';
	}
}


void TFPrintMatrix(const std::string& s, const std::vector<float>& v, const int n, const int m)
{
	std::cout << "\n\n------------------------------------\n";
	std::cout << s << '\n';
	
	int count = 0;
	for(int i = 0; i <= n; ++ i)
	{
		int mt = m / 2 - i % 2;
		for(int j = 0; j <= mt; ++ j)
		{
			std::cout << std::setw(7) << v[count++] << ' ';
		}
		std::cout << '\n';
	}
}

#endif
