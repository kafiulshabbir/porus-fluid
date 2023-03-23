#include "fio_write_files.h"

template <class T>
static void fio_write_files::write(const std::string& file_name, const std::vector<std::vector<T>>& table)
{
	std::ofstream& fout(file_name);
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

static void fio_write_files::write_to_file(const Tfloat& radius)
{
	write(FILE_NAME_RADIUS, radius);
}

static void fio_write_files::write_to_file(const TMns& mnsc)
{
	write(FILE_NAME_MNS, mnsc);	
}
