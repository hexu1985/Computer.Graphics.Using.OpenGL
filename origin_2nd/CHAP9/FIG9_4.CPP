void drawKoch(double dir, double len, int n)
{ // “Koch” to order n the line of length len
		 // from CP in the direction dir 

	double dirRad = 0.0174533 * dir; // in radians  
	if(n == 0)
		lineRel(len * cos(dirRad), len * sin(dirRad));
	else{ 
		n--;						// reduce the order
		len /= 3;					// and the length
		drawKoch(dir, len, n);
		dir += 60;
		drawKoch(dir, len, n);
		dir -= 120;
		drawKoch(dir, len, n);
		dir += 60;
		drawKoch(dir, len, n)
	} 
} 
