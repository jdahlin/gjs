/* -*- mode: C; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
/*
 * Copyright (c) 2010  litl, LLC
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

#include <string.h>
#include <gjs/gjs.h>
#include <girepository.h>

#include "arg.h"
#include "override.h"

static GHashTable *arg_overrides_table = NULL;

JSBool
gjs_arg_override_register(const char *namespace,
                          const char *type_name,
                          GjsArgOverrideInfo *info)
{
    char *canonical_name;

    g_return_val_if_fail(info != NULL, JS_FALSE);
    g_return_val_if_fail(info->to_func != NULL, JS_FALSE);
    g_return_val_if_fail(info->from_func != NULL, JS_FALSE);

    canonical_name = g_strdup_printf("%s.%s", namespace, type_name);
    if (!arg_overrides_table) {
        // FIXME: look into hasing on GITypeInfo instead.
        arg_overrides_table = g_hash_table_new_full(
            g_str_hash, g_str_equal,
            (GDestroyNotify)g_free,
            NULL);
    }
    g_hash_table_insert(arg_overrides_table, canonical_name, info);
    return JS_TRUE;
}

static GjsArgOverrideInfo *
gjs_arg_override_lookup(GITypeInfo *type_info)
{
    const char *type_name;
    const char *namespace;
    char *canonical;
    GIBaseInfo *base_info;

    base_info = g_type_info_get_interface(type_info);
    if (!base_info)
        return NULL;
    namespace = g_base_info_get_namespace(base_info);
    type_name = g_base_info_get_name(base_info);
    g_base_info_unref(base_info);

    /* silly optimiazation, since this is rather frequent
     * CairoContext -> "Cairo.Context" hash
     */
    {
        int namespace_len = strlen(namespace);
        int type_name_len = strlen(type_name);
        int canonical_len = namespace_len + 1 + type_name_len;
        canonical = g_alloca(sizeof(char) * canonical_len + 1);
        memcpy(canonical, namespace, namespace_len);
        canonical[namespace_len] = '.';
        memcpy(canonical + namespace_len + 1, type_name, type_name_len);
        canonical[canonical_len] = '\0';
    }

    return (GjsArgOverrideInfo*)g_hash_table_lookup(arg_overrides_table, canonical);
}

JSBool
gjs_arg_override_convert_to_g_argument(JSContext      *context,
                                       jsval           value,
                                       GITypeInfo     *type_info,
                                       const char     *arg_name,
                                       GjsArgumentType argument_type,
                                       GITransfer      transfer,
                                       gboolean        may_be_null,
                                       GArgument      *arg)
{
    GjsArgOverrideInfo *override;

    if (!arg_overrides_table)
        return JS_FALSE;

    override = gjs_arg_override_lookup(type_info);
    if (!override)
        return JS_FALSE;

    if (!override->to_func(context, value, type_info, arg_name,
                           argument_type, transfer, may_be_null, arg))
        return JS_FALSE;

    return JS_TRUE;
}

JSBool
gjs_arg_override_convert_from_g_argument(JSContext  *context,
                                         jsval      *value_p,
                                         GITypeInfo *type_info,
                                         GArgument  *arg)
{
    GjsArgOverrideInfo *override;

    if (!arg_overrides_table)
        return JS_FALSE;

    override = gjs_arg_override_lookup(type_info);
    if (!override)
        return JS_FALSE;

    if (!override->from_func(context, value_p, type_info, arg))
        return JS_FALSE;

    return JS_TRUE;
}

JSBool
gjs_arg_override_release_g_argument(JSContext  *context,
                                    GITransfer  transfer,
                                    GITypeInfo *type_info,
                                    GArgument  *arg)
{
    GjsArgOverrideInfo *override;

    if (!arg_overrides_table)
        return JS_FALSE;

    override = gjs_arg_override_lookup(type_info);
    if (!override)
        return JS_FALSE;

    if (!override->release_func)
        return JS_TRUE;

    if (!override->release_func(context, transfer, type_info, arg))
        return JS_FALSE;

    return JS_TRUE;
}

