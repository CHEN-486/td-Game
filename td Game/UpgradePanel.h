#ifndef _UPGRATEPANEL_H_
#define _UPGRATEPANEL_H_

#include"panel.h"
#include"ResourceManager.h"
#include"ConfigManager.h"
#include"CoinManager.h"
#include"TowerMananger.h"

class UpgradePanel:public Panel
{
public:
	UpgradePanel()
	{
		const ResourcesManager::TexturePool& tex_pool = ResourcesManager::instance()->get_texture_pool();

		tex_idle = tex_pool.find(ResID::Tex_UIUpgradeIdle)->second;
		tex_top = tex_pool.find(ResID::Tex_UIUpgradeHoveredTop)->second;
		tex_left = tex_pool.find(ResID::Tex_UIUpgradeHoveredLeft)->second;
		tex_right = tex_pool.find(ResID::Tex_UIUpgradeHoveredRight)->second;
	}

	~UpgradePanel() = default;

	void on_update(SDL_Renderer* renderer)override
	{
		const ConfigManager* instance = ConfigManager::instance();
		
		val_top = instance->axeman_template.upgrade_cost[instance->level_axeman];
		val_left = instance->archer_template.upgrade_cost[instance->level_archar];
		val_right = instance->gunner_template.upgrade_cost[instance->level_gunner];

		Panel::on_update(renderer);
	}

protected:
	void on_click_top_area() override
	{
		CoinManager* instance = CoinManager::instance();

		if (val_top > 0 && val_top <= instance->get_current_coin_num())
		{
			TowerManager::instance()->upgrade_tower(TowerType::Axeman);
			instance->decrease_coin(val_top);
		}
	}
	void on_click_left_area() override
	{
		CoinManager* instance = CoinManager::instance();

		if (val_left > 0 && val_left <= instance->get_current_coin_num())
		{
			TowerManager::instance()->upgrade_tower(TowerType::Archer);
			instance->decrease_coin(val_left);
		}
	}
	void on_click_right_area() override
	{
		CoinManager* instance = CoinManager::instance();

		if (val_right > 0 && val_right <= instance->get_current_coin_num())
		{
			TowerManager::instance()->upgrade_tower(TowerType::Gunner);
			instance->decrease_coin(val_right);
		}
	}

};


#endif // !_UPGRATEPANEL_H_

