#include "../include/yungPDF.h"

/*	
 *	Create pdf stream object to either print to stdout, file
 *	option to store in memory or tmpfile
 * 	
 *
 *
 *
 *
 *
 *
 */

PDF* pdfInit() {
		PDF* pdf = (PDF *)malloc(sizeof(PDF));
		if(pdf == NULL) {printf("[yung_pdf] failure to allocate mem for pdf-obj\n");return NULL;}

		// defaults
		pdf->FTPHeader = "%¥±ë";
		pdf->Version = 1.5;
		pdf->Width = 816; //8.5in
		pdf->Height = 1056; //11in
		pdf->ObjCount = 0;
		pdf->TotalPageCount = 0;
		pdf->Offsets = (u32*)malloc(pdf->ObjCount * sizeof(u32));
		if(pdf->Offsets == NULL) 
		{
				printf("[yung_pdf] failure to allocate mem for offsets\n");
				free(pdf);
				return NULL;
		}
		pdf->CurrentOffset = 0;
		pdf->references = createSMap();

		// make it hidden when more confident in structure
		pdf->stream = fopen("test.pdf", "w");
		if(pdf->stream==NULL) {
				printf("[yung_pdf ~io] file can't be written\n");
				free(pdf->Offsets);
				free(pdf);
				return NULL;
		}

		fprintf(pdf->stream, "\%%PDF->%.6g\n%s\n", pdf->Version, pdf->FTPHeader);

		PageNode* root = _initPagesTree(pdf);
		DocumentCatalog* catalog = _initDocumentCatalog(pdf);

		PageNode* node1 = pdfCreatePageNode(pdf, root);
		PageNode* node2 = pdfCreatePageNode(pdf, root);

		pdfCreatePage(pdf, root);
		pdfCreatePage(pdf, node1);
		pdfCreatePage(pdf, node1);
		pdfCreatePage(pdf, node1);

		pdfCreatePage(pdf, node2);
		pdfCreatePage(pdf, node2);
		pdfCreatePage(pdf, node2);

		DFSTraverse(pdf, root);

		
		//pdfWrite(pdf);


		//testing
		Font font;
		font.name = "FreeSerif";

		//pdfCreateFont(pdf, font);


		/*
		// Page1
		pdf->Offsets[pdf->CurrentOffset] = ftell(pdf->stream);
		const char* ref = "3 0 R";
		fprintf(pdf->stream, "\n%u 0 obj\n", pdf->CurrentOffset);
		fprintf(pdf->stream, "<< /Type /Page /Parent %u 0 R /Contents %u 0 R /Resources << /Font << /F1 1 0 R  >> >> >>\n", pdf->pages.Offset, pdf->CurrentOffset+1);
		fprintf(pdf->stream, "endobj\n");
		pdf->CurrentOffset++;

		pdfUpdatePages(pdf, ref);

		// Page1 stream
		pdf->Offsets[pdf->CurrentOffset] = ftell(pdf->stream);
		fprintf(pdf->stream, "\n%u 0 obj\n", pdf->CurrentOffset);
		fprintf(pdf->stream, "<< /Length 44 >>\n"); //len of content stream
		fprintf(pdf->stream, "stream\n");
		fprintf(pdf->stream, "endstream\n");
		fprintf(pdf->stream, "endobj\n");
		*/
		//pdfCreateContentStream(pdf);


		pdfSetMetadata(pdf, "test", "Henry", "Testing", "idk");
		pdfCreateXRef(pdf);
		pdfCreateTrailer(pdf);

		//pdfFlush(pdf);

		printmap(pdf->references);
		return pdf;
}

void pdfCreateContentStream(PDF* pdf) {
		
}

