#pragma once
#include <functional>
#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_image.h>

class VisualComponent {
private:
	SDL_Texture* TextureState1, *TextureState2;
	

public:
	SDL_Texture* Texture;
	int X, Y, Height, Width;
	std::string Name;
	bool IsState1;
	std::function<void(void)> OnClick;

	void DoClick(void);
public:
	bool IsInFocus;

	std::function<void(std::string)> OnKeyPress;

	VisualComponent(std::string name, SDL_Renderer* render);
	VisualComponent();
	~VisualComponent();
};


class Button {
public:
	int TextureID;
	int X, Y, Height, Width;
	std::function<void(void)> OnClick;
	

};

class CheckBox: public VisualComponent {

};

class EditBox : public VisualComponent {

};