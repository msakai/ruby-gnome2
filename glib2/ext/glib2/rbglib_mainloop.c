/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005,2006 Masao Mutoh
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

#include "rbgprivate.h"

/*****************************************/
GType
g_main_loop_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GMainLoop",
                    (GBoxedCopyFunc)g_main_loop_ref,
                    (GBoxedFreeFunc)g_main_loop_unref);
  return our_type;
}
/*****************************************/

#define RG_TARGET_NAMESPACE cMainLoop
#define _SELF(s) ((GMainLoop*)RVAL2BOXED(s, G_TYPE_MAIN_LOOP))

/*****************************************/

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE context, is_running;
    GMainLoop *loop;
    GMainContext *main_context = NULL;

    rb_scan_args(argc, argv, "02", &context, &is_running);

    if (!NIL_P(context))
        main_context = RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT);
    loop = g_main_loop_new(main_context, RVAL2CBOOL(is_running));
    G_INITIALIZE(self, loop);
    return Qnil;
}

/* just for ruby-1.9.0. */
#if !defined(RUBY_UBF_IO) && defined(RB_UBF_DFL)
#  define RUBY_UBF_IO RB_UBF_DFL
#endif

#ifdef HAVE_RB_THREAD_CALL_WITH_GVL
static VALUE
rg_run_without_gvl(void *arg) {
    g_main_loop_run((GMainLoop*)arg);
    return Qnil;
}
#endif

static VALUE
rg_run(VALUE self)
{
/* Note:
 * - rb_thread_call_with_gvl is necessary for callback funcsions to re-aquire GVL.
 * - HAVE_RB_THREAD_CALL_WITH_GVL impleis the existence of rb_thread_blocking_region.
 */
#ifdef HAVE_RB_THREAD_CALL_WITH_GVL
    rb_thread_blocking_region(rg_run_without_gvl, _SELF(self), RUBY_UBF_IO, NULL);    
#else
    g_main_loop_run(_SELF(self));
#endif
    return self;
}

static VALUE
rg_quit(VALUE self)
{
    g_main_loop_quit(_SELF(self));
    return Qnil;
}

static VALUE
rg_running_p(VALUE self)
{
    return CBOOL2RVAL(g_main_loop_is_running(_SELF(self)));
}

static VALUE
rg_context(VALUE self)
{
    return BOXED2RVAL(g_main_loop_get_context(_SELF(self)), G_TYPE_MAIN_CONTEXT);
}

void
Init_glib_main_loop(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_MAIN_LOOP, "MainLoop", mGLib);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(run, 0);
    RG_DEF_METHOD(quit, 0);
    RG_DEF_METHOD_P(running, 0);
    RG_DEF_METHOD(context, 0);
}
