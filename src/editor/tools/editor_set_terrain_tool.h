/*
 * Copyright (C) 2002-2004, 2006-2008, 2012 by the Widelands Development Team
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

#ifndef WL_EDITOR_TOOLS_EDITOR_SET_TERRAIN_TOOL_H
#define WL_EDITOR_TOOLS_EDITOR_SET_TERRAIN_TOOL_H

#include "editor/tools/editor_tool.h"
#include "editor/tools/multi_select.h"

struct Editor_Set_Terrain_Tool : public Editor_Tool, public MultiSelect {
	Editor_Set_Terrain_Tool() : Editor_Tool(*this, *this) {}

	int32_t handle_click_impl(Widelands::Map& map,
	                          const Widelands::World& world,
	                          Widelands::Node_and_Triangle<> center,
	                          Editor_Interactive& parent,
	                          Editor_Action_Args& args) override;

	int32_t handle_undo_impl(Widelands::Map& map,
	                         const Widelands::World& world,
	                         Widelands::Node_and_Triangle<> center,
	                         Editor_Interactive& parent,
	                         Editor_Action_Args& args) override;

	Editor_Action_Args format_args_impl(Editor_Interactive & parent) override;

	char const * get_sel_impl() const override {return "pics/fsel.png";}
	bool operates_on_triangles() const override {return true;}
};

#endif  // end of include guard: WL_EDITOR_TOOLS_EDITOR_SET_TERRAIN_TOOL_H
