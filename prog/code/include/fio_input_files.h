#ifndef DEF_FIO_INPUT_FILES
#define DEF_FIO_INPUT_FILES

class FioInputFiles
{
	
	InputFiles FLoopUntilProperFileInput()
	{
	}

	class fio
	{
	private:
		template <class T>
		static void write_to_file_(const std::string& file_name, const std::vector<std::vector<T>>& table)
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
			write_to_file_(FILE_NAME_RADIUS, radius);
		}

		static void write_to_file(const Tmns& meniscus)
		{
			write_to_file_(FILE_NAME_MNS, meniscus);	
		}
	}
	
	
};

#endif DEF_FIO_INPUT_FILES
