#ifndef INITCONMODE_H
#define INITCONMODE_H

#include "cmdio/cmdioread.h"
#include "cmdio/cmdioprint.h"
#include "dst/dstdiamension.h"
#include "fileio/fileiowrite.h"
#include "dst/dstmns.h"
#include "fileio/fileioread.h"

#include "utility/utilityrandom.h"

namespace initcon
{
class Mode
	{
		class Generate
		{
		public:
			static void radius(const dst::Diamension& diamension);
			static void mnsc(const dst::Diamension& diamension);
		};
		
		class Modify
		{

			static bool exit_code_modify_dst_save(
				char& save_yes_or_no);

			static bool exit_code_modify_dst(
				const std::string& text_promt_modify_type_dst,
				char& command_char, bool& saved_dst);

			template <class T>
			static std::vector<std::vector<T>>
				rectangular_modification_on_dst(
				std::vector<std::vector<T>> table);
			
			template <class T>
			static std::pair<std::vector<std::vector<T>>, bool>
				modify_dst_from_cin(std::vector<std::vector<T>> table);
			
			static Tdouble add_random_temporary(Tdouble radius);
		public:
			static void radius();
			static void mnsc();
			static void add_random_to_radius();
		};

	public:
		class Main
		{
		public:
			static void generate();
			static void modify();
			static bool exit_code_main_menu(const std::string& text_promt, char& command_char);
			static void imhibition_generator();
		};
	};
}


template <class T>
std::pair<std::vector<std::vector<T>>, bool>
	initcon::Mode::Modify::modify_dst_from_cin(
	std::vector<std::vector<T>> table)
{
	bool saved_dst = true;
	
	const std::string text_promt_modify_type_dst = "r)ectangular_modification, s)ave";
	char command_char;
	
	while(!exit_code_modify_dst(text_promt_modify_type_dst, command_char, saved_dst))
	{
		if(command_char == 'r')
		{
			table = rectangular_modification_on_dst(table);
			saved_dst = false;			
		}
		else if(command_char == 's')
		{
			saved_dst = true;
		}
		
		std::string state_of_dst_modified = "current state: [";
		if(!saved_dst)
		{
			state_of_dst_modified += "not ";
		}
		state_of_dst_modified += "saved]";
		
		cmdio::Print::pmat(state_of_dst_modified, table);
	}
	
	return {table, saved_dst};
}


template <class T>
std::vector<std::vector<T>>
	initcon::Mode::Modify::rectangular_modification_on_dst(
	std::vector<std::vector<T>> table)
{
	std::cout << "-CIN-[(row1, col1), (row2, col2)]: " << std::endl;
	int row1, col1, row2, col2;
		
	std::cin >> row1 >> col1 >> row2 >> col2;
		
	T change_to;
		
	std::cout << "-CIN-chage to ";
	std::cin >> change_to;
	
	const int row_min = std::min(row1, row2);
	const int row_max = std::max(row1, row2);
	const int col_min = std::min(col1, col2);
	const int col_max = std::max(col1, col2);
	
	for(int row = row_min; row <= row_max; ++ row)
	{
		for(int col = col_min; col <= col_max; ++ col)
		{
			table[row][col] = change_to;
		}
	}
	
	return table;
}
#endif
