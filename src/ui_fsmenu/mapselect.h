/*
 * Copyright (C) 2002, 2006-2009, 2011 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef WL_UI_FSMENU_MAPSELECT_H
#define WL_UI_FSMENU_MAPSELECT_H

#include <set>
#include <string>

#include "ui_fsmenu/base.h"
#include "logic/map.h"
#include "ui_basic/button.h"
#include "ui_basic/checkbox.h"
#include "ui_basic/multilinetextarea.h"
#include "ui_basic/table.h"
#include "ui_basic/textarea.h"
#include "ui_fsmenu/load_map_or_game.h"


using Widelands::Map;
class GameController;
struct GameSettingsProvider;

namespace UI {
	struct Box;
}

/**
 * Data about a map that we're interested in.
 */
struct MapData {
	using Tags = std::set<std::string>;

	std::string filename;
	std::string name;
	std::string author;
	std::string description;
	std::string hint;
	Tags tags;

	uint32_t width;
	uint32_t height;
	uint32_t nrplayers;
	bool scenario; // is this a scenario we should list?

	MapData()
		: width(0), height(0), nrplayers(0), scenario(false) {}
};

/**
 * Select a Map in Fullscreen Mode. It's a modal fullscreen menu
 */
struct FullscreenMenuMapSelect : public FullscreenMenuLoadMapOrGame {
	FullscreenMenuMapSelect(GameSettingsProvider*, GameController*);

	bool is_scenario();
	MapData const* get_map() const;
	void think() override;

private:
	void ok();
	void map_selected(uint32_t);
	void changed(bool);
	void double_clicked(uint32_t);
	void fill_list();
	bool compare_maprows(uint32_t, uint32_t);

	UI::Checkbox* _add_tag_checkbox(UI::Box*, std::string, std::string);
	void _tagbox_changed(int32_t, bool);

	int32_t const                 m_checkbox_space;

	UI::Textarea                  m_title;
	UI::Textarea                  m_label_mapname;
	UI::MultilineTextarea         m_ta_mapname;
	UI::Textarea                  m_label_author;
	UI::MultilineTextarea         m_ta_author;
	UI::Textarea                  m_label_size;
	UI::MultilineTextarea         m_ta_size;
	UI::Textarea                  m_label_players;
	UI::MultilineTextarea         m_ta_players;
	UI::Textarea                  m_label_description;
	UI::MultilineTextarea         m_ta_description;

	UI::Textarea                  m_label_load_map_as_scenario;
	UI::Checkbox                  m_cb_load_map_as_scenario;
	UI::Checkbox*                 m_cb_show_all_maps;
	std::vector<UI::Checkbox*>    m_tags_checkboxes;

	UI::Table<uintptr_t const>    m_list;
	std::string                   m_curdir, m_basedir;
	Map::ScenarioTypes            m_scenario_types;

	std::vector<std::string>      m_tags_ordered;
	std::set<uint32_t>            m_req_tags;

	std::vector<MapData>          m_maps_data;

	GameSettingsProvider*         m_settings;
	GameController*               m_ctrl;
};

#endif  // end of include guard: WL_UI_FSMENU_MAPSELECT_H
