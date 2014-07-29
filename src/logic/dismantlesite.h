/*
 * Copyright (C) 2002-2004, 2006-2009, 2011 by the Widelands Development Team
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

#ifndef WL_LOGIC_DISMANTLESITE_H
#define WL_LOGIC_DISMANTLESITE_H

#include "base/macros.h"
#include "logic/building.h"
#include "logic/partially_finished_building.h"
#include "logic/player.h"

namespace Widelands {

class Building;

/*
DismantleSite
-------------
The dismantlesite is a way to get some resources back when destroying buildings.
It has a builder as worker and will show the idle image of the original
building which gets smaller and smaller.

Every tribe has exactly one DismantleSite_Descr.
The DismantleSite_Descr's idling animation is remaining graphic that is shown under
the destructed building.
*/
class DismantleSite;

struct DismantleSiteDescr : public BuildingDescr {
	DismantleSiteDescr(char const* name,
	                    char const* descname,
	                    const std::string& directory,
	                    Profile&,
	                    Section& global_s,
							  const Tribe_Descr& tribe);
	~DismantleSiteDescr() override {}

	Building& create_object() const override;

private:
	DISALLOW_COPY_AND_ASSIGN(DismantleSiteDescr);
};

class DismantleSite : public Partially_Finished_Building {
	friend class Map_Buildingdata_Data_Packet;

	static const uint32_t DISMANTLESITE_STEP_TIME = 45000;

	MO_DESCR(DismantleSiteDescr)

public:
	DismantleSite(const DismantleSiteDescr & descr);
	DismantleSite
		(const DismantleSiteDescr & descr, Editor_Game_Base &,
		 Coords const, Player &, bool, Building::FormerBuildings & former_buildings);

	std::string get_statistics_string() override;

	bool burn_on_destroy() override;
	void init   (Editor_Game_Base &) override;

	bool get_building_work(Game &, Worker &, bool success) override;

	static void count_returned_wares(Building* building, std::map<Ware_Index, uint8_t> & res);

protected:
	uint32_t build_step_time() const override {return DISMANTLESITE_STEP_TIME;}

	virtual void create_options_window
		(Interactive_GameBase &, UI::Window * & registry) override;

	void draw(const Editor_Game_Base &, RenderTarget &, const FCoords&, const Point&) override;
};

}

#endif  // end of include guard: WL_LOGIC_DISMANTLESITE_H
