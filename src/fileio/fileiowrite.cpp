#include "fileio/fileiowrite.h"

void fileio::Write::run(const Tfloat& radius)
{
	write_base(declfilename::FILE_RADIUS, radius);
}

void fileio::Write::run(const TMns& mnsc)
{
	write_base(declfilename::FILE_MNSC, mnsc);	
}
