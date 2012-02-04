/*
 * Copyright (C) 2004-2006, 2008-2009, 2012 by the Widelands Development Team
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

#ifndef INTERNET_GAMING_H
#define INTERNET_GAMING_H

#include "build_info.h"
#include "chat.h"
#include "internet_gaming_protocol.h"
#include "network_lan_promotion.h"

#include <stdint.h>
#include <string>
#include <vector>

#ifdef WIN32
#include <winsock2.h>
#include <io.h>
#endif


/// A simply network player struct
struct Net_Client {
	std::string   game;
	std::string   name;
	std::string   build_id;
	uint32_t      type;
	char          stats[16];
};


/**
 * The InternetGaming struct.
 */
struct InternetGaming : public ChatProvider {
	/// The only instance of InternetGaming -> the constructor is private by purpose!
	static InternetGaming & ref();

	// Login and logout
	bool login
		(std::string const & nick, std::string const & pwd, bool registered,
		 std::string const & metaserver, uint32_t port);
	void logout();

	/// \returns whether the client is logged in
	bool logged_in() {return m_state != OFFLINE;}

	void handle_metaserver_communication();

	// Game specific functions
	char const * ip();
	void join_game(std::string const & gamename);
	void open_game();
	void set_game_playing();
	void set_game_done();


	// Informative functions for lobby
	bool updateForGames();
	std::vector<Net_Game_Info> const & games();
	bool updateForClients();
	std::vector<Net_Client>    const & clients();

	/// \returns the maximum allowed number of clients in a game (players + spectators)
	uint32_t max_clients() {return INTERNET_GAMING_MAX_CLIENTS_PER_GAME;}

	/// sets the maximum number of players that may be in the game
	void set_local_maxplayers(uint32_t mp) {m_maxclients = mp;}

	/// sets the name of the local server as shown in the games list
	void set_local_servername(std::string const & name) {m_gamename = name;}


	/// \returns the name of the local server
	std::string & get_local_servername() {return m_gamename;}

	/// \returns the name of the local client
	std::string & get_local_clientname() {return m_clientname;}


	// ChatProvider: sends a message via the metaserver.
	void send(std::string const &);

	/// ChatProvider: adds the message to the message list and calls parent.
	void receive(ChatMessage const & msg) {messages.push_back(msg); ChatProvider::send(msg);}

	/// ChatProvider: returns the list of chatmessages.
	std::vector<ChatMessage> const & getMessages() const {return messages;}

private:
	InternetGaming();

	/// Current state of this class
	enum {
		OFFLINE,
		LOBBY,
		IN_GAME
	} m_state;

	// client informations
	std::string m_clientname;

	// informations of clients game
	std::string m_gamename;
	uint32_t    m_maxclients;

	// Server informations
	bool clientupdate;
	bool tableupdate;
	std::vector<Net_Game_Info> gamelist;
	std::vector<Net_Client>    clientlist;

	/// ChatProvider: chat messages
	std::vector<ChatMessage> messages;

};

#endif
