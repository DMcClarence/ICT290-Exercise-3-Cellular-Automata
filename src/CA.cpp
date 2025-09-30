#include "../includes/CA.h"
#include <thread>
#include <chrono>

namespace CellularAutomata
{
	uint8_t* outGrid = nullptr;
	uint8_t* currStateGrid = nullptr;

	namespace Helpers
	{
		uint8_t sumNeighbours(const int x, const int y);
		void evaluateState(uint8_t& cell, const uint8_t currState, const uint8_t numNeighbours);
		void drawCell(const int x, const int y, const uint8_t alive);
	}
}

void CellularAutomata::init(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(CellularAutomata::width, CellularAutomata::height);
	glutCreateWindow("Cellular Automata");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	outGrid = new uint8_t[CellularAutomata::width * CellularAutomata::height];
	currStateGrid = new uint8_t[CellularAutomata::width * CellularAutomata::height];
	memset(outGrid, 0, CellularAutomata::width * CellularAutomata::height);
	memset(currStateGrid, 0, CellularAutomata::width * CellularAutomata::height);

	for (int i = 0; i < CellularAutomata::width * CellularAutomata::height; ++i)
	{
		currStateGrid[i] = rand() % 2;
	}

	glutDisplayFunc(CellularAutomata::displayCallback);
	glutIdleFunc(CellularAutomata::displayCallback);
}

void CellularAutomata::displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (double)CellularAutomata::width, (double)CellularAutomata::height, 0.0);

	memcpy(outGrid, currStateGrid, CellularAutomata::width * CellularAutomata::height);

	// Slow down simulation
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	// Traverse grid, ignore boundary
	for (int i = 1; i < CellularAutomata::width - 1; ++i)
	{
		for (int j = 1; j < CellularAutomata::height - 1; ++j)
		{
			uint8_t neighbours = CellularAutomata::Helpers::sumNeighbours(i, j);

			CellularAutomata::Helpers::evaluateState(currStateGrid[j * CellularAutomata::width + i],
											         outGrid[j * CellularAutomata::width + i], neighbours);

			CellularAutomata::Helpers::drawCell(i, j, currStateGrid[j * CellularAutomata::width + i]);
		}
	}

	glFlush();
	glutSwapBuffers();
}

uint8_t CellularAutomata::Helpers::sumNeighbours(const int x, const int y)
{
	return outGrid[(y - 1) * CellularAutomata::width + (x - 1)] + // Top Left	
		   outGrid[(y - 1) * CellularAutomata::width + x]       + // Top Middle
		   outGrid[(y - 1) * CellularAutomata::width + (x + 1)] + // Top Right
		   outGrid[y * CellularAutomata::width + (x - 1)]       + // Middle Left
		   outGrid[y * CellularAutomata::width + (x + 1)]       + // Middle Right
		   outGrid[(y + 1) * CellularAutomata::width + (x - 1)] + // Bottom Left
		   outGrid[(y + 1) * CellularAutomata::width + x]       + // Bottom Middle
		   outGrid[(y + 1) * CellularAutomata::width + (x + 1)];  // Bottom Right
}

void CellularAutomata::Helpers::evaluateState(uint8_t& cell, const uint8_t state, const uint8_t numNeighbours)
{
	if (state == 0x1) // If Alive
	{
		cell = numNeighbours == 2 || numNeighbours == 3 ? 0x1 : 0x0;
	}
	else // If Not Alive
	{
		cell = numNeighbours == 3 ? 0x1 : 0x0;
	}
}

void CellularAutomata::Helpers::drawCell(const int x, const int y, const uint8_t alive)
{
	glBegin(GL_POINTS);
	alive == 0x01 ? glColor3f(0.0f, 128.0f, 128.0f) : glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2i(x, y);
	glEnd();
}

void CellularAutomata::run()
{
	glutMainLoop();
}