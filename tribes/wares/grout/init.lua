dirname = path.dirname(__file__)

tribes:new_ware_type{
   name = "grout",
   -- TRANSLATORS: This is a ware name used in lists of wares
   descname = _"Grout",
   -- TRANSLATORS: mass description, e.g. 'The economy needs ...'
   genericname = _"grout",
   icon = dirname .. "menu.png",
   default_target_quantity = {
		barbarians = 10
	},
   preciousness = {
		barbarians = 5
	},
   helptext = {
		-- TRANSLATORS: Helptext for a ware: Grout
		barbarians = _"Granite can be processed into grout which provides a solid, non burning building material. Grout is produced in a lime kiln."
   },
   animations = {
      idle = {
         pictures = { dirname .. "idle.png" },
         hotspot = { 5, 12 },
      },
   }
}
