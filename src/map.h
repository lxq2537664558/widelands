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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef __S__MAP_H
#define __S__MAP_H

#include "events/event_chain.h"
#include "field.h"
#include "widelands_geometry.h"
#include "objective.h"
#include "variable.h"
#include "world.h"

#include "interval.h"
#include "manager.h"

#include <set>
#include <string>
#include <vector>

struct Overlay_Manager;
struct S2_Map_Loader;

namespace Widelands {

class BaseImmovable;
class Player;
class World;
class Map;
class Map_Loader;
#define WLMF_SUFFIX ".wmf"
#define S2MF_SUFFIX     ".swd"

#define S2MF_MAGIC  "WORLD_V1.0"


const uint16_t NUMBER_OF_MAP_DIMENSIONS=29;
const uint16_t MAP_DIMENSIONS[] = {
   64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256,
   272, 288, 304, 320, 336, 352, 368, 384, 400, 416, 432, 448, 464, 480,
   496, 512};


class Path;
class Immovable;


struct ImmovableFound {
	BaseImmovable * object;
	Coords          coords;
};

/*
FindImmovable
FindBob
FindNode
FindResource
CheckStep

Predicates used in path finding and find functions.
*/
struct FindImmovable {
	// Return true if this immovable should be returned by find_immovables()
	virtual bool accept(BaseImmovable *imm) const = 0;
   virtual ~FindImmovable() {}  // make gcc shut up
};
struct FindBob {
	// Return true if this immovable should be returned by find_bobs()
	virtual bool accept(Bob *imm) const = 0;
   virtual ~FindBob() {}  // make gcc shut up
};
struct FindNode {
	// Return true if this immovable should be returned by find_fields()
	virtual bool accept(const Map &, const FCoords coord) const = 0;
   virtual ~FindNode() {}  // make gcc shut up
};
struct CheckStep {
	enum StepId {
		stepNormal, //  normal step treatment
		stepFirst,  //  first step of a path
		stepLast,   //  last step of a path
	};

	// Return true if moving from start to end (single step in the given
	// direction) is allowed.
	virtual bool allowed(Map* map, FCoords start, FCoords end, int32_t dir, StepId id) const = 0;

	// Return true if the destination field can be reached at all
	// (e.g. return false for land-based bobs when dest is in water).
	virtual bool reachabledest(Map* map, FCoords dest) const = 0;
   virtual ~CheckStep() {}  // make gcc shut up
};


/*
Some very simple default predicates (more predicates below Map).
*/
struct FindImmovableAlwaysTrue : public FindImmovable {
	virtual bool accept(BaseImmovable *) const {return true;}
   virtual ~FindImmovableAlwaysTrue() {}  // make gcc shut up
};
struct FindBobAlwaysTrue : public FindBob {
	virtual bool accept(Bob *) const {return true;}
   virtual ~FindBobAlwaysTrue() {}  // make gcc shut up
};
struct FindBobAttribute : public FindBob {
	FindBobAttribute(uint32_t attrib) : m_attrib(attrib) {}

	virtual bool accept(Bob *imm) const;

	int32_t m_attrib;
   virtual ~FindBobAttribute() {}  // make gcc shut up
};

typedef uint8_t Direction;

/** class Map
 *
 * This really identifies a map like it is in the game
 *
 * Odd rows are shifted FIELD_WIDTH/2 to the right. This means that moving
 * up and down depends on the row numbers:
 *               even   odd
 * top-left      -1/-1   0/-1
 * top-right      0/-1  +1/-1
 * bottom-left   -1/+1   0/+1
 * bottom-right   0/+1  +1/+1
 *
 * Warning: width and height must be even
 */
struct Map {
	friend class Editor_Game_Base;
   friend class Map_Loader;
	friend class ::S2_Map_Loader;
	friend struct WL_Map_Loader;
	friend struct Map_Elemental_Data_Packet;
	friend struct Map_Extradata_Data_Packet;
   friend class Editor;
   friend class Main_Menu_New_Map;

	enum { // flags for findpath()

		//  use bidirection cost instead of normal cost calculations
		//  should be used for road building
		fpBidiCost = 1,

	};

	struct Pathfield;

   Map();
   ~Map();

   // For overlays
   Overlay_Manager* get_overlay_manager() {return m_overlay_manager;}
	Overlay_Manager & get_overlay_manager() const {return *m_overlay_manager;}
	const Overlay_Manager & overlay_manager() const {return *m_overlay_manager;}
	Overlay_Manager       & overlay_manager()       {return *m_overlay_manager;}

