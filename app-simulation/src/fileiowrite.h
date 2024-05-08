#ifndef FILEIOWRITE_H
#define FILEIOWRITE_H

#include "decltypedef.h"
#include "declfilename.h"

#include <string>
#include <vector>
#include <fstream>

namespace fileio
{
	class Write
	{
		template <class T>
		static void write_base(const std::string& file_name,
			const std::vector<std::vector<T>>& table);

	public:
		static void run(const Tdouble& radius);
		static void run(const TMns& mnsc);
		static void fluid_ppr(
			const std::vector<std::string>& header,
			const std::vector<std::vector<double>>& table
		);
	};
}

template <class T>
void fileio::Write::write_base(const std::string& file_name,
	const std::vector<std::vector<T>>& table)
{
	std::ofstream fout(file_name);
	fout << table.size() << ' ' << table.front().size() << '\n';

	for(const auto& row: table)
	{
		for(const auto& cell: row)
		{
			fout << cell << ' ';
		}
		fout << '\n';
	}
}
#endif
