#include "../include/yungPDF_objects.h"

#include "../include/yungPDF_util.h"
#include "../include/yungPDF_functions.h"
#include <stdio.h>

PageNode* _initPagesTree(PDF* pdf) 
{
		if(getIndex(pdf->references, "PagesRoot") != -1) return NULL;
		
		PageNode* pages_root;
		pages_root = (PageNode*)malloc(sizeof(PageNode));
		if(pages_root == NULL)
		{
			fprintf(stdout, "[AVL+Tree] memory can't be allocated\n");
			return NULL;
		}

		pages_root->Type = "/Pages";
		pages_root->Reference = ++pdf->ObjCount;
		pages_root->Count = 0;
		pages_root->NumberOfKids = 0;
		pages_root->Parent = NULL;

		pdf->PageTreeRoot = pages_root;
		
		return pages_root;
}

DocumentCatalog* _initDocumentCatalog(PDF* pdf) 
{
	if(getIndex(pdf->references, "DocumentCatalog") != -1) return NULL;

	DocumentCatalog* catalog;
	catalog = (DocumentCatalog*)malloc(sizeof(DocumentCatalog));

	if(pdf->PageTreeRoot == NULL)
	{
		fprintf(stdout, "[yungpdf] PagesRoot not created yet, doing so now...\n");
		_initPagesTree(pdf);
	}
	catalog->RootNode = pdf->PageTreeRoot;
	
	catalog->Reference = ++pdf->ObjCount;
	pdf->DocumentCatalog = catalog;


	beginobj(pdf, catalog->Reference);
	//insert(pdf->references, "DocumentCatalog", genRef(pdf));
	char *toWrite = 
			"<< "
			"/Type %s "
			"/Pages %s "
			"/PageLayout /SinglePage >>";
	fprintf(pdf->stream, toWrite, "/Catalog", genRef(pdf->PageTreeRoot->Reference));
	endobj(pdf);
	updateOffsets(pdf);
	return catalog;
}

void pdfWrite(PDF* pdf)
{
	if(pdf == NULL) return;

	pdfWritePageNode(pdf, pdf->PageTreeRoot);
}

void pdfAddChildPagesNode(PDF* pdf, PageNode* parentNode, PageNode* childNode)
{
	
	if(strcmp(parentNode->Type, "/Pages") == 0 && parentNode->NumberOfKids < MAX_KIDS)
	{
		parentNode->Kids[parentNode->NumberOfKids++] = childNode;
	} 
	else 
	{
		fprintf(stdout, "[yungpdf] PagesNode @%d 0 R (sz: %d) is full!\n", 
		parentNode->Reference, parentNode->NumberOfKids);
	}
}

PageNode* pdfCreatePageNode(PDF* pdf, PageNode* root)
{
		PageNode* node;
		node = (PageNode*)malloc(sizeof(PageNode));
		if(node == NULL)
		{
			fprintf(stdout, "[AVL+Tree] memory can't be allocated\n");
			return NULL;
		}
			
		node->Type = "/Pages";
		node->Reference = ++pdf->ObjCount;
		node->Count = 0;
		node->NumberOfKids = 0;
		node->Parent = root;
		pdfAddChildPagesNode(pdf, root, node);

		return node;
}

// WIP
void pdfCreatePage(PDF* pdf, PageNode* node)
{
	PageNode* page = (PageNode*)malloc(sizeof(PageNode));

	page->Type = "/Page";
	page->Reference = ++pdf->ObjCount;
	page->NumberOfKids = -1;
	page->Count = -1;
	page->Parent = node;

	node->Kids[node->NumberOfKids++] = page;
	node->Count++;

	// Adjust count up the tree
	PageNode* tmp = node;
	while(tmp->Parent != NULL) { 
			tmp = tmp->Parent;
			tmp->Count++;
	}
}

void DFSTraverse(PDF* pdf, PageNode* node)
{
	if(node==NULL) return;

	if(strcmp(node->Type, "/Page") == 0)
	{
        pdfWritePage(pdf, node);
	}
	else 
	{
		pdfWritePageNode(pdf, node);

		for(int i = 0; i < node->Count; i++)
		{
			DFSTraverse(pdf, node->Kids[i]);
		}
	
	}
}

void pdfWritePageNode(PDF* pdf, PageNode* node) 
{
	beginobj(pdf, node->Reference);

	// for loop for refs
	fprintf(pdf->stream, "<< /Type %s ",
			node->Type);
	
	
	if(node->Kids[0] != NULL) {

			int i;
			char buf[((sizeof(char) * MAX_REFERENCE_SIZE) * node->NumberOfKids) + 2];
			int pos = 0;
			pos += sprintf(&buf[pos], " ");
			for(i=0;i<node->NumberOfKids;i++) {
				pos += sprintf(&buf[pos], "%s", genRef(node->Kids[i]->Reference));
				pos += sprintf(&buf[pos], " ");
			}

			fprintf(pdf->stream, "/Kids [%s] ",
					buf);
	}
	fprintf(pdf->stream, "/Count %d ",
					node->Count);
	if(node->Parent != NULL) {
			fprintf(pdf->stream, "/Parent %s ",
							genRef(node->Parent->Reference));
	}
	fprintf(pdf->stream, ">>");
	endobj(pdf);
}

void pdfWritePage(PDF* pdf, PageNode* page)
{
	beginobj(pdf, page->Reference);

	// for loop for refs
	fprintf(pdf->stream, "<< /Type %s ",
			page->Type);
	
	
	if(page->Parent != NULL) {
			fprintf(pdf->stream, "/Parent %s ",
							genRef(page->Parent->Reference));
	}
	fprintf(pdf->stream, ">>");
	endobj(pdf);
}
