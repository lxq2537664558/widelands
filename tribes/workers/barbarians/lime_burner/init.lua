dirname = path.dirname(__file__)

animations = {
   idle = {
      pictures = path.list_directory(dirname, "idle_\\d+.png"),
      hotspot = { 4, 23 },
   }
}
add_worker_animations(animations, "walk", dirname, "walk", {7, 23}, 10)
add_worker_animations(animations, "walkload", dirname, "walkload", {8, 27}, 10)


tribes:new_worker_type {
   msgctxt = "barbarians_worker",
   name = "barbarians_lime_burner",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = pgettext("barbarians_worker", "Lime-Burner"),
   icon = dirname .. "menu.png",
   vision_range = 2,

   buildcost = {
		barbarians_carrier = 1
	},

	-- TRANSLATORS: Helptext for a worker: Lime-Burner
   helptext = pgettext("barbarians_worker", "Burns lime in the lime kiln."),
   animations = animations,
}
