#include "Game.h"

#include <chrono>
#include <random>
#include <iostream>
#include <fstream>


void Game::CreateRender() {
        Window = SDL_CreateWindow("Test lands", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOWS_WIDTH, WINDOWS_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
       // if (Window == nullptr)
      //      std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
     //   if (Render == nullptr)
      //      std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui::BeginTable;
		ImGui_ImplSDL2_InitForSDLRenderer(Window, Render);
		ImGui_ImplSDLRenderer2_Init(Render);

    }

void Game::UpdateGraphic() {
	SDL_SetRenderDrawColor(Render, 0, 0, 0, 0);
	SDL_RenderClear(Render);

	//SDL_SetRenderDrawColor(Render, 255, 255, 0, 0);
	for (int i = 0; i < CellsCountW; i++) {
		for (int j = 0; j < CellsCountH; j++) {
			if (Cells[i][j]) continue;

			//int i = 0; int j = 0;
			SDL_Rect rect{};
			rect.x = i * CellsSize; rect.y = j * CellsSize;
			rect.w = CellsSize; rect.h = CellsSize;
			
			//SDL_RenderDrawRect(Render, &rect);
			SDL_RenderCopy(Render, TextureDeadCell, NULL, &rect);
		}
	}
	SDL_SetRenderDrawColor(Render, 100, 0, 0, 0);
	for (int i = 0; i < CellsCountW + 1; i++) 
		SDL_RenderDrawLine(Render, i * CellsSize, 0, i * CellsSize, CellsCountH * CellsSize);
	for (int i = 0; i < CellsCountH + 1; i++) 
		SDL_RenderDrawLine(Render, 0, i * CellsSize, CellsCountW * CellsSize, i * CellsSize);
	
	UpdateGUI();

	SDL_RenderPresent(Render);
}

void Game::UpdateGUI() {

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize({(WINDOWS_WIDTH - FieldW) * 1.0f, 200.0f });
	ImGui::SetNextWindowPos({FieldW * 1.0f, 0.0f});
	ImGui::Begin("Instruments");
	if (ImGui::Button("Restart")) InitCells();

	ImGui::Checkbox("Wrap", &IsWrapField);

	ImGui::Text("Cell size"); //itoa

	if (ImGui::SliderInt("Size", &CellsSize, 3, 13, "", 16)) InitCells();
	
	ImGui::Text("Game speed");
	ImGui::SliderInt("Speed", &GameSpeed, 1, 20, "", 16);


	ImGui::End();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Render);
}

int Game::HandleInput() {
	SDL_Event windowEvent;


	while (SDL_PollEvent(&windowEvent)) {
		ImGui_ImplSDL2_ProcessEvent(&windowEvent);
		switch (windowEvent.type) {
		case SDL_QUIT: return 1;

		case SDL_MOUSEBUTTONDOWN:
			if (windowEvent.button.button == SDL_BUTTON_RIGHT)
				IsPausing = !IsPausing;
			break;
		}
	}

	return 0;
}

Game::~Game() {
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Window);
}

void Game::LoadAllTextures() {
	TextureDeadCell = LoadTexture("Images/DeadCell.jpg");
}

void Game::ClearCells() {
	Cells.clear();
}

void Game::InitCells() {
	ClearCells();//
	Seed = unsigned(std::time(0));
	std::srand(Seed);

	FieldW = CellsCountW * CellsSize;
	CellsCountW = (WINDOWS_WIDTH - 120)/ CellsSize - 2; //пока подобрал эмперически
	CellsCountH = WINDOWS_HEIGHT / CellsSize - 1;

	for (int i = 0; i < CellsCountW; i++) {
		Cells.push_back(std::vector<bool>());

		for (int j = 0; j < CellsCountH; j++) 
			Cells[i].push_back(std::rand() % 2);
	}
}

void Game::UpdateCells() {
	std::vector<bool> buffer(CellsCountW * CellsCountH);

	for (int i = 0; i < CellsCountW; i++)
		for (int j = 0; j < CellsCountH; j++)
			buffer[i * CellsCountH + j] = IsCheckCellLife(i,j);
	for (int i = 0; i < CellsCountW; i++)
		for (int j = 0; j < CellsCountH; j++)
			Cells[i][j] = buffer[i * CellsCountH + j];
}

bool Game::IsCheckCellLife(int x, int y) {

	int shift_x[3], shift_y[3];
	shift_x[0] = -1; shift_x[1] = 0; shift_x[2] = 1;
	shift_y[0] = -1; shift_y[1] = 0; shift_y[2] = 1;
	
	if (IsWrapField){
		if (x == 0) shift_x[0] = CellsCountW - 1;
		if (x == CellsCountW - 1) shift_x[2] = -(CellsCountW - 1);
		if (y == 0) shift_y[0] = CellsCountH - 1;
		if (y == CellsCountH - 1) shift_y[2] = -(CellsCountH - 1);
	}
	else {
		if (x == 0) shift_x[0] = 0;
		if (x == CellsCountW - 1) shift_x[2] = 0;
		if (y == 0) shift_y[0] = 0;
		if (y == CellsCountH - 1) shift_y[2] = 0;
	}

	int dead_count = 0, alive_count = 0;
	for (int i = 0; i <= 2; i++)
		for (int j = 0; j <= 2; j++)
			if (Cells[x + shift_x[i]][y + shift_y[j]])
				alive_count++;
			else
				dead_count++;


	if (Cells[x][y]) //тк посчитали себя
		alive_count--;
	else
		dead_count--;
	/////todo: separate

	if (!Cells[x][y] && (alive_count == 3))
		return true;
	//if (Cells[x][y] && ((alive_count == 2)||(alive_count == 3)))
	//	return true;
	if (Cells[x][y] && ((alive_count > 3)|| (alive_count < 2)))
		return false;

	return Cells[x][y];
}


double Game::ParseLine(std::string Line) {
	size_t begin = Line.find("=");
	size_t end   = Line.find("--");
	if (end < 0) end = Line.length();

	Line = Line.substr(begin + 1, end - begin - 1);
	double result = std::stod(Line);//agtung!!!
	return result; 
}

void Game::LoadSettingsFromTXT() {
	try {
		std::ifstream inputStream("Images/Settings.txt");

		if (inputStream.is_open() == false) throw "Not found Settings.txt";

		std::vector<double> settings;
		std::string line;
		while (std::getline(inputStream, line)) {
			settings.push_back(ParseLine(line));
		}
		inputStream.close();
		//UpdateTime = settings[0];
	}
	catch (std::string str) {
	}
}

void Game::Run() {

	// init - todo вынести в отдельную функц
	InitCells();
	CreateRender();
	LoadAllTextures();


	UpdateGraphic();

	bool isRunnig = true;
	IsPausing = false;
	IsWrapField = true;
	//SDL_Event windowEvent;
	int result = 0;

	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> duration;
	double elapsed = 0;
	while (result == 0) {
		now = std::chrono::system_clock::now();
		result = HandleInput();
		UpdateGraphic(); 

		duration = std::chrono::system_clock::now() - now;
		elapsed += duration.count();

		if (!IsPausing && elapsed > UpdateTime / GameSpeed) {
			UpdateCells();
			elapsed = 0;
		}
	}
	//Quit();
}

SDL_Texture* Game::LoadTexture(std::string FileName) {
	SDL_Texture* texture = IMG_LoadTexture(Render, FileName.c_str());
	//if (texture == nullptr) {
	//	std::cout << SDL_GetError() << std::endl;
	//	return nullptr;
	//}
	return texture;
}