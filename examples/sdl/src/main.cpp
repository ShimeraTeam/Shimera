#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>
namespace {
	void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
		for (int w = 0; w < radius * 2; w++) {
			for (int h = 0; h < radius * 2; h++) {
				const int dx = radius - w;
				const int dy = radius - h;
				if ((dx*dx + dy*dy) <= (radius * radius)) {
					SDL_RenderPoint(renderer, static_cast<float>(centerX + dx), static_cast<float>(centerY + dy));
				}
			}
		}
	}

	void drawRectangle(SDL_Renderer* renderer, int x, int y, int width, int height) {
		const SDL_FRect rect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
		SDL_RenderFillRect(renderer, &rect);
	}

	void drawTriangle(SDL_Renderer* renderer, int x, int y, int size) {
		for (int i = 0; i < size; i++) {
			const int lineY = y + i;
			const int leftX = x + size/2 - i/2;
			const int rightX = x + size/2 + i/2;
			for (int px = leftX; px <= rightX; px++) {
				SDL_RenderPoint(renderer, static_cast<float>(px), static_cast<float>(lineY));
			}
		}
	}
}

int main() {
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Init error : " << SDL_GetError() << '\n';
		return EXIT_FAILURE;
	}
	auto *window = SDL_CreateWindow(
		"SDL3 - Formes géométriques",
		800, 400,
		SDL_WINDOW_OPENGL
	);
	if(!window) {
		std::cerr << "Window creation error : " << SDL_GetError() << '\n';
		SDL_Quit();
		return EXIT_FAILURE;
	}
	auto *renderer = SDL_CreateRenderer(window, nullptr);
	if(!renderer) {
		std::cerr << "Renderer creation error : " << SDL_GetError() << '\n';
		SDL_DestroyWindow(window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	bool isRunning = true;
	while(isRunning) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_EVENT_QUIT:
					isRunning = false;
					break;
				default:
					break;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Dessiner un cercle (rouge) à gauche
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		drawCircle(renderer, 175, 200, 80);

		// Dessiner un carré (vert) au centre
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		drawRectangle(renderer, 400 - 160/2, 200 - 160/2, 160, 160);

		// Dessiner un triangle (bleu) à droite
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		drawTriangle(renderer, 545, 120, 160);

		SDL_RenderPresent(renderer);
		SDL_Delay(16); // ~60 FPS
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}