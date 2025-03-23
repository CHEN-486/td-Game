#ifndef _PANEL_H_
#define _PANEL_H_

#include<SDL.h>
#include<string>

#include"tile.h"
#include"ResourceManager.h"

class Panel
{

public:
	enum class HoveredTarget
	{
		None,
		Top,
		Left,
		Right
	};

public:
	Panel()
	{
		tex_cursor = ResourcesManager::instance()
			->get_texture_pool().find(ResID::Tex_UISelectCursor)->second;
	}

	~Panel()
	{
		SDL_DestroyTexture(tex_text_background);
		SDL_DestroyTexture(tex_text_foreground);;
	}

	void on_input(const SDL_Event& event)
	{
		if (!is_visabel)return;

		switch (event.type)
		{
		case SDL_MOUSEMOTION:
		{
			SDL_Point cursor_pos = { event.motion.x,event.motion.y };
			SDL_Rect rect_target = { 0,0,size_button,size_button };

			rect_target.x = center_pos.x - width / 2 + offset_top.x;
			rect_target.y = center_pos.y - height / 2 + offset_top.y;
			if (SDL_PointInRect(&cursor_pos, &rect_target))
			{
				hovered_target = HoveredTarget::Top;
				return;
			}

			rect_target.x = center_pos.x - width / 2 + offset_left.x;
			rect_target.y = center_pos.y - height / 2 + offset_left.y;
			if (SDL_PointInRect(&cursor_pos, &rect_target))
			{
				hovered_target = HoveredTarget::Left;
				return;
			}

			rect_target.x = center_pos.x - width / 2 + offset_right.x;
			rect_target.y = center_pos.y - height / 2 + offset_right.y;
			if (SDL_PointInRect(&cursor_pos, &rect_target))
			{
				hovered_target = HoveredTarget::Right;
				return;
			}

			hovered_target = HoveredTarget::None;
			return;
		}
			break;
		case SDL_MOUSEBUTTONUP:
		{
			switch (hovered_target)
			{
			case Panel::HoveredTarget::Top:
				on_click_top_area();
				break;
			case Panel::HoveredTarget::Left:
				on_click_left_area();
				break;
			case Panel::HoveredTarget::Right:
				on_click_right_area();
				break;
			}

			is_visabel = false;
		}
			break;
		}
	}

	virtual void on_update(SDL_Renderer* renderer)
	{
		static TTF_Font* font = ResourcesManager::instance()->get_font_pool().find(ResID::Font_Main)->second;

		if (hovered_target == HoveredTarget::None) return;
		
		int val = 0;
		switch (hovered_target)
		{
		case Panel::HoveredTarget::Top:
			val = val_top;
			break;
		case Panel::HoveredTarget::Left:
			val = val_left;
			break;
		case Panel::HoveredTarget::Right:
			val = val_right;
			break;
		}

		SDL_DestroyTexture(tex_text_background);
		tex_text_background = nullptr;
		SDL_DestroyTexture(tex_text_foreground);
		tex_text_foreground = nullptr;

		std::string str_val = val < 0 ? "MAX" : std::to_string(val);

		SDL_Surface* surf_text_background = TTF_RenderText_Blended(font, str_val.c_str(), color_text_background);
		SDL_Surface* surf_text_foreground = TTF_RenderText_Blended(font, str_val.c_str(), color_text_foreground);

		width_text = surf_text_background->w; height_text = surf_text_background->h;
		tex_text_background = SDL_CreateTextureFromSurface(renderer, surf_text_background);
		tex_text_foreground = SDL_CreateTextureFromSurface(renderer, surf_text_foreground);
		SDL_FreeSurface(surf_text_background);
		SDL_FreeSurface(surf_text_foreground);
	}

	virtual void on_render(SDL_Renderer* renderer)
	{
		if (!is_visabel)
			return;


		 SDL_Rect rect_dst_cursor = {
			center_pos.x - SIZE_TILE / 2,
			center_pos.y - SIZE_TILE / 2,
			SIZE_TILE,
			SIZE_TILE
		};
		SDL_RenderCopy(renderer, tex_cursor, nullptr, &rect_dst_cursor);

		SDL_Texture* tex_pannel = nullptr;
		switch (hovered_target)
		{
		case Panel::HoveredTarget::None:
			tex_pannel = tex_idle;
			break;
		case Panel::HoveredTarget::Top:
			tex_pannel = tex_top;
			break;
		case Panel::HoveredTarget::Left:
			tex_pannel = tex_left;
			break;
		case Panel::HoveredTarget::Right:
			tex_pannel = tex_right;
			break;
		}
		SDL_Rect  rect_dst_pannel = {
			center_pos.x - width / 2,
			center_pos.y - height / 2,
			width,height
		};
		SDL_RenderCopy(renderer, tex_pannel, nullptr, &rect_dst_pannel);

		if (hovered_target == HoveredTarget::None)
			return;

		SDL_Rect rect_dst_text = { 0,0,width_text,height_text };
		rect_dst_text.x = center_pos.x - width_text / 2 + offset_shadow.x;
		rect_dst_text.y = center_pos.y - height_text / 2 + offset_shadow.y;
		SDL_RenderCopy(renderer, tex_text_background, nullptr, &rect_dst_text);
		rect_dst_text.x -= offset_shadow.x;
		rect_dst_text.y -= offset_shadow.y;
		SDL_RenderCopy(renderer, tex_text_foreground, nullptr, &rect_dst_text);
	}

	void show()
	{
		is_visabel = true;
	}

	void set_idx_tile(const SDL_Point& idx)
	{
		idx_tile = idx;
	}

	void set_center_pos(const SDL_Point& pos)
	{
		center_pos = pos;
	}

protected:
	virtual void on_click_top_area() = 0;
	virtual void on_click_left_area() = 0;
	virtual void on_click_right_area() = 0;

protected:
	bool is_visabel = false;
	SDL_Point idx_tile = { 0 };
	SDL_Point center_pos = { 0 };
	SDL_Texture* tex_idle = nullptr;
	SDL_Texture* tex_top = nullptr;
	SDL_Texture* tex_left = nullptr;
	SDL_Texture* tex_right = nullptr;
	SDL_Texture* tex_cursor = nullptr;
	HoveredTarget hovered_target = HoveredTarget::None;
	int val_top = 0, val_left = 0 , val_right = 0;

private:
	const int size_button = 48;
	const int width = 144, height = 144;
	const SDL_Point offset_top = { 48,6 };
	const SDL_Point offset_left = { 8,80 };
	const SDL_Point offset_right = { 90,80 };
	const SDL_Point offset_shadow = { 3,3 };
	const SDL_Color color_text_background = { 175,175,175,255 };
	const SDL_Color color_text_foreground = { 255,255,255,255 };

	int width_text = 0, height_text = 0;
	SDL_Texture* tex_text_background = nullptr;
	SDL_Texture* tex_text_foreground = nullptr;
};


#endif // !_PANEL_H_

