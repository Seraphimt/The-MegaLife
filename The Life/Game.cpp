#include "Game.h"

#include <chrono>
#include <random>
#include <iostream>
#include <fstream>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"


void Game::CreateRender() {
        Window = SDL_CreateWindow("Test lands", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOWS_WIDTH, WINDOWS_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
       // if (Window == nullptr)
      //      std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
     //   if (Render == nullptr)
      //      std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

       // Ratio = 800 / 3;
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui::BeginTable;
		ImGui_ImplSDL2_InitForSDLRenderer(Window, Render);
		ImGui_ImplSDLRenderer2_Init(Render);

    }

void Game::UpdateGraphic() {
	SDL_SetRenderDrawColor(Render, 0, 0, 0, 0);
	SDL_RenderClear(Render);

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize({200, 200});
	ImGui::SetNextWindowPos({ (500), 275.0f });
	ImGui::Begin("Scene");
	ImGui::Text("Temp");
	static float f = 0.0f;
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::End();
	

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
	
	SDL_SetRenderDrawColor(Render, 0, 0, 0, 0);

	SDL_Rect rect{};
	for (int i = 0; i < VisualComponents.size(); i++) {

		rect.x = VisualComponents[i]->X; rect.y = VisualComponents[i]->Y;
		rect.w = VisualComponents[i]->Width; rect.h = VisualComponents[i]->Height;

		SDL_RenderCopy(Render, VisualComponents[i]->Texture, NULL, &rect);
	}

	//SDL_RenderDrawRect(Render, &rect);

	//SDL_RenderClear(Render);
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Render);

	SDL_RenderPresent(Render);
}

int Game::HandleInput() {
	SDL_Event windowEvent;

	//ImGuiIO* t = &ImGui::GetIO();

	while (SDL_PollEvent(&windowEvent))
		switch (windowEvent.type) {
		case SDL_QUIT: return 1;
			 
		case SDL_MOUSEBUTTONDOWN:
			if (windowEvent.button.button == SDL_BUTTON_RIGHT) 
				IsPausing = !IsPausing;			
			else {
				FocusElement = WhereCliced(windowEvent.motion.x, windowEvent.motion.y);
				if (FocusElement < 0 || FocusElement > 99)
					UpdateCells();
				else
					VisualComponents[FocusElement]->DoClick();
			}
			break;

		case SDL_KEYDOWN:
			if (FocusElement > 0 && FocusElement < 100)
				//windowEvent.key.keysym

			break;
	}
	return 0;
}
Game::~Game() {
//	for (int i = 0; i < TextureAtlas.size(); i++) {
	//	SDL_DestroyTexture(TextureAtlas[i].get());
	//}
//	TextureAtlas.clear();
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(Window);
}

void Game::SaveSeedtoFile() {
	std::ofstream out("Images/Seed.txt", std::ios::out | std::ios::binary);;
	//if (out.is_open()) 
	//	out.write((char*)CellsSeed, CellsCount* CellsCount);
	
	out.close();
}

void Game::LoadSeedtoFile() {
	std::ifstream in("Images/Seed.txt", std::ios::in | std::ios::binary);;
	//if (in.is_open())
	//	in.read((char*)Cells, CellsCount * CellsCount);

	in.close();
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

	CellsCountW = (WINDOWS_WIDTH - 120)/ CellsSize - 2; //пока подобрал эмперически
	CellsCountH = WINDOWS_HEIGHT / CellsSize - 1;

	for (int i = 0; i < CellsCountW; i++) {
		Cells.push_back(std::vector<bool>());

		for (int j = 0; j < CellsCountH; j++) 
			Cells[i].push_back(std::rand() % 2);
	}
	
	//Cells[0][0] = true; Cells[1][0] = true; Cells[CellsCount - 1][0] = true;

	//auto rng = std::default_random_engine{};
	//std::shuffle(Deck.begin(), Deck.end(), rng);

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
	
	bool is_wrap = VisualComponents[IdWrapButton]->IsState1;
	if (is_wrap) {
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

int Game::WhereCliced(int x, int y) {

	int CellX = x / CellsSize, CellY = y / CellsSize;

	if (CellX >= 0 && CellX < CellsCountW && CellY >= 0 && CellY < CellsCountH) return 100;
		//return CellX * CellsCount + CellY;
	
	for (int i = 0; i < VisualComponents.size(); i++) {
		if (x >= VisualComponents[i]->X && y >= VisualComponents[i]->Y && x <= VisualComponents[i]->X + VisualComponents[i]->Width && y <= VisualComponents[i]->Y + VisualComponents[i]->Height) 
			return i;
	}

	return -1;
}

void Game::InitVisualComponents() {
	//только здесь известно явно
	IdResetButton = 0;
	VisualComponents.push_back(std::make_unique<VisualComponent>("ButtonRestart", Render));
	VisualComponents[0]->OnClick = [this]() { InitCells(); };
	VisualComponents[0]->X = CellsSize * (CellsCountW + 1);
	VisualComponents[0]->Y = 20;
	VisualComponents[0]->Height = 50;
	VisualComponents[0]->Width = 120;

	IdWrapButton = 1;
	VisualComponents.push_back(std::make_unique<VisualComponent>("CheckBox", Render));
	VisualComponents[1]->OnClick = [this]() {  };

	IdCountEdit = 2;
	VisualComponents.push_back(std::make_unique<VisualComponent>("CountEdit", Render));
	VisualComponents[2]->OnClick = [this]() {};

	IdLoadTXTButton = 3;
	VisualComponents.push_back(std::make_unique<VisualComponent>("ButtonLoadFromTXT", Render));
	VisualComponents[3]->OnClick = [this]() { LoadSettingsFromTXT(); };

	IdSaveSeedButton = 4;
	VisualComponents.push_back(std::make_unique<VisualComponent>("ButtonSaveSeed", Render));
	VisualComponents[IdSaveSeedButton]->OnClick = [this]() { SaveSeedtoFile(); };

	IdLoadSeedButton = 5;
	VisualComponents.push_back(std::make_unique<VisualComponent>("ButtonLoadSeed", Render));
	VisualComponents[IdLoadSeedButton]->OnClick = [this]() { LoadSeedtoFile(); };

	for (int i = 1; i < VisualComponents.size(); i++) 		{
		VisualComponents[i]->X = CellsSize * (CellsCountW + 1);
		VisualComponents[i]->Y = VisualComponents[i - 1]->Y + VisualComponents[i - 1]->Height + 20;
		VisualComponents[i]->Height = 50;
		VisualComponents[i]->Width = 120;

	}
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
		UpdateTime = settings[0];


	}
	catch (std::string str) {
//
	}

}

void Game::Run() {

	// init - todo вынести в отдельную функц
	InitCells();
	CreateRender();
	LoadAllTextures();
	InitVisualComponents();


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

		//	using namespace std::this_thread;     // sleep_for, sleep_until
		//	using namespace std::chrono_literals;
		if (!IsPausing && elapsed > UpdateTime) {
		//	sleep_for(1000ms);
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