#include "make/necessary_compile.h"

void NecessaryCompile::run(std::ofstream& fout, const std::vector<File>& vfile)
{
	std::vector<std::string> vec_exe = Utility::vec_exe(vfile);
	Print::cmd(fout, "necessary_compile", vec_exe);
	Print::echo(fout, "Command executed = necessary_compile");
	
	clear_plot_folder(fout);
}

void NecessaryCompile::clear_plot_folder(std::ofstream& fout)
{
	Print::script(fout, "rm -rf run/output_files/plots");
	Print::script(fout, "mkdir run/output_files/plots");
}
