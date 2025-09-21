#include "../includes/CA.h"
#include <thread>
#include <chrono>

namespace CellularAutomata
{
	uint8_t* outGrid = nullptr;
	uint8_t* currStateGrid = nullptr;
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

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	// Traverse grid, ignore boundary
	for (int i = 1; i < CellularAutomata::width - 1; ++i)
	{
		for (int j = 1; j < CellularAutomata::height - 1; ++j)
		{
			uint8_t neighbours = outGrid[(j - 1) * CellularAutomata::width + (i - 1)] + // Top Left	
								 outGrid[(j - 1) * CellularAutomata::width + i]       + // Top Middle
								 outGrid[(j - 1) * CellularAutomata::width + (i + 1)] + // Top Right
								 outGrid[j * CellularAutomata::width + (i - 1)]       + // Middle Left
								 outGrid[j * CellularAutomata::width + (i + 1)]       + // Middle Right
								 outGrid[(j + 1) * CellularAutomata::width + (i - 1)] + // Bottom Left
								 outGrid[(j + 1) * CellularAutomata::width + i]       + // Bottom Middle
								 outGrid[(j + 1) * CellularAutomata::width + (i + 1)];  // Bottom Right


			if (outGrid[j * CellularAutomata::width + i] == 0x1) // If Alive
			{
				currStateGrid[j * CellularAutomata::width + i] = neighbours == 2 || neighbours == 3 ? 0x1 : 0x0;
				glBegin(GL_POINTS);
					glColor3f(0.0f, 128.0f, 128.0f);
					glVertex2i(i, j);
				glEnd();
			}
			else // If Not Alive
			{
				currStateGrid[j * CellularAutomata::width + i] = neighbours == 3 ? 0x1 : 0x0;
				glBegin(GL_POINTS);
					glColor3f(0.0f, 0.0f, 0.0f);
					glVertex2i(i, j);
				glEnd();
			}
		}
	}

	glFlush();
	glutSwapBuffers();
}

void CellularAutomata::run()
{
	glutMainLoop();
}