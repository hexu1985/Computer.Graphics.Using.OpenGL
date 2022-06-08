void myReshape(GLsizei W, GLsizei H)
{
	if(R > W/H) // use (global) window aspect ratio
		setViewport(0, W, 0, W/R);
	else
            setViewport(0, H * R, 0, H);
}
