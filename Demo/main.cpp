#define SDL_MAIN_HANDLED

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

#include<cJSON.h>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include<SDL2_gfxPrimitives.h>

void test_json()
{
	std::ifstream file("test.json");
	if(!file.good())
	{
		std::cout << "can not open file" << std::endl;
		return;
	}

	std::stringstream str_stream;
	str_stream << file.rdbuf();
	file.close();

	cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

	cJSON* json_name = cJSON_GetObjectItem(json_root, "name");
	cJSON* json_age = cJSON_GetObjectItem(json_root, "age");
	cJSON* json_pets = cJSON_GetObjectItem(json_root, "pets");

	std::cout << json_name->string << ":" << json_name->valuestring << std::endl;
	std::cout << json_age->string << ":" << json_age->valueint << std::endl;
	std::cout << json_pets->string << ":";

	cJSON* json_item = nullptr;
	cJSON_ArrayForEach(json_item,json_pets)
	{
		std::cout << json_item->valuestring<<" ";
	}
	std::cout << std::endl;
}

void test_csv()
{
	std::ifstream file("test.csv");
	if(!file.good())
	{
		std::cout << "can not open file" << std::endl;
		return;
	}

	std::string str_line;
	while(getline(file,str_line))
	{
		std::string str_grid;
		std::stringstream str_stream(str_line);
		while (getline(str_stream, str_grid, ','))
		{
			std::cout << str_grid << " ";
		}
		std::cout << std::endl;
	}

	file.close();
}

int main()
{
	test_json();
	test_csv();
	
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	SDL_Window* window = SDL_CreateWindow(u8"你好 但某", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* suf_img = IMG_Load("avatar.jpg");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	TTF_Font* font = TTF_OpenFont("ipix.ttf", 32);
	SDL_Color color = { 255,255,255,255 };
	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"你好 世界", color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	Mix_Music* music = Mix_LoadMUS("music.mp3");
	Mix_FadeInMusic(music, -1, 1500);

	const int fps = 60;
	bool is_quit = false;

	SDL_Event event;
	SDL_Point pos_cursor = { 0,0 };
	SDL_Rect rec_img, rec_text;

	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 counter_freq = SDL_GetPerformanceFrequency();

	rec_img.w = suf_img->w;
	rec_img.h = suf_img->h;

	rec_text.w = suf_text->w;
	rec_text.h = suf_text->h;

	while (!is_quit)
	{
		//input 
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				pos_cursor.x = event.motion.x;
				pos_cursor.y = event.motion.y;
			}

		}
		//delay(产生的delta输入on_update)
		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)((current_counter - last_counter) / counter_freq);
		if (delta * 1000 < 1000.0 / fps)
			SDL_Delay((Uint32)(1000.0 / fps - delta * 1000));
		//date
		rec_img.x = pos_cursor.x;
		rec_img.y = pos_cursor.y;

		rec_text.x = pos_cursor.x;
		rec_text.y = pos_cursor.y;
		//render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, tex_img, nullptr, &rec_img);
		filledCircleRGBA(renderer, pos_cursor.x, pos_cursor.y, 50, 0, 255, 0, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &rec_text);

		SDL_RenderPresent(renderer);

	}
	return 0;
}