#ifndef _PLACE_PANNEL_H_
#define _PLACE_PANNEL_H_

#include"panel.h"
#include"ResourceManager.h"
#include"ConfigManager.h"
#include"CoinManager.h"
#include"TowerMananger.h"

#include<SDL2_gfxPrimitives.h>

class PlacePanel :public Panel
{
public:
	PlacePanel()
	{
		const ResourcesManager::TexturePool& tex_pool = ResourcesManager::instance()->get_texture_pool();
		tex_idle = tex_pool.find(ResID::Tex_UIPlaceIdle)->second;
		tex_top = tex_pool.find(ResID::Tex_UIPlaceHoveredTop)->second;
		tex_left = tex_pool.find(ResID::Tex_UIPlaceHoveredLeft)->second;
		tex_right = tex_pool.find(ResID::Tex_UIPlaceHoveredRight)->second;
	}

	~PlacePanel() = default;

	void on_update(SDL_Renderer* renderer)override
	{
		ConfigManager* instance = ConfigManager::instance();

		val_top = instance->axeman_template.cost[instance->level_axeman];
		val_left = instance->archer_template.cost[instance->level_archar];
		val_right = instance->gunner_template.cost[instance->level_gunner];

		top_region = instance->axeman_template.view_range[instance->level_axeman];
		left_region = instance->archer_template.view_range[instance->level_archar];
		right_region = instance->gunner_template.view_range[instance->level_gunner];

		Panel::on_update(renderer);
	}

	void on_render(SDL_Renderer* renderer)override
	{
		if (!is_visabel)
			return;

		int region = 0;
		switch (hovered_target)
		{
		case Panel::HoveredTarget::Top:
			region = top_region;
			break;
		case Panel::HoveredTarget::Left:
			region = left_region;
			break;
		case Panel::HoveredTarget::Right:
			region = right_region;
			break;
		}

		if (region > 0)
		{
			filledCircleRGBA(renderer, center_pos.x, center_pos.y,
				region * SIZE_TILE, color_region_content.r, color_region_content.g, color_region_content.b, color_region_content.a);
			aacircleRGBA(renderer, center_pos.x, center_pos.y,
				region * SIZE_TILE, color_region_frame.r, color_region_frame.g, color_region_frame.b, color_region_frame.a);
		}
	
		Panel::on_render(renderer);
	}

protected:
	void on_click_top_area() override
	{
		CoinManager* instace = CoinManager::instance();

		if (val_top <= instace->get_current_coin_num())
		{
			TowerManager::instance()->place_tower(TowerType::Axeman, idx_tile);
			instace->decrease_coin(val_top);
		}
	}
	void on_click_left_area() override
	{
		CoinManager* instace = CoinManager::instance();

		if (val_left <= instace->get_current_coin_num())
		{
			TowerManager::instance()->place_tower(TowerType::Archer, idx_tile);
			instace->decrease_coin(val_left);
		}
	}
	void on_click_right_area() override
	{
		CoinManager* instace = CoinManager::instance();

		if (val_right <= instace->get_current_coin_num())
		{
			TowerManager::instance()->place_tower(TowerType::Gunner, idx_tile);
			instace->decrease_coin(val_right);
		}
	}

private:
	int top_region = 0, left_region = 0, right_region = 0;
private:
	SDL_Color color_region_frame = { 30, 80, 162, 175 };
	SDL_Color color_region_content = { 0, 149, 217, 75 };
};


#endif // !PlacePannel

