#include "cmdio/cmdioprint.h"

void cmdio::Print::pmat(const std::string& s, const Tfloat& matrix)
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


void cmdio::Print::pmat(const std::string& s, const std::vector<float>& v, const int n, const int m)
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

void cmdio::Print::pmat(const std::string& s, const TMns& matrix)
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
			std::cout << std::setw(7) << matrix[i][j].printable() << ' ';
		}
		std::cout << '\n';
	}
}

void cmdio::Print::pmnsc(const TMns& matrix)
{
	for(size_t i = 0; i < matrix.size(); ++ i)
	{
		for(size_t j = 0; j < matrix[i].size(); ++ j)
		{
			const auto& a = matrix[i][j];
			std::cout << "(" << i << ", " << j << "): " << a.n << " " << a.type << " {" << a.pos.front() << ", " << a.pos.back() << "}" << std::endl;
		}
		std::cout << std::endl;
	}
}

void cmdio::Print::diamension()
{
	const std::map<std::string, dst::Diamension> lst_diamension = 
		fileio::Read::diamension();
	
	for(const auto& [file_name, d]: lst_diamension)
	{
		std::cout << "-IFR-diamension " << file_name << " (r, c) = (" << d.rows << ", " << d.cols << ")" << std::endl;
	}
}
	
	
void cmdio::Print::padd(const std::string& s, const std::vector<std::vector<std::vector<float>>>& matrix)
{
	std::cout << "\n\n------------------------------------\n";
	std::cout << s << '\n';
	
	std::cout << std::setw(4) << -1 << " | ";
	for(int j = 0; j < matrix.front().size(); ++ j)
	{
		std::cout << std::setw(19) << j << ' ';
	}
	std::cout << '\n';
	for(int i = 0; i < matrix.size(); ++ i)
	{
		std::cout << std::setw(4) << i << " | ";
		for(int j = 0; j < matrix.front().size(); ++ j)
		{
			std::cout << "(" << std::setw(7) << matrix[i][j].front() << ", " << std::setw(7) << matrix[i][j].back() << ") ";
		}
		std::cout << '\n';
	}
}