// Defaults to root as parent, else pass an intermediate page
void pdfAddPage(PDF* pdf, u8 rotation) {
		// set page root if not
		if(getIndex(pdf->references, "PagesRoot") == -1) _initPagesTree(pdf);

		char *toWrite = 
						"<< "
						"/Type %s "
						"/Parent %s "
						"/Resources "
						"/Rotate %d "
						">>";
		beginobj(pdf, 10);	
		
		// very stupid
		char page[50] = "page";
		char* buf;
		buf = malloc(sizeof(size_t));
		
		sprintf(buf, "%zu", ++pdf->TotalPageCount);
		strcat(page, buf);
		//char* ref = genRef(pdf);	
		//insert(pdf->references, page, ref);
		fprintf(pdf->stream, toWrite, "/Page", get(pdf->references, "PagesRoot"), rotation);
		
		endobj(pdf);

		updateOffsets(pdf);
		//pdfUpdatePages(pdf, ref);
}

void pdfUpdatePages(PDF* pdf, const char* new_ref) {
		fpos_t pos;
		fgetpos(pdf->stream, &pos);

		int l = strlen(new_ref);
		//strncat(pdf->pages.Kids, new_ref, l);
		//pdf->pages.Count++;

		//fseek(pdf->stream, pdf->pages.Offset, SEEK_SET);

		// fix formatting
		fprintf(pdf->stream, "\n1 0 obj\n");
		//fprintf(pdf->stream, "<< /Type /Pages /Kids [%s] /Count %d /MediaBox [0 0 %d %d] >>", 
		//				pdf->pages.Kids, pdf->pages.Count, pdf->Width, pdf->Height);
		//endobj(pdf);
		fsetpos(pdf->stream, &pos);
}

void pdfCreateFont(PDF* pdf, Font font) {
		char *toWrite =
		"";

		beginobj(pdf, 69);
		//insert(pdf->references, (char*)font.name, genRef(pdf));
		pdf->CurrentOffset++;
		fprintf(pdf->stream, "<< /Type /Font /Subtype /Type1 /BaseFont /%s >>", font.name);
		endobj(pdf);
		updateOffsets(pdf);
}


void pdfSetMetadata(PDF *pdf, const char *title, const char *author, const char *subject, const char *keywords) {
		pdf->trailer.title = title;
		pdf->trailer.author = author;
		pdf->trailer.subject = subject;
		pdf->trailer.keywords = keywords;
}

/*

void writeHeader() {


// objs3-5 > fontTables
pdf->Offsets[2] = ftell(pdf->stream);
fprintf(pdf->stream, "3 0 obj\n");
fprintf(pdf->stream, "<< /Type /Font /Subtype /Type1 /BaseFont /FreeSerif >>\n");
fprintf(pdf->stream, "endobj\n");

pdf->Offsets[3] = ftell(pdf->stream);
fprintf(pdf->stream, "4 0 obj\n");
fprintf(pdf->stream, "<< /Type /Font /Subtype /Type1 /BaseFont /FreeSerif-Bold >>\n");
fprintf(pdf->stream, "endobj\n");

pdf->Offsets[4] = ftell(pdf->stream);
fprintf(pdf->stream, "5 0 obj\n");
fprintf(pdf->stream, "<< /Type /Font /Subtype /Type1 /BaseFont /FreeSerif-Italic >>\n");
fprintf(pdf->stream, "endobj\n");
}



void createPage() {

}

void writeLine(const *char text, int x, int y) {

if(token.type == HEADING) {
fprintf(pdf->stream, "BT /F1 24 Tf %d %d Td (%s) Tj ET\n", x, y, text);
}
else if(token.type == SUBHEADING) {
fprintf(pdf->stream, "BT /F1 22 Tf %d %d Td (%s) Tj ET\n", x, y, text);
}
else if(token.type == SUBSUBHEADING) {
fprintf(pdf->stream, "BT /F1 18 Tf %d %d Td (%s) Tj ET\n", x, y, text);
}
else if(token.type == BOLD) {
fprintf(pdf->stream, "BT /F2 12 Tf %d %d Td (%s) Tj ET\n", x, y, text);
}
else if(token.type == ITALIC) {
fprintf(pdf->stream, "BT /F3 12 Tf %d %d Td (%s) Tj ET\n", x, y, text);
}
else if(token.type == PLAIN_TEXT) {
fprintf(pdf->stream, "BT /F1 12 Tf %d %d Td (%s) Tj ET\n", x, y, text);
}
}

void calcTextSize() {
double charWidth = 12.f / 72.f; //1/6 inches
int numCharsFitAcrossWidth = (int) (8.5f / charWidth);     //inches	
printf("%f\n", charWidth);
}

void writepdf() {


// page 1 settings
pdf->Offsets[5] = ftell(pdf->stream);
fprintf(pdf->stream, "6 0 obj\n");
fprintf(pdf->stream, "<< /Type /Page /Parent 2 0 R /MediaBox [0 0 %d %d] /Contents 7 0 R /Resources << /Font << /F1 3 0 R /F2 4 0 R /F3 5 0 R >> >> >>\n", pdf->Width, pdf.height);
fprintf(pdf->stream, "endobj\n");

// page 1 content
pdf->Offsets[6] = ftell(pdf->stream);
fprintf(pdf->stream, "7 0 obj\n");
fprintf(pdf->stream, "<< /Length 44 >>\n"); //len of content stream
fprintf(pdf->stream, "stream\n");

int currentDrawY = 900;
int currentDrawX = 100;
int currentLine = 1;

calcTextSize();

while(!isEmpty(Tokens)) {
Token token;
dequeue(Tokens, &token);

if(currentLine != token.line) {
	currentLine = token.line;
	currentDrawX = 100;
	currentDrawY -= 25;
}

if(token.type == INDENT) {
	currentDrawX += 50;
} 
else {
	writeLine(token, currentDrawX, currentDrawY);
	currentDrawX += 25;
}
}

// BT & ET begin and end text obj; /F1 24 Tf sets font to F1 w/ size 24; 100 700 Td is pos;
// () Tj shows text

}

*/

