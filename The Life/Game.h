#pragma once

#include <string>
#include <vector>
#include <memory>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_image.h>

#include "VisualComponents.h"

const int WINDOWS_WIDTH = 1100;
const int WINDOWS_HEIGHT = 805;

class Game {
private:
	SDL_Window* Window;
	SDL_Renderer* Render;
	SDL_Texture* TextureDeadCell;
	SDL_Texture* TextureButtonRestart;
	//std::vector<std::unique_ptr<SDL_Texture>> TextureAtlas;
	
	void CreateRender();
	SDL_Texture* LoadTexture(std::string FileName);
	void LoadAllTextures();
	void UpdateGraphic();
	int HandleInput();
private:
	int CellsSize = 5;
	int CellsCountW = 180;
	int CellsCountH = 150;
	unsigned int Seed;
	std::vector<std::vector<bool>> Cells;
	void ClearCells();

	void InitCells();
	void UpdateCells();
	bool IsCheckCellLife(int x, int y);

private:
	bool IsPausing;
	int FocusElement = -1;
	int WhereCliced(int x, int y);
private:
	bool IsWrapField;
	double UpdateTime = 0.1;
private:
	std::vector<std::unique_ptr<VisualComponent>> VisualComponents;
	int IdResetButton, IdWrapButton, IdLoadTXTButton, IdSaveSeedButton, IdLoadSeedButton, IdCountEdit;
	void InitVisualComponents();
private:
	void LoadSettingsFromTXT();
	double ParseLine(std::string Line);

	void SaveSeedtoFile();
	void LoadSeedtoFile();
public:
	void Run();
	
	~Game();

};

