#include "Split.h"
#include <string>
#include <fstream>
#include <sstream>

std::vector<std::string> Split::S_Split(std::string str, char key)
{
	int first = 0;
	int last = str.find_first_of(key);

	std::vector<std::string> result;

	while (first < str.size())
	{
		std::string cache(str, first, last - first);
		result.push_back(cache);

		first = last + 1;
		last = str.find_first_of(key, first);

		if (last == std::string::npos)
		{
			last = str.size();
		}
	}

	return result;
}