   // For loading
   Map_Loader* get_correct_loader(const char*);
   void cleanup();

   // for editor
	void create_empty_map
		(const uint32_t w = 64,
		 const uint32_t h = 64,
		 const std::string worldname = std::string("greenland"));

   void load_graphics();
   void recalc_whole_map();
   void recalc_for_field_area(const Area<FCoords>);
   void recalc_default_resources();

	void set_nrplayers(Player_Number const nrplayers);

	void set_starting_pos(const uint32_t plnum, const Coords);
	Coords get_starting_pos(const uint32_t plnum) const
	{return m_starting_pos[plnum - 1];}

	void set_filename(const char *string);
	void set_author(const char *string);
	void set_world_name(const char *string);
	void set_name(const char *string);
	void set_description(const char *string);
	void set_background(const char *string);

	// informational functions
	const char * get_filename() const {return m_filename;}
	const char * get_author() const {return m_author;}
	const char * get_name() const {return m_name;}
	const char * get_description() const {return m_description;}
	const char * get_world_name() const {return m_worldname;}
	const std::string & get_background() const {return m_background;}
	Player_Number get_nrplayers() const throw () {return m_nrplayers;}
	Extent extent() const throw () {return Extent(m_width, m_height);}
	X_Coordinate get_width   () const throw () {return m_width;}
	Y_Coordinate get_height  () const throw () {return m_height;}
	World & world() const throw () {return *m_world;}
	World * get_world() const {return m_world;}
   // The next few functions are only valid
   // when the map is loaded as an scenario.
	const std::string & get_scenario_player_tribe(Player_Number) const;
	const std::string & get_scenario_player_name (Player_Number) const;
	void set_scenario_player_tribe(Player_Number, const std::string &);
	void set_scenario_player_name (Player_Number, const std::string &);

	BaseImmovable * get_immovable(const Coords) const;
	uint32_t find_bobs
		(const Area<FCoords>,
		 std::vector<Bob *> * list,
		 const FindBob & functor = FindBobAlwaysTrue());
	uint32_t find_reachable_bobs
		(const Area<FCoords>,
		 std::vector<Bob *> * list,
		 const CheckStep &,
		 const FindBob & functor = FindBobAlwaysTrue());
	uint32_t find_immovables
		(const Area<FCoords>,
		 std::vector<ImmovableFound> * list,
		 const FindImmovable & = FindImmovableAlwaysTrue());
	uint32_t find_reachable_immovables
		(const Area<FCoords>,
		 std::vector<ImmovableFound> * list,
		 const CheckStep &,
		 const FindImmovable & = FindImmovableAlwaysTrue());
	uint32_t find_fields
		(const Area<FCoords>,
		 std::vector<Coords> * list,
		 const FindNode & functor);
	uint32_t find_reachable_fields
		(const Area<FCoords>,
		 std::vector<Coords>* list,
		 const CheckStep &,
		 const FindNode &);

	// Field logic
	static Map_Index get_index(Coords const c, X_Coordinate const width);
	Map_Index max_index() const {return m_width * m_height;}
	Field & operator[](Map_Index const)  const;
	Field & operator[](Coords) const;
	Field * get_field(Coords) const __attribute__((deprecated));
	FCoords get_fcoords(Coords) const;
	void normalize_coords(Coords *) const;
	FCoords get_fcoords(Field &) const;
	void get_coords(Field & f, Coords & c) const;

	uint32_t calc_distance(const Coords, const Coords) const;
	int32_t is_neighbour(const Coords, const Coords) const;

	int32_t calc_cost_estimate(const Coords, const Coords) const;
	int32_t calc_cost_lowerbound(const Coords, const Coords) const;
	int32_t calc_cost(const int32_t slope) const;
	int32_t calc_cost(const Coords, const int32_t dir) const;
	int32_t calc_bidi_cost(const Coords, const int32_t dir) const;
	void calc_cost(const Path &, int32_t * forward, int32_t * backward) const;

