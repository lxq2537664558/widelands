/*
 * Copyright (C) 2002, 2006 by the Widelands Development Team
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

#include <stdio.h>
#include "error.h"
#include "fullscreen_menu_loadgame.h"
#include "game.h"
#include "game_loader.h"
#include "game_preload_data_packet.h"
#include "graphic.h"
#include "i18n.h"
#include "layered_filesystem.h"

/*
==============================================================================

Fullscreen_Menu_LoadGame

==============================================================================
*/

Fullscreen_Menu_LoadGame::Fullscreen_Menu_LoadGame(Game *, bool) :
Fullscreen_Menu_Base("choosemapmenu.jpg"),

	// UI::Buttons

back
(this,
 570, 505, 200, 26,
 0,
 &Fullscreen_Menu_LoadGame::end_modal, this, 0,
 _("Back")),

m_ok
(this,
 570, 535, 200, 26,
 2,
 &Fullscreen_Menu_LoadGame::clicked_ok, this,
 _("OK"),
 std::string(),
 false),

	// Create the list area
list(this, 15, 205, 455, 365),

title(this, MENU_XRES / 2, 90, _("Choose saved game!"), Align_HCenter),

// Info fields
label_mapname (this, 560, 205, _("Map Name:"), Align_Right),
tamapname     (this, 570, 205, ""),
label_gametime(this, 560, 225, _("Gametime:"), Align_Right),
tagametime    (this, 570, 225, "")

{
	title.set_font(UI_FONT_BIG, UI_FONT_CLR_FG);
	list.selected.set(this, &Fullscreen_Menu_LoadGame::map_selected);
	list.double_clicked.set(this, &Fullscreen_Menu_LoadGame::double_clicked);
   fill_list();
}

Fullscreen_Menu_LoadGame::~Fullscreen_Menu_LoadGame()
{
}

void Fullscreen_Menu_LoadGame::clicked_ok()
{
	m_filename = list.get_selected();
   end_modal(1);
}

void Fullscreen_Menu_LoadGame::map_selected(uint) {
	if (const char * const name = list.get_selected()) {
      FileSystem* fs = g_fs->MakeSubFileSystem( name );
		Game_Loader gl(*fs, game);
      Game_Preload_Data_Packet gpdp;
      gl.preload_game(&gpdp); // This has worked before, no problem

		m_ok.set_enabled(true);
		tamapname.set_text(gpdp.get_mapname());

      char buf[200];
      uint gametime = gpdp.get_gametime();

      int hours = gametime / 3600000;
      gametime -= hours * 3600000;
      int minutes = gametime / 60000;

      sprintf(buf, "%02i:%02i", hours, minutes);
		tagametime.set_text(buf);

      delete fs;
   } else {
		tamapname .set_text("");
		tagametime.set_text("");
   }
}

/*
 * listbox got double clicked
 */
void Fullscreen_Menu_LoadGame::double_clicked(uint) {
   // Ok
   clicked_ok();

}

/*
 * fill the file list
 */
void Fullscreen_Menu_LoadGame::fill_list(void) {
   // Fill it with all files we find.
   g_fs->FindFiles("ssave", "*", &m_gamefiles, 1);

   Game_Preload_Data_Packet gpdp;

   for(filenameset_t::iterator pname = m_gamefiles.begin(); pname != m_gamefiles.end(); pname++) {
      const char *name = pname->c_str();

      FileSystem* fs = 0;

      try {
         fs = g_fs->MakeSubFileSystem( name );
			Game_Loader gl(*fs, game);
			gl.preload_game(&gpdp);

	 char* fname = strdup(FileSystem::FS_Filename(name));
	 FileSystem::FS_StripExtension(fname);
			list.add(fname, name);
         free(fname);

      } catch(_wexception& ) {
         // we simply skip illegal entries
      }
      if( fs )
         delete fs;
   }

	if (list.size()) list.select(0);
}
