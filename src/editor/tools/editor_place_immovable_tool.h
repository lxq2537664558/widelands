/*
 * Copyright (C) 2002-2004, 2006-2008 by the Widelands Development Team
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

#ifndef WL_EDITOR_TOOLS_EDITOR_PLACE_IMMOVABLE_TOOL_H
#define WL_EDITOR_TOOLS_EDITOR_PLACE_IMMOVABLE_TOOL_H

#include "editor/tools/editor_delete_immovable_tool.h"
#include "editor/tools/multi_select.h"

/**
 * This places immovables on the map
*/
struct EditorPlaceImmovableTool : public EditorTool, public MultiSelect {
	EditorPlaceImmovableTool(EditorDeleteImmovableTool & tool)
		: EditorTool(tool, tool)
	{}

	int32_t handle_click_impl(Widelands::Map& map,
	                          const Widelands::World& world,
	                          Widelands::NodeAndTriangle<> center,
	                          EditorInteractive& parent,
	                          EditorActionArgs& args) override;

	int32_t handle_undo_impl(Widelands::Map& map,
	                         const Widelands::World& world,
	                         Widelands::NodeAndTriangle<> center,
	                         EditorInteractive& parent,
	                         EditorActionArgs& args) override;

	EditorActionArgs format_args_impl(EditorInteractive & parent) override;

	ImageCatalog::Keys get_sel_impl() const override {
		return ImageCatalog::Keys::kEditorToolImmovable;
	}
};

#endif  // end of include guard: WL_EDITOR_TOOLS_EDITOR_PLACE_IMMOVABLE_TOOL_H
