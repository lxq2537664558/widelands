dirname = path.dirname(__file__)

animations = {
   idle = {
      pictures = path.glob(dirname, "sheep_\\d+.png"),
      sound_effects = {
         [0] = path.glob("sound/farm", "sheep_\\d+.ogg"),
      },
      player_color_masks = {},
      hotspot = { 8, 16 },
      fps = 20,
   },
}
add_walking_animations(animations, dirname, "sheep_walk", {15, 25}, 20)

world:new_critter_type{
   name = "sheep",
   descname = _ "Sheep",
   swimming = false,
   attributes = { "eatable" },
   programs = {
      remove = { "remove" },
   },
   animations = animations,
}
