use("aux", "formatting")
use("aux", "format_help")

set_textdomain("tribe_barbarians")

return {
	text =
		rt(h1(_"The Barbarian Deep Gold Mine")) ..
	--Lore Section
		rt(h2(_"Lore")) ..
		rt("image=tribes/barbarians/deep_goldmine/deep_goldmine_i_00.png", p(--text identical to goldmine
			_[["Soft and supple.<br> And yet untouched by time and weather.<br> Rays of sun, wrought into eternity ..."]])) ..
		rt("text-align=right",p("font-size=10 font-style=italic", [[Excerpt from "Our treasures underground",<br> a traditional Barbarian song.]])) ..
	--General Section
		rt(h2(_"General")) ..
		rt(p(_"Deep Gold Mines exploit only 2/3 of the resource.<br>" .. _"From there on, they will only have a 5% chance of finding gold ore.")) ..
		rt(h3(_"Purpose:")) ..
		image_line_text("tribes/barbarians/goldstone/menu.png", 1, p(_"Dig gold ore out of the ground in mountain terrain.")) ..
		text_line(_"Working radius:", "2") ..
		text_line(_"Conquer range:", "n/a") ..
		text_line(_"Vision range:", "4") ..
	--Dependencies
		rt(h2(_"Dependencies")) ..
		rt(h3(_"Incoming:")) ..
		image_line_dep("tribes/barbarians/inn/menu.png","tribes/barbarians/snack/menu.png","tribes/barbarians/deep_goldmine/menu.png", p(_"Snacks from an Inn")) ..
		image_line_dep("tribes/barbarians/big_inn/menu.png","tribes/barbarians/snack/menu.png","tribes/barbarians/deep_goldmine/menu.png", p(_"Snacks from a Big Inn")) ..
		rt(h3(_"Outgoing:")) ..
		image_line_dep("tribes/barbarians/resi_gold2/resi_00.png","tribes/barbarians/deep_goldmine/menu.png","tribes/barbarians/goldstone/menu.png", p(_"Gold Ore")) ..
		image_line_out("tribes/barbarians/goldstone/menu.png","tribes/barbarians/smelting_works/menu.png", p(_"Smelting Works")) ..
		rt(p(_"Gold ore always goes to the Smelting works. It has no other use.")) ..
	--Building Section
		rt(h2(_"Building")) ..
		text_image_line(_"Space required:",_"Mine plot","pics/mine.png") ..
		text_image_line(_"Upgraded from:",_"Gold Mine","tribes/barbarians/goldmine/menu.png") ..
		rt(h3(_"Upgrade cost:")) ..
		image_line_text("tribes/barbarians/raw_stone/menu.png", 2, p("2 " .. _"Raw Stone")) ..
		image_line_text("tribes/barbarians/trunk/menu.png", 4, p("4 " .. _"Trunk")) ..
		rt(h3(_"Cost cumulative:")) ..
		image_line_text("tribes/barbarians/raw_stone/menu.png", 4, p("4 " .. _"Raw Stone")) ..
		image_line_text("tribes/barbarians/trunk/menu.png", 8, p("8 " .. _"Trunk")) ..
		rt(h3(_"Dismantle yields:")) ..
		image_line_text("tribes/barbarians/raw_stone/menu.png", 2, p("2 " .. _"Raw Stone")) ..
		image_line_text("tribes/barbarians/trunk/menu.png", 4, p("4 " .. _"Trunk")) ..
		text_image_line(_"Upgradeable to:",_"Deeper Gold Mine","tribes/barbarians/deeper_goldmine/menu.png") ..
	--Workers Section
		rt(h2(_"Workers")) ..
		rt(h3(_"Crew required:")) ..
		image_line_text("tribes/barbarians/chief-miner/menu.png",1, p("1 " .. _"Chief Miner" .. _" or better" .. _" and")) ..
		image_line_text("tribes/barbarians/miner/menu.png",1, p("1 " .. _"Miner" .. _" or better" .. ".")) ..
		text_image_line(_"Workers use:",_"Pick","tribes/barbarians/pick/menu.png") ..
		rt(h3(_"Experience levels:")) ..
		rt("text-align=right", p(_"Miner" .. _" to " .. _"Chief Miner" .. " (19 " .. _"EP" .. ")<br>" .. _"Chief Miner" .. _" to " .. _"Master Miner" .. " (28 " .. _"EP" ..")")) ..
	--Production Section
		rt(h2(_"Production")) ..
		text_line(_"Performance:", _"If the food supply is steady, a Deep Gold Mine can produce one gold ore in 19.5s on average.")
}