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

#ifndef WINMAP_H
# define WINMAP_H

#include "error.h"
#include "window.h"

#include <xcb/xcb.h>

# define WIN_TABLE_SIZE (512)

typedef struct _winmap_t {
    winlist_t **table;
} winmap_t;

leaf_error_t winmap_new(winmap_t **newmap);
void winmap_delete(winmap_t *map);
leaf_error_t winmap_add_window(winmap_t *map, xcb_window_t id, window_t **win);
void winmap_rm_window(winmap_t *map, xcb_window_t id);
window_t *winmap_get_window(winmap_t *map, xcb_window_t id);

#endif /* !WINMAP_H */
