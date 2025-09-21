#ifndef CA_H
#define CA_H

#include "GL/freeglut.h"

#include <cstdint>

namespace CellularAutomata
{
	constexpr int width = 800;
	constexpr int height = 600;

	extern uint8_t* outGrid;
	extern uint8_t* currStateGrid;

	void init(int argc, char** argv);
	void displayCallback();
	void run();
}

#endif
