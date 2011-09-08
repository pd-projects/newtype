/* Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

/* connective objects */

#include "m_pd.h"

#include <string.h>
#include <stdio.h>

static t_class *symbol2_class;

static t_symbol* symbol2_sym;

typedef struct _symbol2
{
    t_object x_obj;
    t_symbol *x_s;
} t_symbol2;

static int istypeselector(t_symbol* s)
{
    if(s == &s_pointer || s == &s_float || s == &s_symbol ||
       s == &s_bang || s == &s_list || s == symbol2_sym)
        return 1;
    else
        return 0;
 }

static t_symbol* setassymbol(t_symbol *sel, int argc, t_atom *argv)
{
    t_symbol* ret = &s_;
    if (argc == 0)
    {
        if(! istypeselector(sel))
            ret = sel;
    }
    else if (argc == 1) 
    {
        if (argv->a_type == A_FLOAT)
        {
            char buf[MAXPDSTRING];
            t_float f = atom_getfloatarg(0, argc, argv);
            snprintf(buf, MAXPDSTRING, "%g", f);
            ret = gensym(buf);
        }
        else
            ret = atom_getsymbolarg(0, argc, argv);
    }
    else
    {
        int bufsize;
        char *buf;
        t_atom a;
        t_binbuf *bb = binbuf_new();
        /* if starts with a float, leave out the 'list' selector */
        if(! istypeselector(sel))
        {
            SETSYMBOL(&a, sel);
            binbuf_add(bb, 1, &a);
        }
        binbuf_add(bb, argc, argv);
        binbuf_gettext(bb, &buf, &bufsize);
        buf = resizebytes(buf, bufsize, bufsize+1);
        buf[bufsize] = 0;
        ret = gensym(buf);
        freebytes(buf, bufsize+1);
        binbuf_free(bb);
    }
    return ret;
}

static void *symbol2_new(t_symbol *sel, int argc, t_atom *argv) 
{
    t_symbol2 *x = (t_symbol2 *)pd_new(symbol2_class);
    outlet_new(&x->x_obj, &s_symbol);
    symbolinlet_new(&x->x_obj, &x->x_s);
    x->x_s = setassymbol(sel, argc, argv);
    return (x);
}

static void symbol2_bang(t_symbol2 *x)
{
    outlet_symbol(x->x_obj.ob_outlet, x->x_s);
}

static void symbol2_symbol(t_symbol2 *x, t_symbol *s)
{
    outlet_symbol(x->x_obj.ob_outlet, x->x_s = s);
}

static void symbol2_anything(t_symbol2 *x, t_symbol *s, int argc, t_atom *argv)
{
    x->x_s = setassymbol(s, argc, argv);
    outlet_symbol(x->x_obj.ob_outlet, x->x_s);
}

void symbol2_setup(void)
{
    symbol2_sym = gensym("symbol2");
    symbol2_class = class_new(symbol2_sym, (t_newmethod)symbol2_new, 0,
        sizeof(t_symbol2), 0, A_GIMME, 0);
    class_addbang(symbol2_class, symbol2_bang);
    class_addsymbol(symbol2_class, symbol2_symbol);
    class_addanything(symbol2_class, symbol2_anything);
}
