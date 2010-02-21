/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
/* Copyright 2010 litl, LLC. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <config.h>

#include <gjs/gjs.h>
#include <gi/override.h>

#include <cairo.h>
#include "cairo-private.h"

typedef struct {
    void *dummy;
    JSContext  *context;
    JSObject   *object;
    cairo_t * cr;
} GjsCairoContext;

GJS_DEFINE_PROTO("CairoContext", gjs_cairo_context)

GJS_DEFINE_PRIV_FROM_JS(GjsCairoContext, gjs_cairo_context_class);

static void
_gjs_cairo_context_construct_internal(JSContext *context,
                                      JSObject *obj,
                                      cairo_t *cr)
{
    GjsCairoContext *priv;

    priv = g_slice_new0(GjsCairoContext);

    g_assert(priv_from_js(context, obj) == NULL);
    JS_SetPrivate(context, obj, priv);

    priv->context = context;
    priv->object = obj;
    priv->cr = cr;
}

static JSBool
gjs_cairo_context_constructor(JSContext *context,
                          JSObject  *obj,
                          uintN      argc,
                          jsval     *argv,
                          jsval     *retval)
{
    JSObject *surface_wrapper;
    cairo_surface_t *surface;
    cairo_t *cr;
    cairo_status_t status;

    if (!gjs_check_constructing(context))
        return JS_FALSE;

    if (!gjs_parse_args(context, "Context", "o", argc, argv,
                        "surface", &surface_wrapper))
        return JS_FALSE;

    surface = gjs_cairo_surface_get_surface(context, surface_wrapper);
    if (!surface) {
        gjs_throw(context, "first argument to Context() should be a surface");
        return JS_FALSE;
    }

    cr = cairo_create(surface);
    status = cairo_status(cr);
    if (status != CAIRO_STATUS_SUCCESS) {
        gjs_throw(context, "Could not create context: %s",
                  cairo_status_to_string(status));
        return JS_FALSE;
    }

    _gjs_cairo_context_construct_internal(context, obj, cr);

    return JS_TRUE;
}

static void
gjs_cairo_context_finalize(JSContext *context,
                           JSObject  *obj)
{
    GjsCairoContext *priv;
    priv = priv_from_js(context, obj);
    if (priv == NULL)
        return;

    cairo_destroy(priv->cr);
    g_slice_free(GjsCairoContext, priv);
}

/* Properties */
static JSPropertySpec gjs_cairo_context_proto_props[] = {
    { NULL }
};

/* Methods */
static JSFunctionSpec gjs_cairo_context_proto_funcs[] = {
    { NULL }
};

JSObject *
gjs_cairo_context_from_cr(JSContext *context,
                          cairo_t *cr)
{
    JSObject *object;

    object = JS_NewObject(context, &gjs_cairo_context_class, NULL, NULL);
    if (!object)
        return NULL;

    _gjs_cairo_context_construct_internal(context, object, cr);
    return object;
}

cairo_t *
gjs_cairo_context_get_cr(JSContext *context,
                         JSObject *object)
{
    GjsCairoContext *priv;
    priv = priv_from_js(context, object);
    if (priv == NULL)
        return NULL;

    return priv->cr;
}

static JSBool
context_to_g_argument(JSContext      *context,
                      jsval           value,
                      GITypeInfo     *type_info,
                      const char     *arg_name,
                      GjsArgumentType argument_type,
                      GITransfer      transfer,
                      gboolean        may_be_null,
                      GArgument      *arg)
{
    JSObject *obj;
    cairo_t *cr;

    obj = JSVAL_TO_OBJECT(value);
    cr = gjs_cairo_context_get_cr(context, obj);
    if (!cr)
        return JS_FALSE;

    arg->v_pointer = cr;
    return JS_TRUE;
}

static JSBool
context_from_g_argument(JSContext  *context,
                        jsval      *value_p,
                        GITypeInfo *type_info,
                        GArgument  *arg)
{
    JSObject *obj;

    obj = gjs_cairo_context_from_cr(context, (cairo_t*)arg->v_pointer);
    if (!obj)
        return JS_FALSE;

    *value_p = OBJECT_TO_JSVAL(obj);
    return JS_TRUE;
}

static GjsArgOverrideInfo override_info = {
    context_to_g_argument,
    context_from_g_argument,
    NULL
};

void
gjs_cairo_context_init(JSContext *context)
{
    gjs_arg_override_register("cairo", "Context", &override_info);
}
