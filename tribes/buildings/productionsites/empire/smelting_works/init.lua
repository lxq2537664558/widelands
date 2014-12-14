dirname = path.dirname(__file__)

tribes:new_productionsite_type {
   name = "empire_smelting_works",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = _"Smelting Works",
   size = "medium",

   buildcost = {
		log = 1,
		granite = 4,
		marble = 2
	},
	return_on_dismantle = {
		granite = 3,
		marble = 1
	},

	-- #TRANSLATORS: Helptext for a building: Smelting Works
   helptext = "", -- NOCOM(GunChleoc): See what we can shift over from help.lua here

   animations = {
		idle = {
			pictures = { dirname .. "idle_\\d+.png" },
			hotspot = { 39, 53 },
		},
		build = {
			pictures = { dirname .. "build_\\d+.png" },
			hotspot = { 39, 53 },
		},
		working = {
			pictures = { dirname .. "working_\\d+.png" },
			hotspot = { 39, 53 },
			fps = 5
		},
	},

   aihints = {
		prohibited_till = 600
   },

	working_positions = {
		empire_smelter = 1
	},

   inputs = {
		iron_ore = 8,
		gold_ore = 8,
		coal = 8
	},
   outputs = {
		"iron",
		"gold"
   },

	programs = {
		work = {
			-- TRANSLATORS: Completed/Skipped/Did not start working because ...
			descname = _"working",
			actions = {
				"call=smelt_iron",
				"call=smelt_gold",
				"call=smelt_iron",
				"return=skipped"
			}
		},
		smelt_iron = {
			-- TRANSLATORS: Completed/Skipped/Did not start smelting iron because ...
			descname = _"smelting iron",
			actions = {
				"return=skipped unless economy needs iron",
				"sleep=25000",
				"consume=iron_ore coal",
				"animate=working 35000",
				"produce=iron"
			}
		},
		smelt_gold = {
			-- TRANSLATORS: Completed/Skipped/Did not start smelting gold because ...
			descname = _"smelting gold",
			actions = {
				"return=skipped unless economy needs gold",
				"sleep=25000",
				"consume=gold_ore coal",
				"animate=working 35000",
				"produce=gold"
			}
		},
	},
}
