/*
 * Copyright (C) 2002-2004, 2006-2007 by the Widelands Development Team
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

#ifndef __S__EVENT_CONQUER_AREA_OPTION_MENU_H
#define __S__EVENT_CONQUER_AREA_OPTION_MENU_H

#include "player_area.h"

#include "ui_window.h"

class Editor_Interactive;
class Event_Conquer_Area;
namespace UI {
struct Edit_Box;
struct Checkbox;
struct Textarea;
};

/*
 * This is a modal box - The user must end this first
 * before it can return
 */
struct Event_Conquer_Area_Option_Menu : public UI::Window {
      Event_Conquer_Area_Option_Menu(Editor_Interactive*, Event_Conquer_Area*);

	bool handle_mousepress  (const Uint8 btn, int x, int y);
	bool handle_mouserelease(const Uint8 btn, int x, int y);

private:
      void update(void);
	void clicked_ok();
      void clicked(int);

      Event_Conquer_Area* m_event;
      Editor_Interactive* m_parent;
      UI::Textarea *m_player_ta, *m_area_ta, *m_x_ta, *m_y_ta;
      UI::Checkbox *m_enable;
      UI::Edit_Box* m_name;
	Player_Area m_player_area;
};

#endif
