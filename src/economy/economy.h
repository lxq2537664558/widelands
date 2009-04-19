/*
 * Copyright (C) 2004, 2006-2009 by the Widelands Development Team
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

#ifndef S__ECONOMY_H
#define S__ECONOMY_H

#include <vector>

#include "widelands_filewrite.h"
#include "widelands_fileread.h"

#include "supply_list.h"
#include "ui_unique_window.h"
#include "warelist.h"
#include "widelands.h"

/**
 * Economy represents a network of Flag through which wares can be transported.
 */
namespace Widelands {
struct Player;
struct Game;
struct Flag;
struct Route;
struct RSPairStruct;
struct Cmd_SetTargetQuantity;
struct Cmd_ResetTargetQuantity;
struct Warehouse;
struct Request;
struct Supply;
struct Map_Map_Object_Saver;
struct Map_Map_Object_Loader;

struct Economy {
	friend struct Request;
	friend struct Cmd_SetTargetQuantity;
	friend struct Cmd_ResetTargetQuantity;

	/// Configurable target quantity for the supply of a ware type in the
	/// economy.
	///
	/// This affects the result of \ref needs_ware and thereby the demand checks
	/// in production programs. A ware type is considered to be needed if there
	/// are less than the temporary target quantity stored in warehouses in the
	/// economy.
	///
	/// The temporary quantity will be decreased when a ware of the type is
	/// consumed, but not below the permanent quantity. This allows the
	/// following use case:
	///   The player has built up a lot of economic infrastructure but lacks
	///   mines. Then he discovers a mountain. He plans to expand to the
	///   mountain and build some mines. Therefore he anticipates that he will
	///   soon need 10 picks. Suppose that he has the target quantity (both
	///   permanent and temporary) for pick set to 2. He sets the temporary
	///   target quantity for pick to 12.  For each pick that is consumed in the
	///   economy, the target quantity for pick drops back 1 step, until it
	///   reaches the permanent level of 2. So when the player has built his
	///   mines and used his 10 picks, the targe quantity setting for pick is
	///   what it was before he changed it. That is the rationale for the name
	///   "temporary".
	///
	/// The last_modified time is used to determine which setting to use when
	/// economies are merged. The setting that was modified most recently will
	/// be used for the merged economy.
	struct Target_Quantity {
		uint32_t permanent, temporary;
		Time     last_modified;
	};

	Economy(Player &);
	~Economy();

	Player & owner() const throw () {return m_owner;}

	static void check_merge(Flag &, Flag &);
	static void check_split(Flag &, Flag &);

	bool find_route
		(Flag & start, Flag & end,
		 Route * route,
		 bool    wait,
		 int32_t cost_cutoff = -1);

	std::vector<Flag *>::size_type get_nrflags() const {return m_flags.size();}
	void    add_flag(Flag &);
	void remove_flag(Flag &);
	Flag & get_arbitrary_flag();

	void    add_wares  (Ware_Index, uint32_t count = 1);
	void remove_wares  (Ware_Index, uint32_t count = 1);

	void    add_workers(Ware_Index, uint32_t count = 1);
	void remove_workers(Ware_Index, uint32_t count = 1);

	void add_warehouse(Warehouse *wh);
	void remove_warehouse(Warehouse *wh);
	uint32_t get_nr_warehouses() const {return m_warehouses.size();}

	void    add_request(Request &);
	bool   have_request(Request &);
	void remove_request(Request &);

	void    add_supply(Supply &);
	void remove_supply(Supply &);

	/// information about this economy
	WareList::count_type stock_ware  (Ware_Index const i) {
		return m_wares  .stock(i);
	}
	WareList::count_type stock_worker(Ware_Index const i) {
		return m_workers.stock(i);
	}

	/// Whether the economy needs more of this ware type.
	/// Productionsites may ask this before they produce, to avoid depleting a
	/// ware type by overproducing another from it.
	bool needs_ware(Ware_Index) const;

	Target_Quantity const & target_quantity(Ware_Index const i) const {
		return m_target_quantities[i.value()];
	}
	Target_Quantity       & target_quantity(Ware_Index const i)       {
		return m_target_quantities[i.value()];
	}

	void show_options_window();
	UI::UniqueWindow::Registry m_optionswindow_registry;


	WareList const & get_wares  () {return m_wares;}
	WareList const & get_workers() {return m_workers;}

	void balance_requestsupply(uint32_t timerid); ///< called by \ref Cmd_Call_Economy_Balance

	void rebalance_supply() {start_request_timer();}

	void Read (FileRead  &, Game &, Map_Map_Object_Loader *);
	void Write(FileWrite &, Game &, Map_Map_Object_Saver  *);

private:
	void do_remove_flag(Flag &);

	void do_merge(Economy &);
	void do_split(Flag &);

	void start_request_timer(int32_t delta = 200);

	Supply * find_best_supply(Game &, Request const &, int32_t & cost);
	void process_requests(Game &, RSPairStruct &);
	void create_requested_workers(Game &);

	typedef std::vector<Request *> RequestList;

	Player & m_owner;

	/// True while rebuilding Economies (i.e. during split/merge)
	bool m_rebuilding;

	typedef std::vector<Flag *> Flags;
	Flags m_flags;
	WareList m_wares;     ///< virtual storage with all wares in this Economy
	WareList m_workers;   ///< virtual storage with all workers in this Economy
	std::vector<Warehouse *> m_warehouses;

	RequestList m_requests; ///< requests
	SupplyList m_supplies;

	Target_Quantity        * m_target_quantities;

	/**
	 * ID for the next request balancing timer. Used to throttle
	 * excessive calls to the request/supply balancing logic.
	 */
	uint32_t m_request_timerid;

	uint32_t mpf_cycle;       ///< pathfinding cycle, see Flag::mpf_cycle
};

}
#endif 


