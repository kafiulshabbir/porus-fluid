#ifndef FUNCCONNECTION_H
#define FUNCCONNECTION_H

#include "decl/decltypedef.h"

namespace func
{
	struct Connections
	{
		bool a = true;
		int r;
		int c;
		int p;	
	};
		
	class Connection
	{
	public:
		static int linear_nodes(int i, int j, int m);
		static std::pair<int, int> connection_ends(int r, int c, int m);
		static int total_nodes(int n, int m);
		static std::vector<Connections> FGenConnectionsEqu(int r, int c, int n, int m);
		static int FCountConnections(const std::vector<Connections>& connections);
	};
}


#endif
