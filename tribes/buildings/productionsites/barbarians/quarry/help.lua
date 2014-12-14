-- The Barbarian Quarry
include "scripting/formatting.lua"
set_textdomain("tribes")
include "tribes/scripting/format_help.lua"

return {
   func = function(building_description)
	return

	--Lore Section
	building_help_lore_string("barbarians", building_description, _[[Text needed]],_[[Source needed]]) ..

	--General Section
	building_help_general_string("barbarians", building_description,
		_"Cuts raw pieces of granite out of rocks in the vicinity.", _"The quarry needs rocks to cut within the working radius.") ..

	--Dependencies
	building_help_dependencies_production("barbarians", building_description, true) ..

	--Workers Section
	building_help_crew_string("barbarians", building_description) ..

	--Building Section
	building_help_building_section("barbarians", building_description) ..

	--Production Section
	building_help_production_section(_"The stonemason pauses %s before going back to work again.":bformat(
		ngettext("%d second", "%d seconds", 65):bformat(65)
	))
   end
}
