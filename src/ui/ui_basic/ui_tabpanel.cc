/*
 * Copyright (C) 2003 by Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <cassert>
#include "graphic.h"
#include "rendertarget.h"
#include "types.h"
#include "ui_tabpanel.h"


#define TP_BUTTON_WIDTH			34
#define TP_BUTTON_HEIGHT		34

#define TP_SEPARATOR_HEIGHT	4	// height of the bar separating buttons and tab contents


/**
Initialize an empty UITab_Panel
*/
UITab_Panel::UITab_Panel(UIPanel* parent, int x, int y, uint background)
	: UIPanel(parent, x, y, 0, 0)
{
	m_active = 0;
	m_snapparent = false;

	switch(background) {
	default:
	case 0: m_pic_background = g_gr->get_picture(PicMod_UI, "pics/but0.png"); break;
	case 1: m_pic_background = g_gr->get_picture(PicMod_UI, "pics/but1.png"); break;
	case 2: m_pic_background = g_gr->get_picture(PicMod_UI, "pics/but2.png"); break;
	case 3: m_pic_background = g_gr->get_picture(PicMod_UI, "pics/but3.png"); break;
	}

	m_highlight = -1;
}


/**
Resize according to number of tabs and the size of the currently visible
contents.
Resize the parent if snapparent is enabled.
*/
void UITab_Panel::resize()
{
	int w;
	int h;

	// size of button row
	w = TP_BUTTON_WIDTH * m_tabs.size();
	h = TP_BUTTON_HEIGHT + TP_SEPARATOR_HEIGHT;

	// size of contents
	if (m_active < m_tabs.size())
		{
		UIPanel* panel = m_tabs[m_active].panel;

		if (panel->get_w() > w)
			w = panel->get_w();
		h += panel->get_h();
		}

	set_size(w, h);

	// adjust parent's size, if necessary
	if (m_snapparent)
		get_parent()->set_inner_size(w, h);
}


/**
If snapparent is enabled, the parent window will be adjusted to the size of the
Tab_Panel whenever the UITab_Panel's size changes.
The default for snapparent behaviour is off.
*/
void UITab_Panel::set_snapparent(bool snapparent)
{
	m_snapparent = snapparent;
}


/**
Add a new tab
*/	
uint UITab_Panel::add(uint picid, UIPanel* panel)
{
	assert(panel);
	assert(panel->get_parent() == this);

	Tab t;
	uint id;

	t.picid = picid;
	t.panel = panel;

	m_tabs.push_back(t);
	id = m_tabs.size() - 1;

	panel->set_pos(0, TP_BUTTON_HEIGHT + TP_SEPARATOR_HEIGHT);
	panel->set_visible(id == m_active);

	return id;
}


/**
Make a different tab the currently active tab.
*/
void UITab_Panel::activate(uint idx)
{
	if (m_active < m_tabs.size())
		m_tabs[m_active].panel->set_visible(false);
	if (idx < m_tabs.size())
		m_tabs[idx].panel->set_visible(true);

	m_active = idx;

	resize();
}


/**
Draw the buttons and the tab
*/
void UITab_Panel::draw(RenderTarget* dst)
{
	uint idx;
	int x;

	// draw the background
	dst->tile(0, 0, m_tabs.size() * TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT - 2, 
	          m_pic_background, get_x(), get_y());
	dst->tile(0, TP_BUTTON_HEIGHT - 2, get_w(), get_h() - TP_BUTTON_HEIGHT + 2,
	          m_pic_background, get_x(), get_y() + TP_BUTTON_HEIGHT - 2);


	// draw the buttons
	for(idx = 0, x = 0; idx < m_tabs.size(); idx++, x += TP_BUTTON_WIDTH)
		{
		int cpw, cph;

		if (m_highlight == (int)idx)
			dst->brighten_rect(x, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT, MOUSE_OVER_BRIGHT_FACTOR);

		// Draw the icon
		g_gr->get_picture_size(m_tabs[idx].picid, &cpw, &cph);

		dst->blit(x + (TP_BUTTON_WIDTH - cpw)/2, (TP_BUTTON_HEIGHT - cph)/2, m_tabs[idx].picid);

		// Draw top part of border
		RGBColor black(0,0,0);

		dst->brighten_rect(x, 0, TP_BUTTON_WIDTH, 2, BUTTON_EDGE_BRIGHT_FACTOR);
		dst->brighten_rect(x, 2, 2, TP_BUTTON_HEIGHT - 4, BUTTON_EDGE_BRIGHT_FACTOR);
		dst->fill_rect(x+TP_BUTTON_WIDTH-2, 2, 1, TP_BUTTON_HEIGHT - 4, black);
		dst->fill_rect(x+TP_BUTTON_WIDTH-1, 1, 1, TP_BUTTON_HEIGHT - 3, black);

		// Draw bottom part
		if (m_active != idx)
			dst->brighten_rect(x, TP_BUTTON_HEIGHT-2, TP_BUTTON_WIDTH, 2, 2*BUTTON_EDGE_BRIGHT_FACTOR);
		else
			{
			dst->brighten_rect(x, TP_BUTTON_HEIGHT-2, 2, 2, BUTTON_EDGE_BRIGHT_FACTOR);

			dst->brighten_rect(x+TP_BUTTON_WIDTH-2, TP_BUTTON_HEIGHT-2, 2, 2, BUTTON_EDGE_BRIGHT_FACTOR);
			dst->fill_rect(x+TP_BUTTON_WIDTH-1, TP_BUTTON_HEIGHT-2, 1, 1, black);
			dst->fill_rect(x+TP_BUTTON_WIDTH-2, TP_BUTTON_HEIGHT-1, 2, 1, black);
			}
		}

	// draw the remaining separator
	dst->brighten_rect(x, TP_BUTTON_HEIGHT-2, get_w()-x, 2, 2*BUTTON_EDGE_BRIGHT_FACTOR);
}


/**
Cancel all highlights when the mouse leaves the panel
*/
void UITab_Panel::handle_mousein(bool inside)
{
	if (!inside && m_highlight >= 0)
		{
		update(m_highlight*TP_BUTTON_WIDTH, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT);

		m_highlight = -1;
		}
}


/**
Update highlighting
*/
void UITab_Panel::handle_mousemove(int x, int y, int xdiff, int ydiff, uint btns)
{
	int hl;

	if (y < 0 || y >= TP_BUTTON_HEIGHT)
		hl = -1;
	else
		{
		hl = x / TP_BUTTON_WIDTH;

		if (hl < 0 || hl >= (int)m_tabs.size())
			hl = -1;
		}

	if (hl != m_highlight)
		{
		if (m_highlight >= 0)
			update(m_highlight*TP_BUTTON_WIDTH, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT);
		if (hl >= 0)
			update(hl*TP_BUTTON_WIDTH, 0, TP_BUTTON_WIDTH, TP_BUTTON_HEIGHT);

		m_highlight = hl;
		}
}


/**
Change the active tab if a tab button has been clicked
*/
bool UITab_Panel::handle_mouseclick(uint btn, bool down, int x, int y)
{
	if (btn == 0) // left mouse button
		{
		int id;

		if (y >= TP_BUTTON_HEIGHT)
			return false;

		id = x / TP_BUTTON_WIDTH;

		if (id >= 0 && id < (int)m_tabs.size())
			{
			activate(id);

			return true;
			}
		}

	return false;
}
