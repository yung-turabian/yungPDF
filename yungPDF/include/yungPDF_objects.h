#ifndef YUNGPDFOBJS_H
#define YUNGPDFOBJS_H

#define MAX_KIDS 5
#define MAX_REFERENCE_SIZE 9 // 10000 0 R

#include "yungPDF_stdinc.h"

//
// PDF data types
//

typedef int8_t i8;

typedef const char* IndirectReference;

typedef union {
		u8 x1;
		u8 y1;
		u8 x2;
		u8 y2;
} Rectangle;

//
// PDF Objects
//

// fopen, or print to string
typedef struct PDFStream {

} PDFStream;

struct PageNode{
		u8 Offset;
		const char* Type;
		i8 Reference;


		struct PageNode* Kids[MAX_KIDS];
		struct PageNode* Parent; // -1 if root
		IndirectReference Resources;
		IndirectReference Contents;

		// Shared, holds dominace if absent in page 
		Rectangle MediaBox;	
		Rectangle CropBox;
		u8 Rotation;

		// Number of pages that are related
		i8 Count; // -1 if Page

		//struct PageNode* Kids[MAX_KIDS];
		u8 NumberOfKids;
} ; // Either root or an intermediate
typedef struct PageNode PageNode;

typedef struct DocumentCatalog {
		u8 Offset;
		const char* Type;
		i8 Reference;
		
		PageNode* RootNode;
		// /PageLabels numbertree
		// /Names dictionary for names
		// /Dests dictionary
		// /ViewerPreferences dict
		// /PageLayour name
		// /PageMode name
		// /Outlines dict
		// /Metadata ref

} DocumentCatalog;

typedef struct XRef {
		int offset;
} XRef;

typedef struct {
		char* FTPHeader;
		float Version;

		int Width;
		int Height;
		int ObjCount;
		size_t TotalPageCount;

		SMap* references;

		u32 *Offsets;
		u32 CurrentOffset;	

		FILE* stream;

		PageNode* PageTreeRoot;
		DocumentCatalog* DocumentCatalog;
		XRef XRef;

		struct Trailer {
				const char *title;
				const char *author;
				const char *keywords;
				const char *subject;
		} trailer;
} PDF;

typedef struct {
	const char* name;	
} Font;

#endif
