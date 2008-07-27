/*
 * Copyright (C) 2002-2003, 2006-2008 by the Widelands Development Team
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef INTERACTIVE_PLAYER_H
#define INTERACTIVE_PLAYER_H

#include "game.h"
#include "interactive_base.h"

#include "ui_button.h"
#include "ui_textarea.h"

#include <SDL_keyboard.h>

#include <vector>

namespace UI {
struct Multiline_Textarea;
struct Textarea;
};

class ChatProvider;
struct Interactive_PlayerImpl;

/**
 * This is the interactive player. this one is
 * responsible to show the correct map
 * to the player and draws the user interface,
 * cares for input and so on.
 */
struct Interactive_Player : public Interactive_Base {
	struct Game_Main_Menu_Windows {
		UI::UniqueWindow::Registry loadgame;
		UI::UniqueWindow::Registry savegame;
		UI::UniqueWindow::Registry readme;
		UI::UniqueWindow::Registry keys;
		UI::UniqueWindow::Registry authors;
		UI::UniqueWindow::Registry license;
		UI::UniqueWindow::Registry sound_options;

		UI::UniqueWindow::Registry building_stats;
		UI::UniqueWindow::Registry general_stats;
		UI::UniqueWindow::Registry ware_stats;
		UI::UniqueWindow::Registry stock;
	};

	Interactive_Player(Widelands::Game &, Widelands::Player_Number, bool, bool);
	~Interactive_Player();

	void start();

	void toggle_main_menu ();
	void toggle_chat        ();
	void toggle_options_menu();
	void toggle_objectives();
	void toggle_buildhelp ();
	void toggle_resources ();
	void toggle_help      ();

	void field_action();

	bool handle_key(bool down, SDL_keysym);

	Widelands::Game * get_game() const {return m_game;}
	Widelands::Game & game() const {return *m_game;}
	Widelands::Player_Number get_player_number() const {return m_player_number;}
	Widelands::Player & player() const throw () {return m_game->player(m_player_number);}
	Widelands::Player * get_player() const throw () {
		assert(m_game);
		return m_game->get_player(m_player_number);
	}

	// for savegames
	void set_player_number(uint32_t plrn);

	// For load
	virtual void cleanup_for_load();
	void postload();

	// Chat messages
	void set_chat_provider(ChatProvider* chat);
	ChatProvider* get_chat_provider();

private:
	Interactive_PlayerImpl* m;
	Widelands::Game        * m_game;
	Widelands::Player_Number m_player_number;

	UI::Button<Interactive_Player> m_toggle_chat;
	UI::Button<Interactive_Player> m_toggle_options_menu;
	UI::Button<Interactive_Player> m_toggle_main_menu;
	UI::Button<Interactive_Player> m_toggle_objectives;
	UI::Button<Interactive_Player> m_toggle_minimap;
	UI::Button<Interactive_Player> m_toggle_buildhelp;
	//UI::Button<Interactive_Player> m_toggle_resources;
	UI::Button<Interactive_Player> m_toggle_help;


	UI::UniqueWindow::Registry m_chat;
	UI::UniqueWindow::Registry m_options;
	UI::UniqueWindow::Registry m_mainmenu;
	UI::UniqueWindow::Registry m_objectives;
	UI::UniqueWindow::Registry m_fieldaction;
	UI::UniqueWindow::Registry m_encyclopedia;
	Game_Main_Menu_Windows  m_mainm_windows;
};


#endif
