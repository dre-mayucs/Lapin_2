#include "World_inport.h"
#include <string>
#include <fstream>
#include <sstream>

World_inport::World_inport()
{
	return;
}

void World_inport::Inport(int World[][2], std::string Stage_name, int *value)
{
	std::string File = "Resources\\scenes\\" + Stage_name;
	
	int count = 0;
	int count2 = 0;

	std::string w_cache, cache[4], cache_2[100];
	std::ifstream stream_File(File);

	//“Ç‚Ýž‚Ý
	stream_File >> w_cache;

	//‘æ‚QƒLƒƒƒbƒVƒ…‚ÖŠi”[
	for (auto cache_str : S_Split(w_cache, '|')) {
		std::istringstream stream = std::istringstream(cache_str);
		stream >> cache_2[count++];
	}
	*value = count;

	for (auto i = 0; i < count; i++) {
		for (auto cache_str : S_Split(cache_2[i], ',')) {
			std::istringstream stream = std::istringstream(cache_str);
			stream >> cache[count2++];
		}

		World[i][0] = std::stoi(cache[0]);
		World[i][1] = std::stoi(cache[2]);

		for (auto i = 0; i < 4; i++) {
			cache[i] = "null";
		}
		count2 = 0;
	}
}
