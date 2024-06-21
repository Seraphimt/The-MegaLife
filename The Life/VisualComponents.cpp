#include "VisualComponents.h"

void VisualComponent::DoClick(void) {
	OnClick();
	IsState1 = !IsState1;
	if (IsState1)
		Texture = TextureState1;
	else
		Texture = TextureState2;
}

VisualComponent::VisualComponent(std::string name, SDL_Renderer* render): Name(name) {
	std::string full_name = "Images/" + name + "Yes.jpg";

	TextureState1 = IMG_LoadTexture(render, full_name.c_str());
	full_name = "Images/" + name + "No.jpg";
	TextureState2 = IMG_LoadTexture(render, full_name.c_str());

	Texture = TextureState1;
	IsState1 = true;

	//if (texture == nullptr) {
	//	std::cout << SDL_GetError() << std::endl;
	//	return nullptr;
	//}
}

VisualComponent::VisualComponent() {
}

VisualComponent::~VisualComponent() {
	Texture = nullptr;
	SDL_DestroyTexture(TextureState1);
	SDL_DestroyTexture(TextureState2);
}
