void floodFill(short x ,short y, short intColor)
// Start at (x, y); change all pixels of intColor to newColor.
// assume drawing color is newColor 
// 4-connected version 
{
	if(getPixel(x,y) == intColor)
	{
		setPixel(x, y); 	// change its color
		floodFill(x - 1, y, intColor);    // fill left 
		floodFill(x + 1, y, intColor);    // fill right 
		floodFill(x, y + 1, intColor);    // fill down 
		floodFill(x, y - 1, intColor)     // fill up
	}
}
