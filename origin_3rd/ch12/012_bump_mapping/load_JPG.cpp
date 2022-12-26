#include "scene.h"
#include "jpeglib.h"

///////////////////////////////// DECODE JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////	This decodes the jpeg and fills in the tImageJPG structure
///////////////////////////////// DECODE JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void DecodeJPG(jpeg_decompress_struct* cinfo, tImageJPG *pImageData)
{
	// Read in the header of the jpeg file
	jpeg_read_header(cinfo, TRUE);
	
	// Start to decompress the jpeg file with our compression info
	jpeg_start_decompress(cinfo);

	// Get the image dimensions and row span to read in the pixel data
	pImageData->rowSpan = cinfo->image_width * cinfo->num_components;
	pImageData->sizeX   = cinfo->image_width;
	pImageData->sizeY   = cinfo->image_height;
	
	// Allocate memory for the pixel buffer
	pImageData->data = new unsigned char[pImageData->rowSpan * pImageData->sizeY];
		
	// Here we use the library's state variable cinfo.output_scanline as the
	// loop counter, so that we don't have to keep track ourselves.
	
	// Create an array of row pointers
	unsigned char** rowPtr = new unsigned char*[pImageData->sizeY];
	for (int i = 0; i < pImageData->sizeY; i++)
		rowPtr[i] = &(pImageData->data[i*pImageData->rowSpan]);

	// Now comes the juice of our work, here we extract all the pixel data
	int rowsRead = 0;
	while (cinfo->output_scanline < cinfo->output_height) 
	{
		// Read in the current row of pixels and increase the rowsRead count
		rowsRead += jpeg_read_scanlines(cinfo, &rowPtr[rowsRead], cinfo->output_height - rowsRead);
	}
	
	// Delete the temporary row pointers
	delete [] rowPtr;

	// Finish decompressing the data
	jpeg_finish_decompress(cinfo);
}

///////////////////////////////// LOAD JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////	This loads the JPG file and returns it's data in a tImageJPG struct
///////////////////////////////// LOAD JPG \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

tImageJPG *LoadJPG(const char *filename)
{
	struct jpeg_decompress_struct cinfo;
	tImageJPG *pImageData = NULL;
	FILE *pFile;
	
	// This is the only function you should care about.  You don't need to
	// really know what all of this does (since you can't cause it's a library!) :)
	// Just know that you need to pass in the jpeg file name, and get a pointer
	// to a tImageJPG structure which contains the width, height and pixel data.
	// Be sure to free the data after you are done with it, just like a bitmap.
	
	// Open a file pointer to the jpeg file and check if it was found and opened 
	if((pFile = fopen(filename, "rb")) == NULL) 
	{
		// Display an error message saying the file was not found, then return NULL
		MessageBox(hWnd, "Unable to load JPG File!", "Error", MB_OK);
		return NULL;
	}
	
	// Create an error handler
	jpeg_error_mgr jerr;

	// Have our compression info object point to the error handler address
	cinfo.err = jpeg_std_error(&jerr);
	
	// Initialize the decompression object
	jpeg_create_decompress(&cinfo);
	
	// Specify the data source (Our file pointer)	
	jpeg_stdio_src(&cinfo, pFile);
	
	// Allocate the structure that will hold our eventual jpeg data (must free it!)
	pImageData = (tImageJPG*)malloc(sizeof(tImageJPG));

	// Decode the jpeg file and fill in the image data structure to pass back
	DecodeJPG(&cinfo, pImageData);
	
	// This releases all the stored memory for reading and decoding the jpeg
	jpeg_destroy_decompress(&cinfo);
	
	// Close the file pointer that opened the file
	fclose(pFile);
	
	if(pImageData->data)		// obratenie textury
	{
		unsigned char*	pom;
		int x,y;

		pom = (unsigned char*) malloc(pImageData->rowSpan*pImageData->sizeY);

		for(y=0; y<pImageData->sizeY; y++)
			for(x=0; x< pImageData->rowSpan; x++) pom[x+y*pImageData->rowSpan]=pImageData->data[x+(pImageData->sizeY-1-y)*pImageData->rowSpan];
		free(pImageData->data);
		pImageData->data = pom;
	}	

	// Return the jpeg data (remember, you must free this data after you are done)
	return pImageData;
}

