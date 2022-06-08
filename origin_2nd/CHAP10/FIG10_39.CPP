#define eps 0.000001
xFirst = (short)(xLeft + 1 - eps);	// on xLeft or the next up
xLast  = (short)(xRight - eps);     // one smaller than xRight
for(x = xFirst; x <= xLast; x++) setPixel(x, y);
