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

#define RG_TARGET_NAMESPACE mSocketConnectionFactory

static VALUE
rg_m_create_connection(G_GNUC_UNUSED VALUE self, VALUE socket)
{
        return GOBJ2RVAL_UNREF(g_socket_connection_factory_create_connection(RVAL2GSOCKET(socket)));
}

void
Init_gsocketconnectionfactory(VALUE glib)
{
        /* TODO: Perhaps just move this to SocketConnection? */
        VALUE RG_TARGET_NAMESPACE = rb_define_module_under(glib, "SocketConnectionFactory");

        RG_DEF_MODFUNC(create_connection, 1);
}
