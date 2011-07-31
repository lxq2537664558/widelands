/*
* Copyright (C) 2004-2011 by the Widelands Development Team
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

#include "ggz_ggzmod.h"
#include "network_ggz.h"
#include "log.h"
#include "gamesettings.h"
#include "wexception.h"

#include <stdint.h>
#include <string>
#include <vector>

#include <limits>

#ifdef WIN32
#include <winsock2.h>
#include <io.h>
#endif
#include <warning.h>
#include "network_ggz.h"

#include <scripting/pdep/llimits.h>


ggz_ggzmod * ggzmodobj = NULL;

ggz_ggzmod::ggz_ggzmod():
	m_data_fd(-1),
	m_server_fd(-1),
	m_connected(false),
	m_mod(NULL),
	m_seat(true),
	m_seat_desired(false),
	m_pending_seat_change(false),
	m_error_exit(false)
{
	if (ggzmodobj)
		throw wexception("created instance of ggz_ggzmod but ggzmodobj exists");
	ggzmodobj = this;
}

ggz_ggzmod::~ggz_ggzmod()
{
	disconnect();
	ggzmodobj = 0;
}

/**
 * Connect to GGZCORE. This does not connect to the metaserver but to our
 * ggzcore instance.
 */
bool ggz_ggzmod::connect()
{
	if (m_connected)
		return false;
	m_error_exit = false;

	log("GGZMOD ## connect to GGZCORE\n");
	if (m_mod) {
		ggzmod_free(m_mod);
	}
	m_mod = ggzmod_new(GGZMOD_GAME);

	// Set handler for ggzmod events:
	ggzmod_set_handler(m_mod, GGZMOD_EVENT_SERVER, &ggz_ggzmod::eventServer);
	ggzmod_set_handler(m_mod, GGZMOD_EVENT_ERROR, &ggz_ggzmod::eventServer);
	ggzmod_set_handler(m_mod, GGZMOD_EVENT_STATE, &ggz_ggzmod::eventServer);
	ggzmod_set_handler(m_mod, GGZMOD_EVENT_PLAYER, &ggz_ggzmod::eventServer);
	ggzmod_set_handler(m_mod, GGZMOD_EVENT_SEAT, &ggz_ggzmod::eventServer);
	ggzmod_set_handler
		(m_mod, GGZMOD_EVENT_SPECTATOR_SEAT, &ggz_ggzmod::eventServer);
	//ggzmod_set_handler(m_mod, GGZMOD_EVENT_STATS, &ggz_ggzmod::eventServer);
	//ggzmod_set_handler(m_mod, GGZMOD_EVENT_INFO, &ggz_ggzmod::eventServer);
	//ggzmod_set_handler(m_mod, GGZMOD_EVENT_CHAT, &ggz_ggzmod::eventServer);
	// not handled / not used events of the GGZMOD Server:
	// * GGZMOD_EVENT_RANKINGS

	if (ggzmod_connect(m_mod)) {
		log("GGZMOD ## connection failed\n");
		return false;
	}

	m_connected = true;

	// This is the fd to the ggzcore of this process
	m_server_fd = ggzmod_get_fd(m_mod);

	log("GGZMOD ## server fd %i\n", m_server_fd);

	m_changed = true;
	m_seat = false;

	// we don not want a seat as long as the player has no position
	m_seat_desired = false;

	// we do not yet know which seat type we have.
	m_pending_seat_change = true;

	return true;
}

void ggz_ggzmod::eventServer(GGZMod * cbmod, GGZModEvent e, const void * cbdata)
{
	ggzmodobj->ggzmod_server(cbmod, e, cbdata);
}


