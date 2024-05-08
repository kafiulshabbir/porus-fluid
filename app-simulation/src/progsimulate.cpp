#include "fileioread.h"
#include "funcglobal.h"

//ffmpeg -framerate 10 -i filename-%03d.jpg output.mp4

int main()
{
	//std::cout << std::setprecision(2) << std::scientific;

	fileio::Data data = fileio::Read::loop_until_proper_files();
	func::Global::simulate(
		data.radius,
		data.mnsc,
		data.diamension
	);
	return 0;
}