int scene::LoadGLTexture_JPG(char *texture_name,GLuint *texture)
{
	int Status=TRUE;								// indikator
	tImageJPG *TextureImage;			// Load the image and store the data
	
	TextureImage=NULL;
	if (TextureImage=LoadJPG(texture_name))
	{
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if(edge_clamp_supported)
		{	// vyuzijeme rozsirenie GL_SGIS_texture_edge_clamp (GL_SGI_texture_edge_clamp, GL_EXT_texture_edge_clamp )
			// pri texturavani v blizkosti hrany sa farba hranu nepouziva, texturuje sa iba z textury
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE_SGIS );	// nastavuje ze textury sa v smere u (vodorovnom) neopakuju
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE_SGIS );	// nastavuje ze textury sa v smere v (zvislom) neopakuju
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );		// nastavuje ze textury sa v smere u (vodorovnom) neopakuju
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );		// nastavuje ze textury sa v smere v (zvislom) neopakuju
		}
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	}
	else 
	{	MessageBox(NULL,"Chyba naèítania obrázka.",texture_name,MB_ICONERROR);Status=false;}

	if (TextureImage)									// Ak textura existuje
	{
		if (TextureImage->data)free(TextureImage->data);// ak existuju data uvo¾ni z pamäte
		free(TextureImage);								// uvo¾ni štruktúru
	}
	return Status;
}

int scene::LoadTexture_JPG_bump(char *texture_name, GLuint *shift, GLuint *inv)	//to iste ako LoadTexture_bump zmeneni iba jeden riadok
{
	int Status=TRUE;								// indikator
	tImageJPG *TextureImage;					// miesto pre štruktúru

	TextureImage=NULL;
	if (TextureImage=LoadJPG(texture_name))			// ZMENA namiesto LoadBMP pouzivam LoadJPG
	{
		int i;
		unsigned char *t = new unsigned char[TextureImage->sizeX*TextureImage->sizeY*3];

		for( i=0;i<3*TextureImage->sizeX*TextureImage->sizeY;)
		{
			t[i] = TextureImage->data[i];		// red
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// green
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// blue
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
		}

		glGenTextures(1, shift);
		glBindTexture(GL_TEXTURE_2D, *shift);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if(edge_clamp_supported)
		{	// vyuzijeme rozsirenie GL_SGIS_texture_edge_clamp (GL_SGI_texture_edge_clamp, GL_EXT_texture_edge_clamp )
			// pri texturavani v blizkosti hrany sa farba hranu nepouziva, texturuje sa iba z textury
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE_SGIS );	// nastavuje za textury sa v smere u (vodorovnom) neopakuju
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE_SGIS );	// nastavuje za textury sa v smere v (zvislom) neopakuju
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );		// nastavuje za textury sa v smere u (vodorovnom) neopakuju
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );		// nastavuje za textury sa v smere v (zvislom) neopakuju
		}
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, t);

		for( i=0;i<3*TextureImage->sizeX*TextureImage->sizeY;)
		{
			t[i] = TextureImage->data[i];		// red
			t[i] = ~t[i];				// invertovanie
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// green
			t[i] = ~t[i];				// invertovanie
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// blue
			t[i] = ~t[i];				// invertovanie
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
		}

		glGenTextures(1, inv);
		glBindTexture(GL_TEXTURE_2D, *inv);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if(edge_clamp_supported)
		{	// vyuzijeme rozsirenie GL_SGIS_texture_edge_clamp (GL_SGI_texture_edge_clamp, GL_EXT_texture_edge_clamp )
			// pri texturavani v blizkosti hrany sa farba hranu nepouziva, texturuje sa iba z textury
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE_SGIS );	// nastavuje za textury sa v smere u (vodorovnom) neopakuju
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE_SGIS );	// nastavuje za textury sa v smere v (zvislom) neopakuju
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );		// nastavuje za textury sa v smere u (vodorovnom) neopakuju
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );		// nastavuje za textury sa v smere v (zvislom) neopakuju
		}
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, t);
		delete []t;	
	}
	else 
	{	MessageBox(NULL,"Chyba naèítania obrázka.",texture_name,MB_ICONERROR);Status=false;}

	if (TextureImage)									// Ak textura existuje
	{
		if (TextureImage->data)free(TextureImage->data);// ak existuju data uvo¾ni z pamäte
		free(TextureImage);								// uvo¾ni štruktúru
	}
	return Status;
}