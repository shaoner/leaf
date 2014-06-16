/*
 *
 * Copyright 2014, Alexandre Laurent, Ayman Khamouma
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

#include "leaf.h"
#include "window.h"
#include "event.h"
#include "error.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <xcb/xcb.h>
#include <xcb/xcb_event.h>

struct glob_config_t gconf;

/*
 * Forward decl.
 */
static void display_usage(FILE *stream);
static void display_version();
static leaf_error_t leaf_init();
static leaf_error_t leaf_run();
static int leaf_exit(int code);

static void display_usage(FILE *stream)
{
    fprintf(stream, "Usage: %s [ options ]\n", PROG_NAME);
    fprintf(stream, "  -v, --version\n");
    fprintf(stream, "\tprint the version information\n");
    fprintf(stream, "  -h, --help\n");
    fprintf(stream, "\tprint this help\n");
}

static void display_version()
{
    fprintf(stdout, "%s %s - %s %s\n",
            PROG_NAME, PROG_VERSION, __DATE__, __TIME__);
}

static void stop_signal_handler(int signal)
{
    switch(signal) {
    case SIGTERM:
    case SIGINT:
        print_d("Receive signal %i, stop %s", signal, PROG_NAME);
        gconf.running = false;
        break;
    }
}

static leaf_error_t leaf_init()
{
    int first_screen = 0;
    xcb_screen_iterator_t iter;
    struct sigaction action;
    uint32_t values;
    xcb_void_cookie_t cookie;
    xcb_generic_error_t *status = NULL;

    gconf.running = false;
    action.sa_handler = stop_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);

    /* Catch signals */
    /*
     * TODO: Catch sighup
     */
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    /* Initialize connection to the X server */
    gconf.conn = xcb_connect(NULL, &first_screen);
    if (xcb_connection_has_error(gconf.conn)) {
        print_e("Cannot initialize connection to the X server");
        return ERR_CONN;
    }
    print_d("First screen = %i", first_screen);
    /* Get back X data */
    gconf.xsetup = xcb_get_setup(gconf.conn);
    /* Find the first screen */
    iter = xcb_setup_roots_iterator(gconf.xsetup);
    while (first_screen--)
        xcb_screen_next(&iter);
    gconf.screen = iter.data;

    /* Configure the root screen to get window's events */
    values = \
        XCB_EVENT_MASK_STRUCTURE_NOTIFY |
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
    cookie = xcb_change_window_attributes_checked(gconf.conn,
                                                  gconf.screen->root,
                                                  XCB_CW_EVENT_MASK,
                                                  &values);
    status = xcb_request_check(gconf.conn, cookie);
    xcb_flush(gconf.conn);

    if (status)
    {
        print_d("X is busy, another window manager may be running (%d)",
                status->error_code);
        free(status);
        return ERR_CONN;
    }

    return ERR_NONE;
}

static leaf_error_t leaf_run()
{
    xcb_generic_event_t *event = NULL;
    int etype;
    leaf_error_t status = ERR_NONE;

    /* Start main loop */
    gconf.running = true;
    do {
        event = xcb_poll_for_event(gconf.conn);
        if (!event) {
            if (xcb_connection_has_error(gconf.conn)) {
                print_e("X connection is lost, exiting");
                return ERR_CONN;
            }
            continue;
        }
        etype = XCB_EVENT_RESPONSE_TYPE(event);

        /* Only for debug prupose */
        print_event(etype);

        /* Execute specific event handler */
        if (etype < MAX_EVENTS && geventhandlers[etype])
            status = geventhandlers[etype](event);
        free(event);
    } while (gconf.running && status == ERR_NONE);

    return status;
}

static int leaf_exit(int code)
{
    print_d("");
    gconf.running = false;
    /* Close connection to X and release memory */
    if (gconf.conn)
        xcb_disconnect(gconf.conn);
    if (code)
        print_e("exit with error %i", code);
    return code;
}

int main(int argc, char *argv[])
{
    struct option longopts[] = {
        { "version", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { 0, 0, 0, 0 }
    };
    int c = 0;

    /* Parse command-line options */
    while ((c = getopt_long(argc, argv, "vh", longopts, NULL)) > -1) {
        switch (c) {
        case 'h':
            display_usage(stdout);
            return EXIT_SUCCESS;
        case 'v':
            display_version();
            return EXIT_SUCCESS;
        default:
            /* Unknown option */
            display_usage(stderr);
            return EXIT_FAILURE;
        }
    }
    /* Start the wm */
    if (leaf_init() != ERR_NONE)
        return leaf_exit(EXIT_FAILURE);
    if (leaf_run() != ERR_NONE)
        return leaf_exit(EXIT_FAILURE);

    return leaf_exit(EXIT_SUCCESS);
}
