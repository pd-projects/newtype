
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void main(void)
{
    char buf[256];
    float f;
    int i, len, isfloat;
    
    char *strs[] = {
        "-345",
        "0.00002",
        "0.000000000003",
        "5e12",
        "10.000000000003",
        "0",
        "0.0",
        "0.00000000000",
        "000000000000",
        ".0",
        ".000000000000000",
        "",
        "0.0.0",
        ".0000000000a00000",
        "0x00000",
        "0x01010",
        "+123",
        "fourty4",
    };

    for (i=0; i< 18; i++) {
        isfloat = 1;
        f = atof(strs[i]);
        printf("original %s  float: %g\n", strs[i], f);
/*
        sprintf(buf, "%g", f);
        printf("char: %s\n", buf);
        f = atof(buf);
        printf("float: %g\n", f);
*/
        len = strlen(strs[i]);
        if (len == 0) {
            isfloat = 0;
            printf("zero length\n");
        } else if(strs[i][0] == '+') {
            isfloat = 0;
            printf("start with +\n");
        } else if(strs[i][0] == '0' && strs[i][1] == 'x') {
            /* pd doesn't support hex 0x format numbers */
            isfloat = 0;
            printf("hex not supported\n");
        } else if (f == 0) {
            if(len == 1 && strs[i][0] != '0') {
                isfloat = 0;
                printf("one length != 0\n");
            } else if (len > 1) {
                if((strs[i][0] != '0' && strs[i][0] != '.' && strs[i][0] != '-') && 
                   strs[i][1] != '0') {
                    isfloat = 0;
                    printf("> 1 but not .0 or -0\n");
                } else {
                    /* we already checked positions 0 and 1 so skip */
                    //printf("teststr: '%s' len: %d\n", strs[i], len);
                    char* str = &strs[i][2];
                    int founddot = 0;
                    while (*str != '\0') {
                        //printf("test:%c ", *str);
                        if(*str == '.')
                            founddot++; 
                        if((*str != '0' && *str != '.') || founddot) {
                            isfloat = 0;
                            printf("found non-0 or non-.\n");
                            break;
                        }
                        str++;
                    }
                    //printf("\n");
                }
            }
        }
//        if(!isfloat)
//            printf("THIS IS NOT\n");
        printf("------------\n");
    }
}
