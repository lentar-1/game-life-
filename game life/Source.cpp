#include <iostream>
#include <windows.h>
#include <sstream>
#include "glfw3.h"

const int WI = 1920, HI = 1080;
const int squareSize = 3;

void DrawGame(bool** bufferOfBooleans, short** bufferOfSquares);
void CheckBuffer(bool** bufferOfBooleans, short** bufferOfSquares);
void CalculateBuffer(bool** bufferOfBooleans, short** bufferOfSquares);
void DisplayInformation(GLFWwindow*);

void DrawGame(bool** bufferOfBooleans, short** bufferOfSquares) {
   
    CheckBuffer(bufferOfBooleans, bufferOfSquares);
    CalculateBuffer(bufferOfBooleans, bufferOfSquares);
    Sleep(1000);
}

void CheckBuffer(bool** bufferOfBooleans, short** bufferOfSquares) {
    glBegin(GL_POINTS);
    for (int y = 0; y < HI / squareSize; y++)
    {
        for (int x = 0; x < WI / squareSize; x++)
        {
            if (bufferOfSquares[y][x] == 3)
            {
                bufferOfBooleans[y][x] = true;
                glVertex2i(x, y);
            }
            else if (bufferOfSquares[y][x] == 2 && bufferOfBooleans[y][x] == true) 
                glVertex2i(x, y);
            else
                bufferOfBooleans[y][x] = false;

            bufferOfSquares[y][x] = 0;
        }
    }
    glEnd();

}

void CalculateBuffer(bool** bufferOfBooleans, short** bufferOfSquares) {
    for (int y = 0; y < HI / squareSize; y++) {
        for (int x = 0; x < WI / squareSize; x++)
        {
            if (bufferOfBooleans[y][x] == true) {
                if (x<1 || x>(WI / squareSize) - 2 || y<1 || y>(HI / squareSize) - 2)
                    continue;
                bufferOfSquares[y + 1][x - 1]++;
                bufferOfSquares[y + 1][x]++;
                bufferOfSquares[y + 1][x + 1]++;
                bufferOfSquares[y][x - 1]++;
                bufferOfSquares[y][x + 1]++;
                bufferOfSquares[y - 1][x - 1]++;
                bufferOfSquares[y - 1][x]++;
                bufferOfSquares[y - 1][x + 1]++;
            }
        }
    }
}

void InitializeBoard(short** bufferOfSquares) {
    srand(time(NULL));
    int density = WI * HI / squareSize/squareSize;
    for (int i = 0; i < (rand()%(density/8))+density/4; i++)
    {
            int y = rand() % (HI / squareSize);
            int x = rand() % (WI / squareSize);
            if (bufferOfSquares[y][x] == 0) {
                bufferOfSquares[y][x] = 3;
            }
    }
}

int main()
{
    short** bufferOfSquares = new short* [HI / squareSize];
    bool** bufferOfBooleans = new bool* [HI / squareSize];

    for (int i = 0; i < HI / squareSize; i++)
    {
        bufferOfSquares[i] = new short[WI / squareSize] {0};
    }
    for (int i = 0; i < HI / squareSize; i++)
    {
        bufferOfBooleans[i] = new bool[WI / squareSize] {false};
    }

    InitializeBoard(bufferOfSquares);

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WI, HI, "game \"life\"", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glOrtho(0, WI / squareSize, 0, HI / squareSize, 1.0f, -1.0f);
    glPointSize(squareSize);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    int genI = 0;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSetWindowTitle(window, ((std::string)"Game \"life\". Generation: " + std::to_string(genI)).c_str());
        DrawGame(bufferOfBooleans, bufferOfSquares);
        glfwSwapBuffers(window);
        glfwPollEvents();
        genI++;
    }

    for (int i = 0; i < HI / squareSize; i++)
    {
        delete[]bufferOfSquares[i];
    }
    for (int i = 0; i < HI / squareSize; i++)
    {
        delete[]bufferOfBooleans[i];
    }
    delete[] bufferOfSquares;
    delete[] bufferOfBooleans;
    glfwTerminate();
    return 0;
}