	void get_ln (const  Coords,  Coords * const) const;
	void get_ln (const FCoords, FCoords * const) const;
	Coords  l_n (const  Coords) const;
	FCoords l_n (const FCoords) const;
	void get_rn (const  Coords,  Coords * const) const;
	void get_rn (const FCoords, FCoords * const) const;
	Coords  r_n (const  Coords) const;
	FCoords r_n (const FCoords) const;
	void get_tln(const  Coords,  Coords * const) const;
	void get_tln(const FCoords, FCoords * const) const;
	Coords  tl_n(const  Coords) const;
	FCoords tl_n(const FCoords) const;
	void get_trn(const  Coords,  Coords * const) const;
	void get_trn(const FCoords, FCoords * const) const;
	Coords  tr_n(const  Coords) const;
	FCoords tr_n(const FCoords) const;
	void get_bln(const  Coords,  Coords * const) const;
	void get_bln(const FCoords, FCoords * const) const;
	Coords  bl_n(const  Coords) const;
	FCoords bl_n(const FCoords) const;
	void get_brn(const  Coords,  Coords * const) const;
	void get_brn(const FCoords, FCoords * const) const;
	Coords  br_n(const  Coords) const;
	FCoords br_n(const FCoords) const;

	void get_neighbour(const Coords, const Direction dir, Coords * const) const;
	void get_neighbour
		(const FCoords, const Direction dir, FCoords * const) const;
	FCoords get_neighbour(const FCoords f, const Direction dir) const throw ();

	// Pathfinding
	int32_t findpath
		(Coords instart,
		 Coords inend,
		 const int32_t persist,
		 Path &,
		 const CheckStep &,
		 const uint32_t flags = 0);

	/**
	 * We can reach a field by water either if it has MOVECAPS_SWIM or if it has
	 * MOVECAPS_WALK and at least one of the neighbours has MOVECAPS_SWIM
	 */
	bool can_reach_by_water(const Coords) const;

	/**
	 * Sets the height to a value. Recalculates brightness. Changes the
	 * surrounding nodes if necessary. Returns the radius that covers all changes
	 * that were made.
	 *
	 * Do not call this to set the height of each node in an area to the same
	 * value, because it adjusts the heights of surrounding nodes in each call,
	 * so it will be terribly slow. Use set_height for Area for that purpouse
	 * instead.
	 */
	uint32_t set_height(const FCoords, Player_Number const new_value);

	/// Changes the height of the nodes in an Area by a difference.
	uint32_t change_height(Area<FCoords>, int16_t const difference);

	/**
	 * Ensures that the height of each node within radius from fc is in
	 * height_interval. If the height is < height_interval.min, it is changed to
	 * height_interval.min. If the height is > height_interval.max, it is changed
	 * to height_interval.max. Otherwise it is left unchanged.
	 *
	 * Recalculates brightness. Changes the surrounding nodes if necessary.
	 * Returns the radius of the area that covers all changes that were made.
	 *
	 * Calling this is much faster than calling change_height for each node in
	 * the area, because this adjusts the surrounding nodes only once, after all
	 * nodes in the area had their new height set.
	 */
	uint32_t set_height(Area<FCoords>, interval<Field::Height> height_interval);

	//  change terrain of a triangle, recalculate buildcaps
	int32_t change_terrain
		(const TCoords<FCoords>, const Terrain_Descr::Index terrain);

   /*
    * Get the a manager for registering or removing
    * something
    */
	Manager<Variable>   const & mvm() const {return m_mvm;}
	Manager<Variable>         & mvm()       {return m_mvm;}
	Manager<Trigger>    const & mtm() const {return m_mtm;}
	Manager<Trigger>          & mtm()       {return m_mtm;}
	Manager<Event>      const & mem() const {return m_mem;}
	Manager<Event>            & mem()       {return m_mem;}
	Manager<EventChain> const & mcm() const {return m_mcm;}
	Manager<EventChain>       & mcm()       {return m_mcm;}
	Manager<Objective>  const & mom() const {return m_mom;}
	Manager<Objective>        & mom()       {return m_mom;}

	/// Returns the military influence on a location from an area.
	Military_Influence calc_influence(const Coords a, const Area<>) const;

private:
	void set_size(const uint32_t w, const uint32_t h);
	void load_world();
	void recalc_border(const FCoords);

	uint16_t m_pathcycle;
	Player_Number m_nrplayers; // # of players this map supports (!= Game's number of players)
	X_Coordinate m_width;
	Y_Coordinate m_height;
	char        m_filename    [256];
	char        m_author       [61];
	char        m_name         [61];
	char        m_description[1024];
	char        m_worldname  [1024];
	std::string m_background;
	World     * m_world;           //  world type
	Coords    * m_starting_pos;    //  players' starting positions

