/*
 * Copyright (C) 2002 by the Widelands Development Team
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
#ifndef __WORLD_H
#define __WORLD_H

#include "bob.h"
#include "descr_maintainer.h"
#include "immovable.h"
#include "worlddata.h"

class Section;
class Editor_Game_Base;

#define WORLD_NAME_LEN 30
#define WORLD_AUTHOR_LEN 30
#define WORLD_DESCR_LEN 1024

struct World_Descr_Header {
   char name[WORLD_NAME_LEN];
   char author[WORLD_AUTHOR_LEN];
   char descr[WORLD_DESCR_LEN];
};

class Resource_Descr {
public:
	Resource_Descr() { }
	~Resource_Descr() { }

	void parse(Section* s, std::string);

   const char* get_name() const { return m_name.c_str(); }
	std::string get_indicator(uint amount) const;

   inline bool is_detectable(void) { return m_is_detectable; }
   inline int get_max_amount(void) { return m_max_amount; }

   std::string get_editor_pic(uint amount);

private:
	struct Indicator {
		std::string		bobname;
		int				upperlimit;
	};
   struct Editor_Pic {
      std::string    picname;
      int            upperlimit;
   };

   bool                    m_is_detectable;
   int                     m_max_amount;
	std::string					m_name;
	std::vector<Indicator>	m_indicators;
	std::vector<Editor_Pic>	m_editor_pics;
};

class Terrain_Descr {
   friend class Descr_Maintainer<Terrain_Descr>;
   friend class World;

   public:
      Terrain_Descr(const char* directory, Section* s, Descr_Maintainer<Resource_Descr>*);
      ~Terrain_Descr(void);

		void load_graphics();

      inline uint get_texture(void) { return m_texture; }
      inline uchar get_is(void) { return m_is; }
      inline const char* get_name() { return m_name; }

      inline bool is_resource_valid(int res) {
         int i=0;
         for(i=0; i<m_nr_valid_resources; i++)
            if(m_valid_resources[i]==res) return true;
         return false;
      }
      inline int get_default_resources(void) { return m_default_resources; }
      inline int get_default_resources_amount(void) { return m_default_amount; }

   private:
      char		m_name[30];
		char*		m_picnametempl;
		uint		m_frametime;
      uchar		m_is;

      uchar*   m_valid_resources;
      uchar    m_nr_valid_resources;
      char     m_default_resources;
      int      m_default_amount;
		uint		m_texture; // renderer's texture
};

/** class World
  *
  * This class provides information on a worldtype usable to create a map;
  * it can read a world file.
  */
class World
{
	friend class Game;

   public:

      enum {
         OK = 0,
         ERR_WRONGVERSION
      };

      World(const char* name);
		~World();

      // Check if a world really exists
      static bool exists_world(std::string);
      static void get_all_worlds(std::vector<std::string>*);

		void postload(Editor_Game_Base*);
		void load_graphics();

      inline const char* get_name(void) { return hd.name; }
      inline const char* get_author(void) { return hd.author; }
      inline const char* get_descr(void) { return hd.descr; }

      inline Terrain_Descr* get_terrain(uint i) { assert(i<ters.get_nitems()); return ters.get(i); }
      inline Terrain_Descr* get_terrain(char* str ) { int i=ters.get_index(str); if(i==-1) return 0; return ters.get(i); }
      inline int get_nr_terrains(void) { return ters.get_nitems(); }
      inline int get_bob(const char* l) { return bobs.get_index(l); }
		inline Bob_Descr* get_bob_descr(ushort index) { return bobs.get(index); }
      inline int get_nr_bobs(void) { return bobs.get_nitems(); }
      inline int get_immovable_index(const char* l) { return immovables.get_index(l); }
      inline int get_nr_immovables(void) { return immovables.get_nitems(); }
		inline Immovable_Descr* get_immovable_descr(int index) { return immovables.get(index); }

      inline uchar get_resource(const char* l) { return m_resources.get_index(l); }
		inline Resource_Descr* get_resource(int res)
		{ assert(res < m_resources.get_nitems()); return m_resources.get(res); }
      inline int get_nr_resources(void) { return m_resources.get_nitems(); }

   private:
		std::string				m_basedir;	// base directory, where the main conf file resides
      World_Descr_Header	hd;

      Descr_Maintainer<Bob_Descr> bobs;
		Descr_Maintainer<Immovable_Descr> immovables;
      Descr_Maintainer<Terrain_Descr> ters;
		Descr_Maintainer<Resource_Descr>	m_resources;

      // Functions
      void parse_root_conf(const char *name);
      void parse_resources();
      void parse_terrains();
      void parse_bobs();
};

#endif

