void myDisplay(void)
{
	clear the screen
	setWindow(-0.6, 0.6, -0.6, 0.6); // the portion of the swirl to draw	
	for(int i = 0; i < 5; i++)	     // make a pattern of 5 by 4 copies
	   for(int j = 0; j < 4; j++)
	   {
		   int L = 80; // the amount to shift each viewport
		   setViewport(i * L, L + i * L, j * L, L + j * L); // the next viewport
		   hexSwirl();
	   }
}
