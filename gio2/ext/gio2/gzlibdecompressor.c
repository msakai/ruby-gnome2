/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "gio2.h"

#define RG_TARGET_NAMESPACE cZlibDecompressor
#define _SELF(value) G_ZLIB_DECOMPRESSOR(RVAL2GOBJ(value))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE format;

        rb_scan_args(argc, argv, "01", &format);
        G_INITIALIZE(self,
                     g_zlib_decompressor_new(RVAL2GZLIBCOMPRESSORFORMATDEFAULT(format)));

        return Qnil;
}

void
Init_gzlibdecompressor(VALUE glib)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_ZLIB_DECOMPRESSOR, "ZlibDecompressor", glib);

        RG_DEF_METHOD(initialize, -1);
}
