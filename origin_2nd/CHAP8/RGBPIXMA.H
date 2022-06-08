
class RGB{ // holds a color triple – each with 256 possible intensities
    public: unsigned char r,g,b;
};

//The RGBpixmap class stores the number of rows and columns in the pixmap, as well as the address of the first pixel in memory:

class RGBpixmap{
  public: 
	int nRows, nCols; // dimensions of the pixmap
	RGB* pixel; 	// array of pixels
	int readBMPFile(char * fname); // read BMP file into this pixmap
	void makeCheckerboard();
void setTexture(GLuint textureName);
};

void RGBpixmap:: makeCheckerboard()
{  // make checkerboard patten
	nRows = nCols = 64;
	pixel = new RGB[3 * nRows * nCols]; 
if(!pixel){cout << ”out of memory!”;return;}
	long count = 0;
	for(int i = 0; i < nRows; i++)
		for(int j = 0; j < nCols; j++)
		{
			int c = (((i/8) + (j/8)) %2) * 255;  
			pixel[count].r = c; 	// red
			pixel[count].g = c; 	// green
			pixel[count++].b = 0; 	// blue
		}
}

void RGBpixmap :: setTexture(GLuint textureName)
{
	glBindTexture(GL_TEXTURE_2D,textureName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,nCols,nRows,0, GL_RGB,
               GL_UNSIGNED_BYTE, pixel);
}
