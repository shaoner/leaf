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

#ifndef CYLON_H
# define CYLON_H

# include <stdlib.h>
# include <stdbool.h>
# include <xcb/xcb.h>

# include "utils.h"

/*
 * Program info
 */
# define PROG_NAME "leaf"
# ifdef VERSION
#  define PROG_VERSION _stringify(VERSION)
# else
#  error "version number is not defined"
# endif

/*
 * Global configuration
 */
struct glob_config_t {
    xcb_connection_t *conn;
    bool running;
    const xcb_setup_t *xsetup;
    xcb_screen_t *screen;
};

extern struct glob_config_t gconf;

#endif /* !CYLON_H */
