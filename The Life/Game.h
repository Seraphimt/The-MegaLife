#pragma once

#include <string>
#include <vector>
#include <memory>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_image.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


const int WINDOWS_WIDTH = 1100;
const int WINDOWS_HEIGHT = 805;

class Game {
private:
	SDL_Window* Window;
	SDL_Renderer* Render;
	SDL_Texture* TextureDeadCell;
	
	void CreateRender();
	SDL_Texture* LoadTexture(std::string FileName);
	void LoadAllTextures();
	void UpdateGraphic();
	void UpdateGUI();
	int HandleInput();
private:
	int CellsSize = 5;
	int CellsCountW = 180;
	int CellsCountH = 150;
	int FieldW = CellsCountW * CellsSize;
	unsigned int Seed;
	std::vector<std::vector<bool>> Cells;
	void ClearCells();

	void InitCells();
	void UpdateCells();
	bool IsCheckCellLife(int x, int y);

private:
	bool IsPausing;
private:
	bool IsWrapField;
	const double UpdateTime = 0.1;
	int GameSpeed = 1;
private:
	void LoadSettingsFromTXT();
	double ParseLine(std::string Line);

	void SaveSeedtoFile();
	void LoadSeedtoFile();
public:
	void Run();
	
	~Game();

};

