
void doTrio(double size, int depth)
{
	int i;
	if(depth == 1) drawTrio();
	else for(i = 0; i < 4; i++) // draw four children
	{
		<set up for the i-th child>;
		doTrio(size / 2, depth - 1);				// draw child
	}

}
