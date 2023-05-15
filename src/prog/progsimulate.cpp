#include "fileio/fileioread.h"
#include "func/funcglobal.h"

//ffmpeg -framerate 10 -i filename-%03d.jpg output.mp4

int main()
{
	//std::cout << std::setprecision(1) << std::scientific;
	
	fileio::Data data = fileio::Read::loop_until_proper_files();
	func::Global::simulate(data.radius, data.mnsc, data.diamension);
	return 0;
}