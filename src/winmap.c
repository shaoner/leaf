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

#include "winmap.h"
#include "utils.h"
#include "error.h"
#include "window.h"

# include <stdlib.h>

#include <xcb/xcb.h>

static inline uint32_t winmap_hash(uint32_t id)
{
    return id;
}

leaf_error_t winmap_new(winmap_t **newmap)
{
    d_assert(newmap);
    d_assert(*newmap == NULL);

    winmap_t *map = NULL;

    if (!(map = malloc(sizeof(winmap_t))))
        return ERR_NOMEM;

    if (!(map->table = calloc(WIN_TABLE_SIZE, sizeof(winlist_t *)))) {
        free(map);
        return ERR_NOMEM;
    }
    *newmap = map;
    return ERR_NONE;
}

void winmap_delete(winmap_t *map)
{
    d_assert(map);

    winlist_t *list = NULL;
    winlist_t *node = NULL;

    for (uint32_t i = 0; i < WIN_TABLE_SIZE; ++i) {
        list = map->table[i];
        while (list) {
            node = list;
            list = list->next;
            free(node->win);
            free(node);
        }
    }
    free(map->table);
    map->table = NULL;
    free(map);
}

leaf_error_t winmap_add_window(winmap_t *map, xcb_window_t id, window_t **win)
{
    d_assert(map);
    d_assert(win);
    d_assert(*win == NULL);

    uint32_t idx = 0;
    winlist_t *node = NULL;

    /* Check the window is not already in the hash table */
    if (winmap_get_window(map, id))
        return ERR_BADARG;

    if (!(*win = malloc(sizeof(window_t))))
        return ERR_NOMEM;

    /* Add the window to the hash table */
    if (!(node = malloc(sizeof(winlist_t)))) {
        free(win);
        return ERR_NOMEM;
    }

    idx = winmap_hash(id) % WIN_TABLE_SIZE;
    (*win)->id = id;
    node->win = *win;
    node->next = map->table[idx];
    map->table[idx] = node;
    return ERR_NONE;
}

void winmap_rm_window(winmap_t *map, xcb_window_t id)
{
    d_assert(map);

    winlist_t *prev = NULL;
    winlist_t *list = NULL;
    window_t *win = NULL;
    uint32_t idx = winmap_hash(id) % WIN_TABLE_SIZE;

    for (list = map->table[idx]; list != NULL; prev = list, list = list->next) {
        if (list->win->id == id) {
            /*
             * The window has been found
             * Relink the list in the hash table
             */
            if (!prev)
                map->table[idx] = list->next;
            else
                prev->next = list->next;
            win = list->win;
            free(win);
            free(list);
            return;
        }
    }
    /*
     * If the window cannot be found
     * - the id is not valid
     * - or the window was mapped before the wm
     * In both cases, nothing to do
     */
}

window_t *winmap_get_window(winmap_t *map, xcb_window_t id)
{
    d_assert(map);

    uint32_t idx = winmap_hash(id) % WIN_TABLE_SIZE;

    for (winlist_t *list = map->table[idx]; list != NULL; list = list->next)
        if (list->win->id == id)
            return list->win;
    return NULL;
}
