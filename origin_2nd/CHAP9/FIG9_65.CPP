void drawTrio(int depth)
{ // Draw a triomino manipulating the CT
	if(depth >= maxDepth)    // draw the outline
	{   							
		worldMoveTo(0, 0);	worldLineTo(0, 4); 
		worldLineTo(2, 4);	worldLineTo(2, 2);      
		worldLineTo(4, 2); worldLineTo(4, 0);  
		worldLineTo(0, 0);
		return;
   }
	saveCT();					// make a copy for later 
	depth++; 					// update for the next generation
   	scale(0.5, 0.5);        
	drawTrio(depth); 			// Draw B
   	translate(2,2); 		 	// to pivot of C
	drawTrio(depth);			// Draw C 
	translate(6,-2);   
	rotate(90);  
	drawTrio(depth); 			// Draw A
   	translate(8,8);    
	rotate(180);  
	drawTrio(depth); 			// Draw D
	restoreCT(); 				// restore the original CT
}
