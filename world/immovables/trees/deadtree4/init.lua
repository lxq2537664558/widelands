dirname = path.dirname(__file__)

world:new_immovable_type{
   name = "deadtree4",
   descname = _ "Dead Tree",
   editor_category = "trees_dead",
   size = "none",
   attributes = {},
   programs = {
      program = {
	     "animate=idle 20000",
		 "remove=18"
      }
   },
   animations = {
      idle = {
         template = "idle",
         directory = dirname,
         hotspot = { 23, 61 },
      },
   }
}
