/*
 * Copyright (C) 2002-2004, 2006-2010 by the Widelands Development Team
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

#include "event_factory.h"

#include "event_message.h"
#include "event_expire_message.h"
#include "profile/profile.h"

#include "wexception.h"

#include "i18n.h"

namespace Widelands {

namespace Event_Factory {

///\todo This information belongs into the event classes
Type_Descr EVENT_TYPE_DESCRIPTIONS[] = {
	{
		false,
		"immovable",              _("Create immovable"),
		_("Create an immovable.")
	},
	{
		true,
		"message_box",            _("Show message box"),
		_
			("Show a message box. It can be modal or and have a picture. An "
			 "events can be assigned to each button to give the user chose the "
			 "course of the game.")
	},
	{
		false,
		"message",                   _("Creates a message for a player"),
		_
			("Creates a message for a player. This message can last forever, "
			 "during a specified duration, or until a special event is run.")
	},
	{
		true,
		"expire_message",            _("Expires a previously created message"),
		_
			("Expires a message that was previously created for a player. The "
			 "message will cease to exist.")
	}
};


Event & create(size_t const id) {
	return
		create
			(id, i18n::translate(EVENT_TYPE_DESCRIPTIONS[id].name), Event::INIT);
}


Event & create
	(size_t const id, char const * const name, Event::State const state)
{
	switch (id) {
	case 23: return *new Event_Message                  (name, state);
	case 24: return *new Event_Expire_Message           (name, state);
	default:
		assert(false);
	}
}


Event & create(Section & s, Editor_Game_Base & egbase) {
	char const * type_name = s.get_safe_string("type");

	size_t i = 0;
	while (strcmp(type_name, EVENT_TYPE_DESCRIPTIONS[i].id))
		if (++i == nr_event_types())
			throw wexception("invalid type \"%s\"", type_name);
	switch (i) {
	case 23: return *new Event_Message                  (s, egbase);
	case 24: return *new Event_Expire_Message           (s, egbase);
	default:
		assert(false);
	}
}


Type_Descr const & type_descr(size_t const id) {
	assert(id < nr_event_types());
	return EVENT_TYPE_DESCRIPTIONS[id];
}


size_t nr_event_types() {
	return sizeof(EVENT_TYPE_DESCRIPTIONS) / sizeof(*EVENT_TYPE_DESCRIPTIONS);
}

}

}
