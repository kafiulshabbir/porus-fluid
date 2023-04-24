#include "make/all.h"

void All::run(std::ofstream& fout)
{
	Print::cmd(fout, "all", {"necessary_compile"});
	Print::echo(fout, "Command executed = all/default make");
}
