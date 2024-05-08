#include "fileiowrite.h"

void fileio::Write::run(const Tdouble& radius)
{
	write_base(declfilename::FILE_RADIUS, radius);
}

void fileio::Write::run(const TMns& mnsc)
{
	write_base(declfilename::FILE_MNSC, mnsc);
}

void fileio::Write::fluid_ppr(
	const std::vector<std::string>& header,
	const std::vector<std::vector<double>>& table
)
{
	std::ofstream fout(declfilename::FILE_FLUID_PPR);

	for(const std::string& head: header)
	{
		fout << head << '\t';
	}
	fout << '\n';

	for(const auto& row: table)
	{
		for(const auto x: row)
		{
			fout << x << '\t';
		}
		fout << '\n';
	}
}
