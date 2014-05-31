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

/*
 * Utils
 */

/* Stringify a macro */
# define __stringify(s) #s
# define _stringify(s) __stringify(s)

/* General print */
# ifdef NDEBUG
#  define print_d(fmt, args...)
# else
#  define print_d(fmt, args...)	  \
	fprintf(stderr, "[debug " __FILE__ ":" _stringify(__LINE__) " %s] " \
	        fmt "\n", __FUNCTION__, ## args)
# endif /* !NDEBUG */

# define print_e(fmt, args...)	  \
	fprintf(stderr, "[error] " fmt "\n", ## args)

# define print_w(fmt, args...)	  \
	fprintf(stderr, "[warn] " fmt "\n", ## args)

#endif /* !UTILS_H */
