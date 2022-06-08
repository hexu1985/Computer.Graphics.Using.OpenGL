class RGBpixmap{
   private: 
		int nRows, nCols; // dimensions of the pixmap
		RGB* pixel; // array of pixels
  public:
	RGBpixmap() {nRows = nCols = 0; pixel = 0;}
	RGBpixmap(int r, int c) //constructor
	{
		nRows = r;
		nCols = c;
		pixel = new RGB[r*c]; 
	}
	void setPixel(int x, int y, RGB color)
	{
		if(x >= 0 && x < nCols && y >= 0 && y < nRows)
			pixel[nCols * y + x] = color;
	}
	RGB getPixel(int x, int y)
	{
			return pixel[nCols * y + x];
	}
   //*** draw this pixmap at the current raster position 
   void draw(){ 
		glDrawPixels(nCols, nRows, GL_RGB, GL_UNSIGNED_BYTE,pixel);
	}
	//*** read a rectangle of pixels into this pixmap 
   void read(int x, int y, int wid, int ht){ 
		nRows = ht;
		nCols = wid;
		pixel = new RGB[nRows *nCols]; if(!pixel)exit(-1);
		glReadPixels(x, y, nCols, nRows, GL_RGB, 
					  GL_UNSIGNED_BYTE, pixel);
	}
   //*** copy a region of the display back onto the 

display 
   void copy(int x, int y, int wid, int ht){ 
		glCopyPixels(x, y, wid, ht, GL_COLOR);
	}
	//*** read BMP file into this pixmap
   int readBmpFile(char * fname);
	//*** write this pixmap to a BMP file
   void writeBmpFile(char * fname);
	
	// …others …

};








