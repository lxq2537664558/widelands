dirname = path.dirname(__file__)

tribes:new_immovable_type {
   msgctxt = "immovable",
   name = "resi_stones2",
   -- TRANSLATORS: This is a resource name used in lists of resources
   descname = pgettext("immovable", "A Lot of Stones"),
   attributes = { "resi" },
   programs = {
      program = {
         "animate=idle 600000",
         "remove="
      }
   },

   animations = {
      idle = {
         pictures = path.list_files(dirname .. "idle_??.png"),
         hotspot = { 7, 10 },
      },
   }
}
