#ifndef YUNGPDFFUNCS_H
#define YUNGPDFFUNCS_H

#include "yungPDF_stdinc.h"
#include "yungPDF_objects.h"

//
// Internal functions
//
PageNode* _initPagesTree(PDF* pdf);
DocumentCatalog* _initDocumentCatalog(PDF* pdf);

void pdfAddChildPagesNode(PDF* pdf, 
				PageNode* parentNode, PageNode* childNode);

PageNode* pdfCreatePageNode(PDF* pdf, PageNode* root);

void pdfWrite(PDF* pdf);

// WIP
void pdfCreatePage(PDF* pdf, PageNode* node);

void DFSTraverse(PDF* pdf, PageNode* node);

void pdfWritePageNode(PDF* pdf, PageNode* node);
void pdfWritePage(PDF* pdf, PageNode* page);

#endif