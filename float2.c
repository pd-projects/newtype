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

static void *float2_new(t_floatarg f)
{
    t_float2 *x = (t_float2 *)pd_new(float2_class);
    x->x_f = f;
    outlet_new(&x->x_obj, &s_float);
    floatinlet_new(&x->x_obj, &x->x_f);
    return (x);
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
    int isfloat = 1;
    int len = strlen(s->s_name);
    char buf[MAXPDSTRING];
    t_float f = atof(s->s_name);
    if (len == 0) {
        isfloat = 0;
        sprintf(buf, "zero length symbol");
    } else if(s->s_name[0] == '+') {
        /* pd doesn't consider things starting with '+' as float */
        isfloat = 0;
        sprintf(buf, "starts with +");
    } else if(s->s_name[0] == '0' && s->s_name[1] == 'x') {
        /* pd doesn't support hex 0x format numbers as floats */
        isfloat = 0;
        sprintf(buf, "0x hex not supported");
    } else if (f == 0) {
        if(len == 1 && s->s_name[0] != '0') {
            isfloat = 0;
            sprintf(buf, "length == 1 and != '0'");
        } else if (len > 1) {
            if((s->s_name[0] != '0' && s->s_name[0] != '.' && s->s_name[0] != '-') && 
               s->s_name[1] != '0') {
                isfloat = 0;
                sprintf(buf, "length > 1 but doesn't start with '.0' or '-0'");
            } else {
                /* we already checked positions 0 and 1 so skip */
                char* str = &s->s_name[2];
                int founddot = 0;
                while (*str != '\0') {
                    if(*str == '.')
                        founddot++; 
                    if((*str != '0' && *str != '.') || founddot) {
                        isfloat = 0;
                        sprintf(buf, "== 0 and found characters besides '0' or '.'");
                        break;
                    }
                    str++;
                }
            }
        }
    }
    if(isfloat)
    {
        x->x_f = f;
        outlet_float(x->x_obj.ob_outlet, x->x_f);
    }
    else
        pd_error(x,"cannot convert '%s' to a float!\n\t(%s)", s->s_name, buf);
}

void float2_setup(void)
{
    float2_class = class_new(gensym("float2"), (t_newmethod)float2_new, 0,
        sizeof(t_float2), 0, A_GIMME, 0);
    class_addbang(float2_class, float2_bang);
    class_addfloat(float2_class, (t_method)float2_float);
    class_addsymbol(float2_class, (t_method)float2_symbol);
}
