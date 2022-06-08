void arrow(float f, float h, float t, float w)
{ // assumes global Canvas object: cvs
	cvs.lineRel(-w - t / 2, -f); 		// down the left side  
	cvs.lineRel(w, 0);  
	cvs.lineRel(0, -h); 	
	cvs.lineRel(t, 0); 			// across 
	cvs.lineRel(0, h);  			// back up 
	cvs.lineRel(w, 0); 
	cvs.lineRel(-w - t / 2, f); 	
}
