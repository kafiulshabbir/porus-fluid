#include "force.h"

void Force::run(std::ofstream& fout)
{
	Print::cmd(fout, "force", {"clean", "necessary_compile"});
	Print::echo(fout, "Command executed = force");
}
