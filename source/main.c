#include <SDL.h>
#include "ttf/ttf.h"
#include "ttf/file.h"
#include "ttf/render.h"
#include "util.h"

#if (SDL_MAJOR_VERSION != 2) || (SDL_MINOR_VERSION != 30) || (SDL_PATCHLEVEL != 5)
#error Version 2.30.5 is expected, check SDL git submodule
#endif

void show_error(const char* title) {
	const char* error = SDL_GetError();
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, error, NULL) == 0) {
		return;
	}
	SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "%s: %s", title, error);
}

#define BEZIER_STEPS 64

// app state
int updateTitle = 1;
SDL_Window* window;
SDL_Renderer* renderer;
float dpi;
TtfFile* file;
int running = 1;
// render general settings
#define RENDERING_STRING (glyphIndex < 0)
int glyphIndex;
vec2 renderPos;
int pointsSize;
// rendering toggles
int renderEmbox = 0;
int renderBezier = 1;
int renderBezierStraight = 0;
int renderSegment = 1;

int create_window() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		show_error("Could not initialize SDL");
		goto ErrorInit;
	}
	window = SDL_CreateWindow("",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == NULL) {
		show_error("Could not create window");
		goto ErrorWindow;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	}
	if (renderer == NULL) {
		show_error("Could not create a renderer");
		goto ErrorRenderer;
	}
	if (SDL_GetDisplayDPI(SDL_GetWindowDisplayIndex(window), NULL, NULL, &dpi) != 0) {
		show_error("Could not query a display's DPI parameter");
	}
	return 0;
ErrorRenderer:
	SDL_DestroyWindow(window);
ErrorWindow:
	SDL_Quit();
ErrorInit:
	return -1;
}

void process_event(SDL_Event* event) {
	if (event->type == SDL_QUIT) {
		running = 0;
	}
	if (event->type == SDL_DROPFILE) {
		free_ttf(file);
		file = read_ttf_file(event->drop.file);
		if(file == NULL) {
			show_error("Couldn't read TTF file");
		} else {
			glyphIndex = 0;
			pointsSize = 72;
			renderPos.x = 0;
			renderPos.y = dpi;
			updateTitle = 1;
		}
	}
	if (file != NULL) {
		if (event->type == SDL_KEYDOWN) {
			float step = (event->key.keysym.mod & KMOD_SHIFT) ? 10 : 1;
			// glyph index
			if (!RENDERING_STRING) {
				if (event->key.keysym.sym == SDLK_RIGHT) {
					glyphIndex++;
				} else if (event->key.keysym.sym == SDLK_LEFT) {
					glyphIndex--;
				}
			
				if (glyphIndex < 0) {
					glyphIndex += file->maxp.numGlyphs;
				}
				glyphIndex %= file->maxp.numGlyphs;
			}
			// text size
			if (event->key.keysym.sym == SDLK_UP) {
				pointsSize += step;
			} else if (event->key.keysym.sym == SDLK_DOWN) {
				pointsSize -= step;
			}
			if (pointsSize < 1) {
				pointsSize = 1;
			}
			// text position
			if (event->key.keysym.sym == SDLK_w) {
				renderPos.y -= step;
			} else if (event->key.keysym.sym == SDLK_s) {
				renderPos.y += step;
			} else if (event->key.keysym.sym == SDLK_a) {
				renderPos.x -= step;
			} else if (event->key.keysym.sym == SDLK_d) {
				renderPos.x += step;
			}
			int w, h;
			if (!SDL_GetRendererOutputSize(renderer, &w, &h)) {
				w = SDL_MAX_SINT32;
				h = SDL_MAX_SINT32;
			}
			renderPos.x = clamp(0, renderPos.x, w);
			renderPos.y = clamp(0, renderPos.y, h);
			// render toggles
			if (event->key.keysym.sym == SDLK_l) {
				renderSegment = !renderSegment;
			} else if (event->key.keysym.sym == SDLK_k) {
				renderBezier = !renderBezier;
			} else if (event->key.keysym.sym == SDLK_i) {
				renderBezierStraight = !renderBezierStraight;
			} else if (event->key.keysym.sym == SDLK_o) {
				renderEmbox = !renderEmbox;
			}
		}
		updateTitle = 1;
	}
}

vec2 bezier(vec2 a, vec2 b, vec2 c, float t) {
	vec2 ab, bc, res;
	ab.x = a.x + (b.x - a.x) * t;
	ab.y = a.y + (b.y - a.y) * t;
	bc.x = b.x + (c.x - b.x) * t;
	bc.y = b.y + (c.y - b.y) * t;
	res.x = ab.x + (bc.x - ab.x) * t;
	res.y = ab.y + (bc.y - ab.y) * t;
	return res;
}

void render_callback(RenderData* data) {
	if (data->kind == TTF_RENDER_EMBOX && renderEmbox) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_FRect rect;
		rect.x = min(data->emBox.a.x, data->emBox.b.x);
		rect.y = min(data->emBox.a.y, data->emBox.b.y);
		rect.w = abs(data->emBox.a.x - data->emBox.b.x);
		rect.h = abs(data->emBox.a.y - data->emBox.b.y);
		SDL_RenderDrawRectF(renderer, &rect);
	}
	if (data->kind == TTF_RENDER_SEGMENT) {
		if (renderSegment) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawLineF(renderer,
				data->segment.a.x, data->segment.a.y,
				data->segment.b.x, data->segment.b.y);
		}
		// render points
	}
	if (data->kind == TTF_RENDER_BEZIER) {
		if (renderBezierStraight) {
			SDL_SetRenderDrawColor(renderer, 255, 64, 64, 255);
			SDL_RenderDrawLineF(renderer,
				data->bezier.a.x, data->bezier.a.y,
				data->bezier.b.x, data->bezier.b.y);
			SDL_RenderDrawLineF(renderer,
				data->bezier.b.x, data->bezier.b.y,
				data->bezier.c.x, data->bezier.c.y);
		}
		if (renderBezier) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			vec2 a;
			vec2 b;
			for (int i = 0; i < (BEZIER_STEPS - 1); i++) {
				a = bezier(data->bezier.a, data->bezier.b, data->bezier.c, i / (float)BEZIER_STEPS);
				b = bezier(data->bezier.a, data->bezier.b, data->bezier.c, (i + 1) / (float)BEZIER_STEPS);
				SDL_RenderDrawLineF(renderer, a.x, a.y, b.x, b.y);
			}
		}
		// render points
	}
}

void render() {
	if (updateTitle) {
		char* buff = NULL;
		if (file == NULL) {
			SDL_asprintf(&buff, "Drop an .ttf file into this window to load it");
		} else {
			if (RENDERING_STRING) {
				// not implemented yet
			} else {
				SDL_asprintf(&buff, "Rendering glyph %i out of %i; %ipt",
					glyphIndex, (int)(file->maxp.numGlyphs - 1), pointsSize);
			}
		}
		SDL_SetWindowTitle(window, buff);
		SDL_free(buff);
		updateTitle = 0;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	if (file != NULL) {
		if (RENDERING_STRING) {

		} else {
			vec2 at = renderPos;
			render_ttf(file, render_callback, glyphIndex, &at, pointsSize * dpi / 72);
		}
	}
	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[])
{
	if (create_window() != 0) {
		return -1;
	}
	while (1) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			process_event(&event);
		}
		if (!running) {
			break;
		}
		render();
	}
	free_ttf(file);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
