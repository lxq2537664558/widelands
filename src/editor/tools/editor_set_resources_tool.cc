/*
 * Copyright (C) 2002-2004, 2006-2008, 2010, 2012 by the Widelands Development Team
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

#include "editor/tools/editor_set_resources_tool.h"

#include "editor/editorinteractive.h"
#include "editor/tools/editor_decrease_resources_tool.h"
#include "editor/tools/editor_increase_resources_tool.h"
#include "graphic/graphic.h"
#include "logic/field.h"
#include "logic/map_objects/world/resource_description.h"
#include "logic/map_objects/world/world.h"
#include "logic/mapregion.h"

int32_t EditorSetResourcesTool::handle_click_impl(const Widelands::World& world,
                                                  Widelands::NodeAndTriangle<> const center,
                                                  EditorInteractive& /* parent */,
                                                  EditorActionArgs* args,
                                                  Widelands::Map* map) {
	Widelands::MapRegion<Widelands::Area<Widelands::FCoords> > mr
	(*map,
	 Widelands::Area<Widelands::FCoords>
	 (map->get_fcoords(center.node), args->sel_radius));
	do {
		Widelands::ResourceAmount amount     = args->set_to_resource;
		Widelands::ResourceAmount max_amount = args->cur_res != Widelands::kNoResource ?
							 world.get_resource(args->cur_res)->max_amount() : 0;

		if (amount > max_amount)
			amount = max_amount;

		if (map->is_resource_valid(world, mr.location(), args->cur_res)) {
			args->org_res_t.push_back(mr.location().field->get_resources());
			args->org_res.push_back(mr.location().field->get_resources_amount());
			map->initialize_resources(mr.location(), args->cur_res, amount);
		}
	} while (mr.advance(*map));
	return mr.radius();
}

int32_t
EditorSetResourcesTool::handle_undo_impl(const Widelands::World& world,
                                         Widelands::NodeAndTriangle<Widelands::Coords> center,
                                         EditorInteractive& /* parent */,
                                         EditorActionArgs* args,
                                         Widelands::Map* map) {
	Widelands::MapRegion<Widelands::Area<Widelands::FCoords> > mr
	(*map,
	 Widelands::Area<Widelands::FCoords>
	 (map->get_fcoords(center.node), args->sel_radius));
	std::list<Widelands::ResourceAmount>::iterator amount_it = args->org_res.begin();
	std::list<Widelands::DescriptionIndex>::iterator type_it = args->org_res_t.begin();
	do {
		Widelands::ResourceAmount amount     = *amount_it;
		Widelands::ResourceAmount max_amount = world.get_resource(args->cur_res)->max_amount();

		if (amount > max_amount)
			amount = max_amount;

		map->initialize_resources(mr.location(), *type_it, amount);
		++amount_it;
		++type_it;
	} while (mr.advance(*map));
	args->org_res.clear();
	args->org_res_t.clear();
	return mr.radius();
}

EditorActionArgs EditorSetResourcesTool::format_args_impl(EditorInteractive & parent)
{
	EditorActionArgs a(parent);
	a.cur_res = m_cur_res;
	a.set_to_resource = m_set_to;
	return a;
}
