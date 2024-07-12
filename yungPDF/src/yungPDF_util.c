#include "../include/yungPDF_util.h"

void endobj(PDF* pdf)
{
		fprintf(pdf->stream, "\nendobj\n");
}

void beginobj(PDF* pdf, i8 ref)
{
		pdf->Offsets[pdf->CurrentOffset++] = ftell(pdf->stream);
		fprintf(pdf->stream, "\n%d 0 obj\n", ref);
}

char* genRef(i8 ref) 
{
		char* buf;
		buf = malloc(sizeof(int));
		sprintf(buf, "%d", ref);
		return strncat(buf, " 0 R", 4);
}

void updateOffsets(PDF* pdf) {
		pdf->Offsets = (u32*)realloc(pdf->Offsets, pdf->ObjCount * sizeof(u32));
}

