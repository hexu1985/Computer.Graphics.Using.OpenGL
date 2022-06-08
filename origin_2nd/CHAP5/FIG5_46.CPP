cvs.pushCT();			// so we can return here
cvs.translate2D(W, H);	      // position for the first motif
for(row = 0; row < 3; row++)	// draw each row
{ 				     		
	cvs.pushCT();
	for(col = 0; col < 4; col++)// draw the next row of motifs
	{		
		motif();
		cvs.translate2D(L, 0); 	// move to the right
	}
	cvs.popCT(); 			// back to the start of this row
	cvs.translate2D(0, D);	// move up to the next row
}
cvs.popCT();			// back to where we started




