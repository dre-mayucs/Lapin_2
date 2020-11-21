#pragma once
#include "Split.h"

class World_inport : public Split
{
public:
	int World_value;

	World_inport();
	void Inport(int World[][2], std::string Stage_name);
};

