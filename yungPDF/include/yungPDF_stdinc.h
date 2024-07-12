


/**
 *	\file yungPDF_stdinc.h
 *
 *	General header that includes C language
 */

#ifndef YUNGPDFSTDINC_H
#define YUNGPDFSTDINC_H

#include <xtra.h>
#include "../../map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG(...)\
if(verbose && SHOW_ERROR) {\
fprintf(stdout, "[yungpdf] Err: %s, %d",__FUNCTION__, __LINE__);\
printf(__VA_ARGS__);\
}\
else if (verbose && SHOW_WARNING) {\
fprintf(stdout, "[yungpdf] Warning: %s, %d",__FUNCTION__, __LINE__);\
printf(__VA_ARGS__);\
}\

#endif
