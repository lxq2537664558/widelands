/*
 * Copyright (C) 2002-2004, 2004 by the Widelands Development Team
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

#ifndef __S__EDITOR_DECREASE_RESOURCES_TOOL_H
#define __S__EDITOR_DECREASE_RESOURCES_TOOL_H

#include "editor_tool.h"

/*
=============================
class Editor_Decrease_Resources_Tool

this decreases the resources of a field by a value
=============================
*/
class Editor_Decrease_Resources_Tool : public Editor_Tool {
   public:
      Editor_Decrease_Resources_Tool() : Editor_Tool(this,this) { m_changed_by=1; m_cur_res=0; }
      virtual ~Editor_Decrease_Resources_Tool() { }

      virtual int handle_click_impl(FCoords&, Map*, Editor_Interactive*);

      virtual const char* get_fsel_impl(void) { return "pics/fsel_editor_decrease_resources.png"; }

      inline int get_changed_by(void) { return m_changed_by; }
      inline void set_changed_by(int n) { m_changed_by=n; }
      inline int get_cur_res(void) { return m_cur_res; }
      inline void set_cur_res(int res) { m_cur_res=res; }

   private:
      int m_cur_res;
      int m_changed_by;
};

#endif
