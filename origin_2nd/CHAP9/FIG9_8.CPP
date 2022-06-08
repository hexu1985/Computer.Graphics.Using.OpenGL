
void produceString(char *st,int order)
{
  for(;st; st++)		// scan through each character 
	switch(*st)
	{
		case ‘+’: CD -= angle; break;	// right turn 
		case '-': CD += angle; break;  // left turn
		case 'F': if(order > 0) 
			produceString(Fstr, order - 1);
		 	else forward(length, 1); break;
		case ‘X’: if(order > 0)
			produceString(Xstr, order - 1); break;
		case ‘Y’: if(order > 0)
			produceString(Ystr, order - 1);
	}
} 


