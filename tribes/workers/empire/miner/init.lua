dirname = path.dirname(__file__)

animations = {
   idle = {
      pictures = path.list_directory(dirname, "idle_\\d+.png"),
      hotspot = { 3, 23 }
   }
}
add_worker_animations(animations, "walk", dirname, "walk", {9, 24}, 10)
add_worker_animations(animations, "walkload", dirname, "walkload", {7, 24}, 10)


tribes:new_worker_type {
   msgctxt = "empire_worker",
   name = "empire_miner",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = pgettext("empire_worker", "Miner"),
   icon = dirname .. "menu.png",
   vision_range = 2,

   buildcost = {
		empire_carrier = 1,
		pick = 1
	},

	experience = 19,
	becomes = "empire_miner_master",

	-- TRANSLATORS: Helptext for a worker: Miner
   helptext = pgettext("empire_worker", "Works deep in the mines to obtain coal, iron, gold or marble."),
   animations = animations,
}
