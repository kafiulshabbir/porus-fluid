#include <fstream>
#include <vector>

using namespace std;

ifstream radius("radius.txt");
ifstream node("node.txt");
ofstream pressure_matrix_out("pressure_matrix_out.txt");

const double AMBIENT_PRESSURE_INPUT = 10;
const double AMBIENT_PRESSURE_OUTPUT = 2;
const double LEAK_RADIUS = 1;

int main()
{
	vector<int> diamensions;
	int diamension_ipt;
	while(node >> diamension_ipt)
	{
		diamensions.push_back(diamension_ipt);
	}
	
	vector<vector<double>> radius_first(diamensions[0] - 1, vector<double>(diamensions[1]));
	
	for(auto& x: radius_first)
	{
		for(auto& y: x)
		{
			radius >> y;
		}
	}
	
	vector<vector<double>> radius_second(diamensions[0], vector<double>(diamensions[1] - 1));
	
	for(auto& x: radius_second)
	{
		for(auto& y: x)
		{
			radius >> y;
		}
	}
	
	vector<vector<double>> pressure(diamensions[0], vector<double>(diamensions[1]));
	
	vector<vector<double>> gauss_matrix;
	
	for(int i = 0; i + 1 < pressure.size(); ++ i)
	{
		for(int j = 1; j + 1 < pressure[i].size(); ++ j)
		{
			double radius_left = radius_second[i][j - 1];			
			double radius_right = radius_second[i][j];
			double radius_up = 0;
			if(i > 0)
				radius_up = radius_first[i - 1][j];
			
			double radius_down = 0;
			if(i < radius_first.size())
				radius_down = radius_first[i][j];
			
			int index_myself = diamensions[1]*i + j;
			int index_left = diamensions[1]*i + j - 1;
			int index_right = diamensions[1]*i + j + 1;
			int index_up = diamensions[1]*(i - 1)+ j;
			int index_down = diamensions[1]*(i + 1) + j;
			
			vector<double> equation(diamensions[0] * diamensions[1] + 1);
			equation[index_myself] = -(radius_left + radius_right + radius_up + radius_down);
			equation[index_left] = radius_left;
			equation[index_right] = radius_right;
			equation[index_up] = radius_up;
			equation[index_down] = radius_down;
			
			gauss_matrix.push_back(equation);
			
		}
	}
		
			
	for(auto x: radius_first)
	{
		for(auto y: x)
			out << y << ", ";
		out << '\n';
	}
	
	for(auto x: radius_second)
	{
		for(auto y: x)
			out << y << ", ";
		out << '\n';
	}
	
	out << "\ngauss\n";
	
	for(auto x: gauss_matrix)
	{
		for(auto y: x)
			out << y << ", ";
		out << '\n';
	}
	return 0;
	
}


