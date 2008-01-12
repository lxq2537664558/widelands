/*
 * Copyright (C) 2002-2004, 2007-2008 by the Widelands Development Team
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

#ifndef __S__EVENT_FACTORY_H
#define __S__EVENT_FACTORY_H

#include <string>

struct Editor_Interactive;

namespace Widelands {

class Event;

struct Event_Descr {
	std::string id;
	const std::string name;
	const std::string descr;
};

/// Returns the correct descriptions, ids and creates the correct option dialog
/// and (of course) event for each event-id.
namespace Event_Factory {
Event * get_correct_event(const char* id);
Event * make_event_with_option_dialog
	(const char * id, Editor_Interactive &, Event *);
Event_Descr * get_event_descr(uint32_t id);
const uint32_t get_nr_of_available_events();
};

};

#endif
