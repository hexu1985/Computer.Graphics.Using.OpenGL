void RGBApixmap::setChromaKey(RGB c)
{	long count = 0;
	for(int row = 0; row < nCols; row++)
	 for(int col = 0; col < nRows; col++)
	 {
RGBA p = pixel[count];
if(p.r == c.r && p.g == c.g && p.b == c.b) 
	          pixel[count++].a = 0;
	      else pixel[count++].a = 255;
	}
}
