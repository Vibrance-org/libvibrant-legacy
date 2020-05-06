/*
 * vibrant - Adjust color vibrance of X11 output
 * Copyright (C) 2020  Sefa Eyeoglu <contact@scrumplex.net> (https://scrumplex.net)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * vibrant is based on color-demo-app written by Leo (Sunpeng) Li <sunpeng.li@amd.com>
 *
 * Original license text of color-demo-app:
 *
 * Copyright 2018 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#include <stdio.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

/**
 * Find the output on the RandR screen resource by name.
 *
 * @param dpy The X Display
 * @param res The RandR screen resource
 * @param name The output name to search for
 * @return The RandR-Output X-ID if found, 0 (None) otherwise
 */
static RROutput find_output_by_name(Display *dpy, XRRScreenResources *res,
                                    const char *name) {
    int i, cmp;
    RROutput ret;
    XRROutputInfo *output_info;

    for (i = 0; i < res->noutput; i++) {
        ret = res->outputs[i];
        output_info = XRRGetOutputInfo(dpy, res, ret);

        cmp = strcmp(name, output_info->name);
        XRRFreeOutputInfo(output_info);

        if (!cmp) {
            return ret;
        }
    }
    return 0;
}

/**
 * Check if output has specified property.
 *
 * @param dpy The X Display
 * @param output RandR output to get the information from
 * @param property String containing name of property
 * @return 1 if it has a property, 0 if it doesn't or X doesn't support it
 */
static int output_has_property(Display *dpy, RROutput output, char *property) {
    Atom prop_atom;

    // Find the X Atom associated with the property name
    prop_atom = XInternAtom(dpy, property, 1);
    if (!prop_atom) {
        printf("Property key '%s' not found.\n", property);
        return 0;
    }

    // Make sure the property exists
    if (!XRRQueryOutputProperty(dpy, output, prop_atom)) {
        printf("Property key '%s' not found on output\n", property);
        return 0;
    }
    return 1;
}
