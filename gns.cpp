#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "dst.h"

const std::string PROGRAM_PROPERTY_NAME = "gns - GeNerateinitialSettings";
const std::string PROGRAM_PROPERTY_VERSION = "26.01.2023";

const int DEF_VAL_ROWS = 6;
const int DEF_VAL_COLS = 4;

template <class T>
T FCinRead(const std::string& message)
{
	std::cout << "-CIN-" << message << ": ";
	
	T input;
	std::cin >> input;
	
	return input;
}

bool FReadCommand(char& command)
{
	command = FCinRead<char>("d)efault, r)adius, R)adius_change, f)ill, c)heck, e)xit");
	
	return command != 'e';
}

void FUpdateDiamensions(int& nrows, int& ncols)
{
	if(nrows == -1 && ncols == -1)
	{
		nrows = FCinRead<int>("norws");
		ncols = FCinRead<int>("ncols");
	}
}

dst::Fill FGenFillInitial(int nrows, int ncols)
{
	dst::Fill fill(nrows, ncols);
	for(auto& val: fill.v.back())
	{
		val = dst::FillProperty(1, 0, 0.1, 0);
	}
	
	return fill;
}


int main()
{
	std::cout << PROGRAM_PROPERTY_NAME << ", " << PROGRAM_PROPERTY_VERSION << "\n\n";
	
	char command;
	int nrows = -1, ncols = -1;
	while(FReadCommand(command))
	{
		if(command == 'd')
		{
			nrows = DEF_VAL_ROWS;
			ncols = DEF_VAL_COLS;
			
			dst::Radius(nrows, ncols, 1).write(dst::FILE_NAME_RADIUS);
			FGenFillInitial(nrows, ncols).write(dst::FILE_NAME_FILL);
		}
		else if(command == 'r')
		{
			FUpdateDiamensions(nrows, ncols);
			const auto def_val_radius = FCinRead<real>("Radius default value");
			dst::Radius(nrows, ncols, def_val_radius).write(dst::FILE_NAME_RADIUS);
		}
		else if(command == 'R')
		{
			std::cout << "Enter x1, x2, y1, y2, val: ";
			real x1, x2, y1, y2, val;
			std::cin >> x1 >> x2 >> y1 >> y2 >> val;
			dst::Radius radius;
			radius.read(dst::FILE_NAME_RADIUS);
			radius.update(x1, x2, y1, y2, val);
			radius.write(dst::FILE_NAME_RADIUS);
			radius.print();
		}
		else if(command == 'f')
		{
			FUpdateDiamensions(nrows, ncols);
			//const auto def_val_fill = FCinRead<FillProperty>("Fill value [n_meniscus, type{0,1}, pos1 of men 0.0 - 1.0, pos2");
			FGenFillInitial(nrows, ncols).write(dst::FILE_NAME_FILL);
		}
		dst::FPrintValidityStatus();	
		std::cout << "\n\n";	
	}	
	
	if(dst::FCheckValidity())
	{
		std::cout << "-FDK-" << PROGRAM_PROPERTY_NAME << " is closed properly!" << "\n\n";
	}
	else
	{
		std::cout << "-ERR-" << PROGRAM_PROPERTY_NAME << " closed with corrupted files!" << "\n\n";
	}
	
	return 0;
}