	Field     * m_fields;

	Pathfield * m_pathfields;
   Overlay_Manager* m_overlay_manager;

   std::vector<std::string>  m_scenario_tribes; // only alloced when really needed
   std::vector<std::string>  m_scenario_names;

	Manager<Variable>   m_mvm;
	Manager<Trigger>    m_mtm;
	Manager<Event>      m_mem;
	Manager<EventChain> m_mcm;
	Manager<Objective>  m_mom;

	struct Extradata_Info {
		enum Type {
         PIC,
		};
      uint32_t    data;
      std::string filename;
      Type        type;
	};
	typedef std::vector<Extradata_Info> Extradata_Infos;
	Extradata_Infos m_extradatainfos; // Only for caching of extradata for writing and reading

	void recalc_brightness(FCoords);
	void recalc_fieldcaps_pass1(FCoords);
	void recalc_fieldcaps_pass2(FCoords);
	void check_neighbour_heights(FCoords, uint32_t & radius);
	void increase_pathcycle();

	template<typename functorT>
		void find_reachable
		(const Area<FCoords>, const CheckStep &, functorT &);

	template<typename functorT> void find(const Area<FCoords>, functorT &) const;

	Map & operator=(Map const &);
	explicit Map   (Map const &);
};

// FindImmovable functor
struct FindImmovableSize : public FindImmovable {
	FindImmovableSize(int32_t min, int32_t max) : m_min(min), m_max(max) {}
   virtual ~FindImmovableSize() {}  // make gcc shut up

	virtual bool accept(BaseImmovable *imm) const;

	int32_t m_min, m_max;
};
struct FindImmovableType : public FindImmovable {
	FindImmovableType(int32_t type) : m_type(type) {}
   virtual ~FindImmovableType() {}  // make gcc shut up

	virtual bool accept(BaseImmovable *imm) const;

	int32_t m_type;
};
struct FindImmovableAttribute : public FindImmovable {
	FindImmovableAttribute(uint32_t attrib) : m_attrib(attrib) {}
   virtual ~FindImmovableAttribute() {}  // make gcc shut up

	virtual bool accept(BaseImmovable *imm) const;

	int32_t m_attrib;
};
struct FindImmovablePlayerImmovable : public FindImmovable {
	FindImmovablePlayerImmovable() {}
   virtual ~FindImmovablePlayerImmovable() {}  // make gcc shut up

	virtual bool accept(BaseImmovable* imm) const;
};

struct FindNodeCaps : public FindNode {
	FindNodeCaps(uint8_t mincaps) : m_mincaps(mincaps) {}
   virtual ~FindNodeCaps() {}  // make gcc shut up

	virtual bool accept(const Map &, const FCoords coord) const;

	uint8_t m_mincaps;
};

// Accepts fields if they are accepted by all subfunctors.
struct FindNodeAnd : public FindNode {
	FindNodeAnd() {}
	virtual ~FindNodeAnd() {}

	void add(const FindNode* findfield, bool negate = false);

	virtual bool accept(const Map &, const FCoords coord) const;

	struct Subfunctor {
		bool              negate;
		const FindNode * findfield;
	};

	std::vector<Subfunctor> m_subfunctors;
};

// Accepts fields based on what can be built there
struct FindNodeSize : public FindNode {
	enum Size {
		sizeAny    = 0,   //  any field not occupied by a robust immovable
		sizeBuild,        //  any field we can build on (flag or building)
		sizeSmall,        //  at least small size
		sizeMedium,
		sizeBig,
		sizeMine,         //  can build a mine on this field
		sizePort,         //  can build a port on this field
	};

	FindNodeSize(Size size) : m_size(size) {}
   virtual ~FindNodeSize() {}  // make gcc shut up

	virtual bool accept(const Map &, const FCoords coord) const;

	Size m_size;
};

// Accepts a field for a certain size if it has
// a valid resource and amount on it
struct FindNodeSizeResource : public FindNodeSize {
   FindNodeSizeResource(Size size, int32_t res) : FindNodeSize(size) {m_res=res;}
   virtual ~FindNodeSizeResource() {}  // make gcc shut up

   virtual bool accept(const Map &, const FCoords coord) const;

