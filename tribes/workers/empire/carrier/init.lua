dirname = path.dirname(__file__)

animations = {
   idle = {
      pictures = path.list_directory(dirname, "idle_\\d+.png"),
      hotspot = { 7, 22 },
      fps = 10
   }
}
add_worker_animations(animations, "walk", dirname, "walk", {7, 20}, 10)
add_worker_animations(animations, "walkload", dirname, "walkload", {8, 22}, 10)


tribes:new_carrier_type {
   msgctxt = "empire_worker",
   name = "empire_carrier",
   -- TRANSLATORS: This is a worker name used in lists of workers
   descname = pgettext("empire_worker", "Carrier"),
   icon = dirname .. "menu.png",
   vision_range = 2,

	-- TRANSLATORS: Helptext for a worker: Carrier
   helptext = pgettext("empire_worker", "Carries items along your roads."),
   animations = animations,
}
