#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

const std::string INPUT_FILE = "input.txt";
const std::string OUTPUT_FILE = "output.txt";
const std::string ARTICLE = "@article{";

const std::vector<std::string> BEGIN_KEYS
{
	"code",
	"title",
	"author",
	"journal",
	"volume",
	"number",
	"pages",
	"year",
	"publisher"
};

typedef std::map<std::string, std::string> AdvancedFormat;

std::string ExtractCode(const std::string& temp_str)
{
	const int bracket = temp_str.find('{') + 1;
	
	return std::string(temp_str.cbegin() + bracket, temp_str.cend() - 1);
}

int ExtractBegin(const std::string& temp_str)
{
	for(auto x: BEGIN_KEYS)
	{
		auto i = temp_str.find(x);
		if(i != std::string::npos)
		{
			return i;
		}
	}
	
	std::cout << "FATAL ERROR" << std::endl;
	return -1;
}

std::string ExtractKey(const std::string& temp_str)
{
	const int equal = temp_str.find('=');
	
	const int extract_begin = ExtractBegin(temp_str);
	return temp_str.substr(extract_begin, equal - extract_begin);
}

int EndBracket(std::string s)
{
	for(int i = s.size() - 1; i >= 0; -- i)
	{
		if(s[i] == '}')
		{
			return i;
		}
	}
	
	std::cout << "Fathal error" << std::endl;
	return -1;
}

std::string ExtractInfo(const std::string& temp_str, const std::string& key)
{
	const int close = EndBracket(temp_str);
	const int begin = temp_str.find('{') + 1;
	
	
	return temp_str.substr(begin, close - begin);
}



AdvancedFormat CleanAdvancedFormat()
{
	AdvancedFormat empty_adv_frmt;
	for(const std::string& key: BEGIN_KEYS)
	{
		empty_adv_frmt[key];
	}
	
	return empty_adv_frmt;
}

std::vector<AdvancedFormat> ReadInput()
{
	std::ifstream fin(INPUT_FILE);
	std::vector<AdvancedFormat> vec_refs;
	std::string temp_str;
	
	while(std::getline(fin, temp_str))
	{
		
		std::string key;
		std::string info;
		
		const auto article_position = temp_str.find(ARTICLE);

		if(article_position != std::string::npos)
		{
			vec_refs.push_back(CleanAdvancedFormat());
			
			key = "code";
			info = ExtractCode(temp_str);
		}
		else if(temp_str.size() < 6)
		{
			continue;
		}
		else
		{
			key = ExtractKey(temp_str);
			info = ExtractInfo(temp_str, key);
		}
		
		/*
		std::cout << "line- " << temp_str << std::endl;
		std::cout << "key- " << key << std::endl;
		std::cout << "info- " << info << std::endl << std::endl;
		*/
		
		vec_refs.back().at(key) = info;
	}
	
	return vec_refs;		
}


std::string Short(std::string s)
{
	int count = 0;
	for(char c: s)
	{
		count += (c == ',');
	}
	
	if(count > 1)
	{
		return s.substr(0, s.find(',')) + " et al.";
	}
	
	return s;
}

std::string AdvancedToOld(const AdvancedFormat& adv_frmt)
{
	
	const std::string code = adv_frmt.at("code");
	const std::string title = adv_frmt.at("title");
	const std::string author = adv_frmt.at("author");
	const std::string journal = adv_frmt.at("journal");
	const std::string volume = adv_frmt.at("volume");

	
	const std::string number = adv_frmt.at("number");
	const std::string pages = adv_frmt.at("pages");
	const std::string year = adv_frmt.at("year");
	const std::string publisher = adv_frmt.at("publisher");
	
	std::stringstream ss;
	
	ss << "\\bibitem[" << Short(author) << ", " << year << "] {" << code << "}";
	ss << " \\textit{" << author << "} " << title << " // ";
	
	
	if(!journal.empty())
	{
		ss << journal;
	}
	
	if(!publisher.empty())
	{
		ss << ", " << publisher;
	}
	
	if(!year.empty())
	{
		ss << ".~--- " << year;
	}
	
	if(!volume.empty())
	{
		ss << ".~--- " << " Vol.~" << volume;
	}
	
	if(!number.empty())
	{
		ss << "," << " No.~" << number;
	}
	
	if(!pages.empty())
	{
		ss << ".~--- " << "P.~"<< pages;
	}
	
	ss << ".";
	 
	return ss.str();	
}

void Run()
{
	const std::vector<AdvancedFormat> input = ReadInput();
	std::vector<std::string> v;
	
	for(const AdvancedFormat& adv_frmt: input)
	{
		v.push_back(AdvancedToOld(adv_frmt));
	}
	
	std::sort(v.begin(), v.end());
	
	std::ofstream fout(OUTPUT_FILE);
	
	for(auto x: v)
	{
		fout << x << "\n\n";
	}
}


int main()
{
	Run();
	
	return 0;
}
