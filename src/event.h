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

#ifndef EVENT_H
# define EVENT_H

# include <xcb/xcb.h>

typedef enum {
	EVENT_ERR_NONE
} event_error_t;

typedef event_error_t (*event_handler_t)(xcb_generic_event_t*);

#define MAX_EVENTS XCB_MAPPING_NOTIFY + 1
extern event_handler_t geventhandlers[MAX_EVENTS];


# ifndef NDEBUG
void event_print(int type);
#  define print_event(type) event_print(type)
# else /* NDEBUG */
#  define print_event(type)
# endif /* !NDEBUG */

#endif /* !EVENT_H */