void pdfCreateXRef(PDF* pdf) 
{
		pdf->Offsets[pdf->CurrentOffset] = ftell(pdf->stream);
		pdf->XRef.offset = pdf->CurrentOffset;
		pdf->CurrentOffset++;
		fprintf(pdf->stream, "\nxref\n");
		fprintf(pdf->stream, "0 %d\n", pdf->ObjCount + 1);
		fprintf(pdf->stream, "0000000000 65535 f \n");
		// make a xref offset
		for (int i = 0; i < pdf->ObjCount; i++) 
		{
			fprintf(pdf->stream, "%010u 00000 n \n", pdf->Offsets[i]);
		}
}

void pdfCreateTrailer(PDF* pdf) 
{
		fprintf(pdf->stream, "\ntrailer\n");
		fprintf(pdf->stream, 
						"<< /Size %d \n/Info << /Title (%s) /Author (%s) /Subject(%s) /Keywords (%s) >>\n/Root %s \n>>\n", 
						pdf->ObjCount + 1, pdf->trailer.title, pdf->trailer.author, pdf->trailer.subject, 
						pdf->trailer.keywords, genRef(pdf->DocumentCatalog->Reference));
		fprintf(pdf->stream, "startxref\n");
		fprintf(pdf->stream, "%u\n", pdf->Offsets[pdf->XRef.offset]);
		fprintf(pdf->stream, "\%%\%%EOF\n");
}

void pdfEmpty(PDF *pdf) 
{
		if(pdf == NULL) {
				printf("[yung_pdf] pdf-obj most likely already killed\n");
				return;
		}

		if(pdf->stream != NULL && fclose(pdf->stream) == 0) 
		{
				printf("[yung_pdf ~io] closed properly ✅\n");
				pdf->stream = NULL;
		}

		if(pdf->Offsets != NULL) 
		{
				free(pdf->Offsets);
				pdf->Offsets = NULL;
		}

		killSMap(pdf->references);
}

void pdfDestroy(PDF **pdf) 
{
		if(pdf == NULL) 
		{
				printf("[xtra-pdf] pdf-obj most likely already killed\n");
				return;
		}
		pdfEmpty(*pdf);

		free(*pdf);
		*pdf = NULL;
}
