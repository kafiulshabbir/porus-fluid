#ifndef DEF_FILEWRITE
#define DEF_FILEWRITE

namespace fio
{
	class Write
	{
		template <class T>
		static void write(const std::string& file_name, const std::vector<std::vector<T>>& table)
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
		
	public:
		static void write_to_file(const Tfloat& radius)
		{
			write(FILE_NAME_RADIUS, radius);
		}

		static void write_to_file(const TMns& mnsc)
		{
			write(FILE_NAME_MNS, mnsc);	
		}
	};
}

#endif