   int32_t m_res;
};

// Accepts fields based on the size of immovables on the field
struct FindNodeImmovableSize : public FindNode {
	enum {
		sizeNone   = 1 << 0,
		sizeSmall  = 1 << 1,
		sizeMedium = 1 << 2,
		sizeBig    = 1 << 3
	};

	FindNodeImmovableSize(uint32_t sizes) : m_sizes(sizes) {}
   virtual ~FindNodeImmovableSize() {}  // make gcc shut up

	virtual bool accept(const Map &, const FCoords coord) const;

	uint32_t m_sizes;
};

// Accepts a field if it has an immovable with a given attribute
struct FindNodeImmovableAttribute : public FindNode {
	FindNodeImmovableAttribute(uint32_t attrib) : m_attribute(attrib) {}
   virtual ~FindNodeImmovableAttribute() {}  // make gcc shut up

	virtual bool accept(const Map &, const FCoords coord) const;

	uint32_t m_attribute;
};


// Accepts a field if it has the given resource
struct FindNodeResource : public FindNode {
	FindNodeResource(uint8_t res) : m_resource(res) {}
   virtual ~FindNodeResource() {}  // make gcc shut up

	virtual bool accept(const Map &, const FCoords coord) const;

	uint8_t m_resource;
};


/*
CheckStepDefault
----------------
Implements the default step checking behaviours that should be used for all
normal bobs.

Simply check whether the movecaps are matching (basic exceptions for water bobs
moving onto the shore).
*/
struct CheckStepDefault : public CheckStep {
	CheckStepDefault(uint8_t movecaps) : m_movecaps(movecaps) {}
	virtual ~CheckStepDefault() {} //  make gcc shut up

	virtual bool allowed(Map* map, FCoords start, FCoords end, int32_t dir, StepId id) const;
	virtual bool reachabledest(Map* map, FCoords dest) const;

private:
	uint8_t m_movecaps;
};


/*
CheckStepWalkOn
---------------
Implements the default step checking behaviours with one exception: we can move
from a walkable field onto an unwalkable one.
If onlyend is true, we can only do this on the final step.
*/
struct CheckStepWalkOn : public CheckStep {
	CheckStepWalkOn(uint8_t movecaps, bool onlyend) : m_movecaps(movecaps), m_onlyend(onlyend) {}
	virtual ~CheckStepWalkOn() {} //  make gcc shut up

	virtual bool allowed(Map* map, FCoords start, FCoords end, int32_t dir, StepId id) const;
	virtual bool reachabledest(Map* map, FCoords dest) const;

private:
	uint8_t m_movecaps;
	bool  m_onlyend;
};


/*
-------------
Implements the step checking behaviour for road building.

player is the player who is building the road.
movecaps are the capabilities with which the road is to be built (swimming
for boats, walking for normal roads).
*/
struct CheckStepRoad : public CheckStep {
	CheckStepRoad(Player const & player, uint8_t const movecaps)
		: m_player(player), m_movecaps(movecaps)
	{}
	virtual ~CheckStepRoad() {} //  make gcc shut up

	virtual bool allowed(Map* map, FCoords start, FCoords end, int32_t dir, StepId id) const;
	virtual bool reachabledest(Map* map, FCoords dest) const;

private:
	const Player                                     &       m_player;
	const uint8_t                                            m_movecaps;
};

/// A version of CheckStepRoad that is limited to a set of allowed locations.
struct CheckStepRoadLimited : public CheckStepRoad {
	CheckStepRoadLimited
		(Player const & player, uint8_t const movecaps,
		 std::set<Coords, Coords::ordering_functor> const & allowed_locations)
		: CheckStepRoad(player, movecaps), m_allowed_locations(allowed_locations)
	{}

	virtual bool allowed
		(Map *, FCoords start, FCoords end, int32_t dir, StepId id) const;

private:
	std::set<Coords, Coords::ordering_functor> const & m_allowed_locations;
};



/** class Path
 *
 * Represents a cross-country path found by Path::findpath, for example
 */
class CoordPath;

struct Path {
	friend class Map;

	Path() {}
	Path(Coords c) : m_start(c), m_end(c) {}
	Path(CoordPath &o);

	void reverse();

	Coords get_start() const throw () {return m_start;}
	Coords get_end  () const throw () {return m_end;}

	typedef std::vector<Direction> Step_Vector;
	Step_Vector::size_type get_nsteps() const throw () {return m_path.size();}
	Direction operator[](const Step_Vector::size_type i) const throw ()
	{assert(i < m_path.size()); return m_path[m_path.size() - i - 1];}

