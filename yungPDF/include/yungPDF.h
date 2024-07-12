/*
		yung-turabian's PDF Encoder




 */

/**
 *	\file yungpdf.h
 *
 *	Main include header for the library.
 */

#ifndef YUNGPDF_H
#define YUNGPDF_H

#include "yungPDF_stdinc.h"
#include "yungPDF_objects.h"
#include "yungPDF_functions.h"
#include "yungPDF_util.h"

PDF* pdfInit();

void writeHeader();
void writeXRef();
void writePDF();

void pdfAddPage(PDF* pdf, u8 rotation);

void pdfSetMetadata(PDF *pdf, 
				const char *title, const char *author, const char *subject, const char *keywords);
void pdfUpdatePages(PDF* pdf, const char* new_ref);
void pdfCreateFont(PDF* pdf, Font font);

void pdfCreateXRef(PDF* pdf);
void pdfCreateTrailer(PDF* pdf);

void pdfDestroy(PDF **pdf);

#endif
