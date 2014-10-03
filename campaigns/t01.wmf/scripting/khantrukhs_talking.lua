-- ======================
-- Messages by Khantrukh
-- ======================

include "map:scripting/texts.lua"
include "scripting/coroutine.lua"
include "scripting/messages.lua"
include "scripting/objective_utils.lua"

map = wl.Game().map

function check_quarries()
   while not check_for_buildings(p, {quarry = 2},
      map:get_field(8,13):region(3)) do sleep(5000) end
   objq.done = true
end

function check_ranger()
   while not check_for_buildings(p, {rangers_hut = 1},
      map:get_field(17,11):region(3)) do sleep(5000) end
   objr.done = true
end

function tutorial_thread()
   p = wl.Game().players[1]
   crossroads = map:get_field(14,11)

   show_story_box(_"Somebody approaches you", khantrukh_1)
   show_story_box(_"The Advisor", khantrukh_2, nil, 80, 80)
   local o = add_campaign_objective(start_lumberjack_01)

   -- Wait till the hut is build.
   while not check_for_buildings(p, {constructionsite = 1},
      map:get_field(15,11):region(2)) do sleep(5000) end

   -- Tell player to build a road
	p:message_box(_"The Advisor", khantrukh_3, {field = crossroads, h = 400})

   -- Wait till the hut is build.
   while not check_for_buildings(p, {lumberjacks_hut = 1},
      map:get_field(15,11):region(2)) do sleep(5000) end
   o.done = true

	if (not (crossroads.immovable and crossroads.immovable.descr.type_name == "flag")) then
		message_box(p, _"The Advisor", khantrukh_4, { h = 400 })
		message_box(p, _"The Advisor", khantrukh_5, { h = 400 })
		o = add_campaign_objective(start_lumberjack_02)
	else
	   message_box(p, _"The Advisor", khantrukh_5, { h = 400 })
		o = add_campaign_objective(start_lumberjack_02_02)
	end

   -- Wait till the hut is build.
   while not check_for_buildings(p, {lumberjacks_hut = 1},
         map:get_field(12,13):region(2)) do sleep(5000) end
   o.done = true

	message_box(p, _"The Advisor", khantrukh_6, { h = 400 })
   p:allow_buildings{"rangers_hut", "quarry"}
   objq = add_campaign_objective(start_quarries)
   objr = add_campaign_objective(start_ranger)
   run(check_ranger)
   run(check_quarries)

	message_box(p, _"The Advisor", khantrukh_7, { h = 400 })
   while not (objr.done and objq.done) do
      sleep(5000)
   end

   p:reveal_scenario("barbariantut01")
   p:message_box(_"Mission Complete", khantrukh_8, { h = 400 })
end

run(tutorial_thread)
