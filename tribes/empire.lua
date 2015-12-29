dirname = path.dirname(__file__)

tribes:new_tribe {
   name = "empire",

   animations = {
      -- No idea for the frontier. Maybe some javelins?
      frontier = {
         template = "frontier_??",
         directory = dirname .. "images/empire/",
         hotspot = { 1, 19 },
      },
      -- Not just a plain color, maybe a cross or some stripes
      flag = {
         template = "flag_??",
         directory = dirname .. "images/empire/",
         hotspot = { 14, 38 },
         fps = 10
      }
   },

   -- Image file paths for this tribe's road textures
   roads = {
      busy = {
         "tribes/images/empire/roadt_busy.png",
      },
      normal = {
         "tribes/images/empire/roadt_normal_00.png",
         "tribes/images/empire/roadt_normal_01.png",
         "tribes/images/empire/roadt_normal_02.png",
      },
   },

   -- Wares positions in wares windows.
   -- This also gives us the information which wares the tribe uses.
   -- Each subtable is a column in the wares windows.
   wares_order = {
      {
         -- Building Materials
         "granite",
         "log",
         "planks",
         "wool",
         "cloth"
      },
      {
         -- Food
         "fish",
         "meat",
         "water",
         "wheat",
         "flour",
         "empire_bread",
         "beer",
         "grape",
         "wine",
         "ration",
         "meal"
      },
      {
         -- Mining
         "marble",
         "marble_column",
         "coal",
         "iron_ore",
         "iron",
         "gold_ore",
         "gold"
      },
      {
         -- Tools
         "pick",
         "felling_ax",
         "saw",
         "shovel",
         "hammer",
         "fishing_rod",
         "hunting_spear",
         "scythe",
         "bread_paddle",
         "basket",
         "kitchen_tools",
         "fire_tongs"
      },
      {
         -- Weapons & Armor
         "spear_wooden",
         "spear",
         "spear_advanced",
         "spear_heavy",
         "spear_war",
         "armor_helmet",
         "armor",
         "armor_chain",
         "armor_gilded"
      }
   },
   -- Workers positions in workers windows.
   -- This also gives us the information which workers the tribe uses.
   -- Each subtable is a column in the workers windows.
   workers_order = {
      {
         -- Carriers
         "empire_carrier",
         "empire_donkey",
         "empire_donkeybreeder"
      },
      {
         -- Building Materials
         "empire_stonemason",
         "empire_carpenter",
         "empire_lumberjack",
         "empire_forester",
         "empire_builder",
         "empire_shepherd",
         "empire_weaver",
         "empire_shipwright"
      },
      {
         -- Food
         "empire_fisher",
         "empire_hunter",
         "empire_farmer",
         "empire_miller",
         "empire_baker",
         "empire_brewer",
         "empire_vinefarmer",
         "empire_vintner",
         "empire_pigbreeder",
         "empire_innkeeper"
      },
      {
         -- Mining
         "empire_geologist",
         "empire_miner",
         "empire_miner_master",
         "empire_charcoal_burner",
         "empire_smelter"
      },
      {
         -- Tools
         "empire_toolsmith"
      },
      {
         -- Military
         "empire_soldier",
         "empire_trainer",
         "empire_weaponsmith",
         "empire_armorsmith",
         "empire_scout"
      }
   },

   immovables = {
      "ashes",
      "destroyed_building",
      "field_tiny",
      "field_small",
      "field_medium",
      "field_ripe",
      "field_harvested",
      "grapevine_tiny",
      "grapevine_small",
      "grapevine_medium",
      "grapevine_ripe",
      "resi_coal1",
      "resi_coal2",
      "resi_gold1",
      "resi_gold2",
      "resi_iron1",
      "resi_iron2",
      "resi_none",
      "resi_water1",
      "resi_stones1",
      "resi_stones2",
      "empire_shipconstruction",
   },

   -- The order here also determines the order in lists on screen.
   buildings = {
      -- Warehouses
      "empire_headquarters",
      "empire_headquarters_shipwreck",
      "empire_warehouse",
      "empire_port",

      -- Small
      "empire_quarry",
      "empire_lumberjacks_house",
      "empire_foresters_house",
      "empire_fishers_house",
      "empire_hunters_house",
      "empire_well",
      "empire_scouts_house",

      -- Medium
      "empire_stonemasons_house",
      "empire_sawmill",
      "empire_mill",
      "empire_bakery",
      "empire_brewery",
      "empire_vineyard",
      "empire_winery",
      "empire_tavern",
      "empire_inn",
      "empire_charcoal_kiln",
      "empire_smelting_works",
      "empire_toolsmithy",
      "empire_armorsmithy",
      "empire_shipyard",

      -- Big
      "empire_donkeyfarm",
      "empire_farm",
      "empire_piggery",
      "empire_sheepfarm",
      "empire_weaving_mill",
      "empire_weaponsmithy",


      -- Mines
      "empire_marblemine",
      "empire_marblemine_deep",
      "empire_coalmine",
      "empire_coalmine_deep",
      "empire_ironmine",
      "empire_ironmine_deep",
      "empire_goldmine",
      "empire_goldmine_deep",

      -- Training Sites
      "empire_arena",
      "empire_colosseum",
      "empire_trainingcamp",

      -- Military Sites
      "empire_blockhouse",
      "empire_sentry",
      "empire_outpost",
      "empire_barrier",
      "empire_tower",
      "empire_fortress",
      "empire_castle",

      -- Partially Finished Buildings - these are the same 2 buildings for all tribes
      "constructionsite",
      "dismantlesite",
   },

   ship_names = {
      "Lutius",
      "Saledus",
      "Amalea",
      "Cicero",
      "Ceasar",
      "Alexandria", 
      "Porto Bello",
      "Baltic Sea",
      "Azurea Sea",
      "Siena",
      "Marble",
      "Vine",
      "Adriaitic Sea",
      "Carrara",
      "Liguria",
      "Thyrrenia",
      "Empire Glory",
      "Leonardo",
      "Jupiter",
      "Neptune",
      "Juno",
      "Mars",
      "Venus",
      "Bellona",
      "Minerva",
      "Janus",
      "Vesta",
      "Neptune",
      "Mars",
      "Apollo",
      "Venus",
      "Diana",
      "Jupiter",
      "Pluto",
      "Saturn",
      "Julius Caesar",
      "Octavianus",
      "Augustus",
      "Juno",
      "Tiberius",
      "Caligula",
      "Claudius",
      "Nero",
      "Galba",
      "Otho",
      "Vitellius",
      "Vespasianus",
      "Titus",
      "Domitianus",
      "Nerva",
      "Trajan",
      "Romulus",
      "Remus",
      "Vesta",
      "Latonia",
      "Faustulus",
      "Amolius",
      "Brutus",
      "Quirinus",
      "Bounty",
      "Latium",
      "Camilla",
      "Gallus",
      "Sparta",
      "Ben Hur",
      "Neptune",
   },

   -- Special types
   builder = "empire_builder",
   carrier = "empire_carrier",
   carrier2 = "empire_donkey",
   geologist = "empire_geologist",
   soldier = "empire_soldier",
   ship = "empire_ship",
   headquarters = "empire_headquarters",
   port = "empire_port",
}
