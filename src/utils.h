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

#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>

# if !defined(DEBUG) || DEBUG == 0
#  define DEBUG_LEVEL 0
#  undef DEBUG
# else
#  define DEBUG_LEVEL DEBUG
# endif /* !DEBUG */

/* Stringify a macro */
# define __stringify(s) #s
# define _stringify(s) __stringify(s)

/* Display */
# define ERROR   (0)
# define WARNING (0)
# define DEBUG1  (1)
# define DEBUG2  (2)
# define DEBUG3  (3)

# define log(level, fmt, ...)                                           \
    do {                                                                \
        if (level <= DEBUG_LEVEL)                                       \
            fprintf(stderr, "[" #level " "  __FILE__ ":"                \
                    _stringify(__LINE__) ": %s] " fmt "\n",             \
                    __func__, ## __VA_ARGS__);                          \
    } while (0)

# ifdef DEBUG
#  include <assert.h>
#  define d_assert(c) assert(c)
# else
#  define d_assert(c)
# endif /* !DEBUG */

#endif /* !UTILS_H */
