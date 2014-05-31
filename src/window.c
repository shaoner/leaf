/*
 *
 * Copyright 2014, Alexandre Laurent
 *
 * This file is part of leaf.
 *
 * leaf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * leaf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with leaf. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "window.h"
#include "leaf.h"
#include "utils.h"

#include <xcb/xcb.h>

void window_configure(xcb_window_t win)
{
	uint32_t vals[1];

	print_d("Configure window %d", win);

	/* Border color */
	vals[0] = 0x666666;
	xcb_change_window_attributes(gconf.conn, win, XCB_CW_BORDER_PIXEL, vals);

	/* Border width */
	vals[0] = 1;
	xcb_configure_window(gconf.conn, win, XCB_CONFIG_WINDOW_BORDER_WIDTH, vals);

	vals[0] = XCB_EVENT_MASK_ENTER_WINDOW;
	xcb_change_window_attributes_checked(gconf.conn, win, XCB_CW_EVENT_MASK, vals);

	/* Show window on screen. */
	xcb_map_window(gconf.conn, win);
	xcb_flush(gconf.conn);
}
