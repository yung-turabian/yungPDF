#include "yungPDF_stdinc.h"
#include "yungPDF_objects.h"

void endobj(PDF* pdf);

void beginobj(PDF* pdf, i8 ref);

char* genRef(i8 ref); 

void updateOffsets(PDF* pdf); 
