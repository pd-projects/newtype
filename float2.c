/* Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

#include "m_pd.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static t_class *float2_class;

typedef struct _float2
{
    t_object x_obj;
    t_float x_f;
} t_float2;

static void *float2_new(t_pd *dummy, t_float f)
{
    t_float2 *x = (t_float2 *)pd_new(float2_class);
    x->x_f = f;
    outlet_new(&x->x_obj, &s_float);
    floatinlet_new(&x->x_obj, &x->x_f);
    return (x);
}

static void *float2_new2(t_floatarg f)
{
    return (float2_new(0, f));
}

static void float2_bang(t_float2 *x)
{
    outlet_float(x->x_obj.ob_outlet, x->x_f);
}

static void float2_float(t_float2 *x, t_float f)
{
    outlet_float(x->x_obj.ob_outlet, x->x_f = f);
}

static void float2_symbol(t_float2 *x, t_symbol *s)
{
    x->x_f = atof(s->s_name);
    outlet_float(x->x_obj.ob_outlet, x->x_f);
}

void float2_setup(void)
{
    float2_class = class_new(gensym("float2"), (t_newmethod)float2_new, 0,
        sizeof(t_float2), 0, A_GIMME, 0);
    class_addbang(float2_class, float2_bang);
    class_addfloat(float2_class, (t_method)float2_float);
    class_addsymbol(float2_class, (t_method)float2_symbol);
}
