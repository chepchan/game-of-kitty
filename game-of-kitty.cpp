#include <iostream>
#include <chrono>
#include <thread>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace std::chrono_literals;

class GameOfLife : public olc::PixelGameEngine
{
public:
	GameOfLife()
	{
		sAppName = "GameOfLife";
	}
    olc::Sprite* spriteKitty = nullptr;
	olc::Decal* decalKitty = nullptr;

    int** create2DArray(int rows, int cols)
    {
        int ** array;
        array = new int * [rows];
        for (int i = 0; i < rows; i++){
            array[i] = new int[cols];
        }
        return array;
    }

    int countNeighbors(int **grid, int x, int y)
    {
        int sum = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {

                int colsWrap = (x + i + cols) % cols;
                int rowsWrap = (y + j + rows) % rows;

                sum += grid[colsWrap][rowsWrap];
            }
        }
        sum -= grid[x][y];
        return sum;
    }

private:
    int ** grid;
    int cols;
    int rows;
    int resolution = 28;

public:
	bool OnUserCreate() override
	{
        spriteKitty = new olc::Sprite("kitty.png");
        decalKitty = new olc::Decal(spriteKitty);

        cols = ScreenWidth() / resolution;
        rows = ScreenHeight() / resolution;

        grid = create2DArray(cols, rows);

        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                grid[i][j] = rand() % 2;
            }
        }
 
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        std::this_thread::sleep_for(30ms); //sleep

        Clear(olc::Pixel(255, 161, 203));

        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                int x = i * resolution;
                int y = j * resolution;
                if (grid[i][j] == 1)
                {
                    DrawDecal({(float)x, (float)y}, decalKitty);
                }else{
                    DrawRect(x, y, resolution - 1, resolution - 1, olc::Pixel(255, 161, 203));
                    FillRect(x, y, resolution, resolution, olc::Pixel(255, 161, 203));
                }                                                       
            }
        }

        //second 2d array representing the next generation
        int ** next;
        next = create2DArray(cols, rows);
        
        //compute next based on grid
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {

                int state = grid[i][j];

                    //count live neighbors
                    int sum = 0;
                    int neighbors = countNeighbors(grid, i, j);

                    //implement rules:
                    if (state == 0 && neighbors == 3){
                        next[i][j] = 1;
                    } else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                        next[i][j] = 0;
                    } else {
                        next[i][j] = state;
                }
            }
        }
        grid = next;

		return true;
	}
};


int main()
{
	GameOfLife demo;
	if (demo.Construct(1200, 1000, 1, 1))
		demo.Start(); 

	return 0;
}