/// handles the events of the ggzmod server
void ggz_ggzmod::ggzmod_server
	(GGZMod * const cbmod, GGZModEvent const e, void const * const cbdata)
{
	log("GGZMOD ## ggzmod_server\n");
	switch (e) {
		case GGZMOD_EVENT_SERVER:
		{
			int32_t const fd = *static_cast<int32_t const *>(cbdata);
			ggzmodobj->m_data_fd = fd;
			log("GGZMOD ## got data fd: %i\n", fd);
			ggzmod_set_state(cbmod, GGZMOD_STATE_PLAYING);
			int is_spectator, seatnum;
			const char * name = ggzmod_get_player(m_mod, &is_spectator, &seatnum);
			m_seat = not is_spectator;
			log
				("GGZMOD ## I am \"%s\" at seat %i and I am %s spectator\n",
				 name, seatnum, is_spectator?"a":"not a");
			break;
		}
		case GGZMOD_EVENT_ERROR:
		{
			const char * msg = static_cast<const char * >(cbdata);
			log("GGZMOD ## ERROR: %s\n", msg);
			break;
		}
		case GGZMOD_EVENT_CHAT:
		{
			// This should not happen. We do not use ggz chat inside table.
			const GGZChat * chat = static_cast<const GGZChat *>(cbdata);
			log
				("GGZMOD ## ERROR: Received EVENT_CHAT from %s: \"%s\"\n",
				 chat->player, chat->message);
			 break;
		}
		case GGZMOD_EVENT_INFO:
		{
			//const GGZPlayerInfo * pi = static_cast<const GGZPlayerInfo*>(cbdata);
			//log("GGZMOD ## received EVENT_INFO\n");
			break;
		}
		case GGZMOD_EVENT_PLAYER:
		{
			// seat changed status
			/* data is a int[2] pair consisting of the old
			 *  {is_spectator, seat_num}. */
			const int * data = static_cast<const int *>(cbdata);
			int is_spectator, seatnum;
			ggzmod_get_player(m_mod, &is_spectator, &seatnum);
			log
				("GGZMOD ## EVENT_PLAYER: old data: is_spectator: %i, seat_num: %i new: is_spectator: %i,"
				 "seat_num: %i\n",
				 data[0], data[1], is_spectator, seatnum);
			m_pending_seat_change = false;
			m_seat = not is_spectator;
			break;
		}
		case GGZMOD_EVENT_SEAT:
		{
			/* This event occurs when a seat change occurs.  The old seat
			*  (a GGZSeat*) is passed as the event's data.  The seat
			*  information will be updated before the event is invoked. */
			break;
		}
		case GGZMOD_EVENT_SPECTATOR_SEAT:
		{
			/* This event occurs when a spectator seat change occurs.  The
			*  old spectator (a GGZSpectator*) is passed as the event's data.
			*  The spectator information will be updated before the event is
			*  invoked. */
			break;
		}
		case GGZMOD_EVENT_STATE:
		{
			/* This event occurs when the game's status changes.  The old
			 *  state (a GGZModState*) is passed as the event's data.
			 *  @see GGZModState */
			const GGZModState oldstate = * static_cast<const GGZModState * >(cbdata);
			log
				("GGZMOD ## state change from %i to %i\n",
				 oldstate, ggzmod_get_state(m_mod));
			break;
		}
		case GGZMOD_EVENT_STATS:
		{
			//log("GGZMOD ## A players' stats have been updated.\n");
			break;
		}
		default:
			log("GGZMOD ## HANDLE ERROR: %i\n", e);
	}
}

int32_t ggz_ggzmod::datafd()
{
	//if (m_data_fd < 0)
		//wexception("GGZMOD ## Tried to get datafd but it is not valid\n");
	return m_data_fd;
}

void ggz_ggzmod::disconnect(bool err)
{
	if (not m_mod)
		return;

	ggzmod_disconnect(m_mod);

	if (m_data_fd > 0)
		close(m_data_fd);
	m_data_fd = -1;

	if (m_mod)

	ggzmod_free(m_mod);


	m_mod = 0;
	m_server_fd = 0;
	m_connected = false;
}

void ggz_ggzmod::process()
{
	if (not m_connected or m_error_exit)
		return;
	if (m_changed)
	{
		changeSeat();
		if (not m_pending_seat_change and m_seat == m_seat_desired) {
			m_changed = false;
		}
	}
	ggzmod_dispatch(m_mod);
}

bool ggz_ggzmod::data_pending()
{
	fd_set read_fd_set;
	int result;
	struct timeval tv;

	if (not m_connected)
		return false;

	if (m_server_fd < 0 or m_server_fd > FD_SETSIZE)
		return false;

	FD_ZERO(&read_fd_set);
	if (m_server_fd > 0 and m_server_fd <= FD_SETSIZE)
		FD_SET(m_server_fd, &read_fd_set);
		tv.tv_sec = tv.tv_usec = 0;
	result = select(FD_SETSIZE, &read_fd_set, NULL, NULL, &tv);
	if (result > 0)
		return true;
	return false;
}

void ggz_ggzmod::set_player()
{
	log("GGZMOD ## switch to player\n");
	m_seat_desired = true;
	m_changed = true;
}

void ggz_ggzmod::set_spectator()
{
	log("GGZMOD ## switch to spectator\n");
	m_seat_desired = false;
	m_changed = true;
}

std::string ggz_ggzmod::playername()
{
	std::string name;
	const char * plrname;
	int spectator, seat_num;
	plrname = ggzmod_get_player(m_mod, &spectator, &seat_num);
	if (plrname) {
		name = plrname;
		ggz_free(plrname);
		return name;
	}
	else
		return "";
}

void ggz_ggzmod::changeSeat()
{
	log("GGZMOD: changeSeat()\n");
	if (m_seat != m_seat_desired and not m_pending_seat_change) {
		log("do a change\n");
		if (m_seat_desired) {
			for (int i = 0; i < ggzmod_get_num_seats(m_mod); ++i)
			{
				GGZSeat seat = ggzmod_get_seat(m_mod, i);
				if (seat.type == GGZ_SEAT_OPEN) {
					log("GGZMOD ## found open seat. request sit\n");
					ggzmod_request_sit(m_mod, seat.num);
					break;
				}
			}
			m_pending_seat_change = true;
		} else {
			ggzmod_request_stand(m_mod);
			log("GGZMOD: request stand\n");
			m_pending_seat_change = true;
		}
	}
}


