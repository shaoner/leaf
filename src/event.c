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

#include "event.h"
#include "utils.h"
#include "window.h"
#include "leaf.h"

#include <xcb/xcb.h>

static leaf_error_t event_create_notify(xcb_generic_event_t *ge)
{
#ifndef NDEBUG
    xcb_create_notify_event_t *e = (xcb_create_notify_event_t *)ge;
    print_d("New window 0x%08X (%i, %i) [%u, %u]",
            e->window, e->x, e->y, e->width, e->height);
#endif
    return ERR_NONE;
}

static leaf_error_t event_destroy_notify(xcb_generic_event_t *ge)
{
    print_d("");
    return ERR_NONE;
}

static leaf_error_t event_map_request(xcb_generic_event_t *ge)
{
    xcb_map_request_event_t *e = (xcb_map_request_event_t *)ge;

    print_d("");
    window_configure(e->window);
    return ERR_NONE;
}

static leaf_error_t event_configure_request(xcb_generic_event_t *ge)
{
    print_d("");
    return ERR_NONE;
}

event_handler_t geventhandlers[MAX_EVENTS] = {
    NULL,
    NULL,
    NULL, /* XCB_KEY_PRESS         */
    NULL, /* XCB_KEY_RELEASE       */
    NULL, /* XCB_BUTTON_PRESS      */
    NULL, /* XCB_BUTTON_RELEASE    */
    NULL, /* XCB_MOTION_NOTIFY     */
    NULL, /* XCB_ENTER_NOTIFY      */
    NULL, /* XCB_LEAVE_NOTIFY      */
    NULL, /* XCB_FOCUS_IN          */
    NULL, /* XCB_FOCUS_OUT         */
    NULL, /* XCB_KEYMAP_NOTIFY     */
    NULL, /* XCB_EXPOSE            */
    NULL, /* XCB_GRAPHICS_EXPOSURE */
    NULL, /* XCB_NO_EXPOSURE       */
    NULL, /* XCB_VISIBILITY_NOTIFY */
    event_create_notify,
    event_destroy_notify,
    NULL, /* XCB_UNMAP_NOTIFY      */
    NULL, /* XCB_MAP_NOTIFY        */
    event_map_request,
    NULL, /* XCB_REPARENT_NOTIFY   */
    NULL, /* XCB_CONFIGURE_NOTIFY  */
    event_configure_request,
    NULL, /* XCB_GRAVITY_NOTIFY    */
    NULL, /* XCB_RESIZE_REQUEST    */
    NULL, /* XCB_CIRCULATE_NOTIFY  */
    NULL, /* XCB_CIRCULATE_REQUEST */
    NULL, /* XCB_PROPERTY_NOTIFY   */
    NULL, /* XCB_SELECTION_CLEAR   */
    NULL, /* XCB_SELECTION_REQUEST */
    NULL, /* XCB_SELECTION_NOTIFY  */
    NULL, /* XCB_COLORMAP_NOTIFY   */
    NULL, /* XCB_CLIENT_MESSAGE    */
    NULL, /* XCB_MAPPING_NOTIFY    */
};

#ifndef NDEBUG
static const char *eventnames[MAX_EVENTS] = {
    "",
    "",
    "KeyPress",
    "KeyRelease",
    "ButtonPress",
    "ButtonRelease",
    "MotionNotify",
    "EnterNotify",
    "LeaveNotify",
    "FocusIn",
    "FocusOut",
    "KeymapNotify",
    "Expose",
    "GraphicsExpose",
    "NoExpose",
    "VisibilityNotify",
    "CreateNotify",
    "DestroyNotify",
    "UnmapNotify",
    "MapNotify",
    "MapRequest",
    "ReparentNotify",
    "ConfigureNotify",
    "ConfigureRequest",
    "GravityNotify",
    "ResizeRequest",
    "CirculateNotify",
    "CirculateRequest",
    "PropertyNotify",
    "SelectionClear",
    "SelectionRequest",
    "SelectionNotify",
    "ColormapNotify",
    "ClientMessage",
    "MappingNotify"
};

void event_print(int type)
{
    if (type < MAX_EVENTS)
        print_d("-> Event %i (%s)", type, eventnames[type]);
}
#endif
