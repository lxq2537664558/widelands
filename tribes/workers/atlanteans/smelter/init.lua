dirname = path.dirname(__file__)

animations = {
   idle = {
      pictures = path.list_directory(dirname, "idle_\\d+.png"),
      hotspot = { 10, 22 }
   }
}
add_worker_animations(animations, "walk", dirname, "walk", {12, 22}, 10)
add_worker_animations(animations, "walkload", dirname, "walkload", {12, 24}, 10)


tribes:new_worker_type {
   msgctxt = "atlanteans_worker",
   name = "atlanteans_smelter",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = pgettext("atlanteans_worker", "Smelter"),
   icon = dirname .. "menu.png",
   vision_range = 2,

   buildcost = {
		atlanteans_carrier = 1,
		fire_tongs = 1
	},

	-- TRANSLATORS: Helptext for a worker: Smelter
   helptext = pgettext("atlanteans_worker", "Smelts ores into metal."),
   animations = animations,
}