	void append(const Map & map, const Direction dir);

private:
	Coords m_start;
	Coords m_end;
	Step_Vector m_path;
};

// CoordPath is an extended path that also caches related Coords
struct CoordPath {
	CoordPath() {}
	CoordPath(Coords c) {m_coords.push_back(c);}
	CoordPath(const Map & map, const Path & path);

	Coords get_start() const throw () {return m_coords.front();}
	Coords get_end  () const throw () {return m_coords.back ();}
	const std::vector<Coords> &get_coords() const {return m_coords;}

	typedef std::vector<Direction> Step_Vector;
	Step_Vector::size_type get_nsteps() const throw () {return m_path.size();}
	Direction operator[](const Step_Vector::size_type i) const throw ()
	{assert(i < m_path.size()); return m_path[i];}
	const Step_Vector & steps() const throw () {return m_path;}

	int32_t get_index(Coords field) const;

	void reverse();
	void truncate (const std::vector<char>::size_type after);
	void starttrim(const std::vector<char>::size_type before);
	void append(const Map & map, const Path & tail);
	void append(const CoordPath &tail);

private:
	Step_Vector          m_path;   //  directions
	std::vector<Coords>  m_coords; //  m_coords.size() == m_path.size() + 1
};

/*
==============================================================================

Field arithmetics

==============================================================================
*/

inline Map_Index Map::get_index(Coords const c, X_Coordinate const width)
{return c.y * width + c.x;}

inline Field & Map::operator[](Map_Index const i) const {return m_fields[i];}
inline Field & Map::operator[](const Coords c) const
{return operator[](get_index(c, m_width));}

inline Field * Map::get_field(const Coords c) const {return &operator[](c);}

inline FCoords Map::get_fcoords(const Coords c) const
{
	return FCoords(c, &operator[](c));
}

inline void Map::normalize_coords(Coords * c) const
{
	while (c->x < 0)         c->x += m_width;
	while (c->x >= m_width)  c->x -= m_width;
	while (c->y < 0)         c->y += m_height;
	while (c->y >= m_height) c->y -= m_height;
}


/**
 * Calculate the field coordates from the pointer
 */
inline FCoords Map::get_fcoords(Field & f) const {
	const int32_t i = &f - m_fields;
	return FCoords(Coords(i % m_width, i / m_width), &f);
}
inline void Map::get_coords(Field & f, Coords & c) const {c = get_fcoords(f);}


/** get_ln, get_rn, get_tln, get_trn, get_bln, get_brn
 *
 * Calculate the coordinates and Field pointer of a neighboring field.
 * Assume input coordinates are valid.
 *
 * Note: Input coordinates are passed as value because we have to allow
 *       usage get_XXn(foo, &foo).
 */
inline void Map::get_ln(const Coords f, Coords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	o->y = f.y;
	o->x = (f.x ? f.x : m_width) - 1;
	assert(0 <= o->x);
	assert(0 <= o->y);
	assert(o->x < m_width);
	assert(o->y < m_height);
}

inline void Map::get_ln(const FCoords f, FCoords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	o->y = f.y;
	o->x = f.x - 1;
	o->field = f.field - 1;
	if (o->x == -1) {
		o->x = m_width - 1;
		o->field += m_width;
	}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
	assert(m_fields <= o->field);
	assert            (o->field < m_fields + max_index());
}
inline Coords Map::l_n(const Coords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	Coords result(f.x - 1, f.y);
	if (result.x == -1) result.x = m_width - 1;
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	return result;
}
inline FCoords Map::l_n(const FCoords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	FCoords result(Coords(f.x - 1, f.y), f.field - 1);
	if (result.x == -1) {
		result.x = m_width - 1;
		result.field += m_width;
	}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	assert(m_fields <= result.field);
	assert            (result.field < m_fields + max_index());
	return result;
}

inline void Map::get_rn(const Coords f, Coords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	o->y = f.y;
	o->x = f.x + 1;
	if (o->x == m_width) o->x = 0;
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
}

inline void Map::get_rn(const FCoords f, FCoords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	o->y = f.y;
	o->x = f.x + 1;
	o->field = f.field + 1;
	if (o->x == m_width) {o->x = 0; o->field -= m_width;}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
	assert(m_fields <= o->field);
	assert            (o->field < m_fields + max_index());
}
inline Coords Map::r_n(const Coords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	Coords result(f.x + 1, f.y);
	if (result.x == m_width) result.x = 0;
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	return result;
}
inline FCoords Map::r_n(const FCoords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	FCoords result(Coords(f.x + 1, f.y), f.field + 1);
	if (result.x == m_width) {result.x = 0; result.field -= m_width;}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	assert(m_fields <= result.field);
	assert            (result.field < m_fields + max_index());
	return result;
}

// top-left: even: -1/-1  odd: 0/-1
inline void Map::get_tln(const Coords f, Coords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	o->y = f.y - 1;
	o->x = f.x;
	if (o->y & 1) {
		if (o->y == -1) o->y = m_height - 1;
		o->x = (o->x ? o->x : m_width) - 1;
	}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
}

inline void Map::get_tln(const FCoords f, FCoords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	o->y = f.y - 1;
	o->x = f.x;
	o->field = f.field - m_width;
	if (o->y & 1) {
		if (o->y == -1) {
			o->y = m_height - 1;
			o->field += max_index();
		}
		--o->x;
		--o->field;
		if (o->x == -1) {
			o->x = m_width - 1;
			o->field += m_width;
		}
	}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
	assert(m_fields <= o->field);
	assert            (o->field < m_fields + max_index());
}
inline Coords Map::tl_n(const Coords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	Coords result(f.x, f.y - 1);
	if (result.y & 1) {
		if (result.y == -1) result.y = m_height - 1;
		--result.x;
		if (result.x == -1) result.x = m_width - 1;
	}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	return result;
}
inline FCoords Map::tl_n(const FCoords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	FCoords result(Coords(f.x, f.y - 1), f.field - m_width);
	if (result.y & 1) {
		if (result.y == -1) {
			result.y = m_height - 1;
			result.field += max_index();
		}
		--result.x;
		--result.field;
		if (result.x == -1) {
			result.x = m_width - 1;
			result.field += m_width;
		}
	}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	assert(m_fields <= result.field);
	assert            (result.field < m_fields + max_index());
	return result;
}

// top-right: even: 0/-1  odd: +1/-1
inline void Map::get_trn(const Coords f, Coords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	o->x = f.x;
	if (f.y & 1) {
		++o->x;
		if (o->x == m_width) o->x = 0;
	}
	o->y = (f.y ? f.y : m_height) - 1;
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
}

inline void Map::get_trn(const FCoords f, FCoords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	o->x = f.x;
	o->field = f.field - m_width;
	if (f.y & 1) {
		++o->x;
		++o->field;
		if (o->x == m_width) {
			o->x = 0;
			o->field -= m_width;
		}
	}
	o->y = f.y - 1;
	if (o->y == -1) {
		o->y = m_height - 1;
		o->field += max_index();
	}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
	assert(m_fields <= o->field);
	assert            (o->field < m_fields + max_index());
}
inline Coords Map::tr_n(const Coords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	Coords result(f.x, f.y - 1);
	if (f.y & 1) {++result.x; if (result.x == m_width) result.x = 0;}
	if (result.y == -1) result.y = m_height - 1;
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	return result;
}
inline FCoords Map::tr_n(const FCoords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	FCoords result(Coords(f.x, f.y - 1), f.field - m_width);
	if (f.y & 1) {
		++result.x;
		++result.field;
		if (result.x == m_width) {
			result.x = 0;
			result.field -= m_width;
		}
	}
	if (result.y == -1) {
		result.y = m_height - 1;
		result.field += max_index();
	}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	assert(m_fields <= result.field);
	assert            (result.field < m_fields + max_index());
	return result;
}

// bottom-left: even: -1/+1  odd: 0/+1
inline void Map::get_bln(const Coords f, Coords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	o->y = f.y + 1;
	o->x = f.x;
	if (o->y == m_height) o->y = 0;
	if (o->y & 1) o->x = (o->x ? o->x : m_width) - 1;
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
}

inline void Map::get_bln(const FCoords f, FCoords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	o->y = f.y + 1;
	o->x = f.x;
	o->field = f.field + m_width;
	if (o->y == m_height) {
		o->y = 0;
		o->field -= max_index();
	}
	if (o->y & 1) {
		--o->x;
		--o->field;
		if (o->x == -1) {
			o->x = m_width - 1;
			o->field += m_width;
		}
	}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
	assert(m_fields <= o->field);
	assert            (o->field < m_fields + max_index());
}
inline Coords Map::bl_n(const Coords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	Coords result(f.x, f.y + 1);
	if (result.y == m_height) result.y = 0;
	if (result.y & 1) {--result.x; if (result.x == -1) result.x = m_width - 1;}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	return result;
}
inline FCoords Map::bl_n(const FCoords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	FCoords result(Coords(f.x, f.y + 1), f.field + m_width);
	if (result.y == m_height) {
		result.y = 0;
		result.field -= max_index();
	}
	if (result.y & 1) {
		--result.x;
		--result.field;
		if (result.x == -1) {
			result.x = m_width - 1;
			result.field += m_width;
		}
	}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	assert(m_fields <= result.field);
	assert            (result.field < m_fields + max_index());
	return result;
}

// bottom-right: even: 0/+1  odd: +1/+1
inline void Map::get_brn(const Coords f, Coords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	o->x = f.x;
	if (f.y & 1) {
		(o->x)++;
		if (o->x == m_width) o->x = 0;
	}
	o->y = f.y + 1;
	if (o->y == m_height) o->y = 0;
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
}

inline void Map::get_brn(const FCoords f, FCoords * const o) const
{
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	o->x = f.x;
	o->field = f.field + m_width;
	if (f.y & 1) {
		++o->x;
		++o->field;
		if (o->x == m_width) {
			o->x = 0;
			o->field -= m_width;
		}
	}
	o->y = f.y + 1;
	if (o->y == m_height) {
		o->y = 0;
		o->field -= max_index();
	}
	assert(0 <= o->x);
	assert(o->x < m_width);
	assert(0 <= o->y);
	assert(o->y < m_height);
	assert(m_fields <= o->field);
	assert            (o->field < m_fields + max_index());
}
inline Coords Map::br_n(const Coords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	Coords result(f.x, f.y + 1);
	if (f.y & 1) {++result.x; if (result.x == m_width) result.x = 0;}
	if (result.y == m_height) result.y = 0;
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	return result;
}
inline FCoords Map::br_n(const FCoords f) const {
	assert(0 <= f.x);
	assert(f.x < m_width);
	assert(0 <= f.y);
	assert(f.y < m_height);
	assert(m_fields <= f.field);
	assert            (f.field < m_fields + max_index());
	FCoords result(Coords(f.x, f.y + 1), f.field + m_width);
	if (f.y & 1) {
		++result.x;
		++result.field;
		if (result.x == m_width) {
			result.x = 0;
			result.field -= m_width;
		}
	}
	if (result.y == m_height) {
		result.y = 0;
		result.field -= max_index();
	}
	assert(0 <= result.x);
	assert(result.x < m_width);
	assert(0 <= result.y);
	assert(result.y < m_height);
	assert(m_fields <= result.field);
	assert            (result.field < m_fields + max_index());
	return result;
}

inline FCoords Map::get_neighbour(const FCoords f, const Direction dir) const
throw ()
{
	switch (dir) {
	case Map_Object::WALK_NW: return tl_n(f);
	case Map_Object::WALK_NE: return tr_n(f);
	case Map_Object::WALK_E:  return  r_n(f);
	case Map_Object::WALK_SE: return br_n(f);
	case Map_Object::WALK_SW: return bl_n(f);
	case Map_Object::WALK_W:  return  l_n(f);
	}
	assert(false);
}

inline void move_r(const X_Coordinate mapwidth, FCoords & f) {
	assert(f.x < mapwidth);
	++f.x;
	++f.field;
	if (f.x == mapwidth) {f.x = 0; f.field -= mapwidth;}
	assert(f.x < mapwidth);
}

inline void move_r(X_Coordinate const mapwidth, FCoords & f, Map_Index & i) {
	assert(f.x < mapwidth);
	++f.x;
	++f.field;
	++i;
	if (f.x == mapwidth) {f.x = 0; f.field -= mapwidth; i -= mapwidth;}
	assert(f.x < mapwidth);
}


#define iterate_Map_FCoords(map, extent, fc)                                  \
	Widelands::FCoords fc(map.get_fcoords(Widelands::Coords(0, 0)));           \
	for (fc.y = 0; fc.y < extent.h; ++fc.y)                                    \
		for (fc.x = 0; fc.x < extent.w; ++fc.x, ++fc.field)                     \


std::string g_VariableCallback(std::string str, void* data);

};

#endif // __S__MAP_H
