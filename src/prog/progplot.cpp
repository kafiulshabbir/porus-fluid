#include "fileio/fileioread.h"
#include "fileio/fileioplot.h"


int main()
{
	fileio::Data data = fileio::Read::loop_until_proper_files();
	fileio::Plot::with_radius2(data.mnsc, data.radius, 1, 1);
	
	return 0;
}
