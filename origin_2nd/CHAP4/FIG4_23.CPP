void canvas:: drawTween(Point2 A[], Point2 B[], int n, float t)
{   // draw the tween at time t between polylines A and B
	for(int i = 0; i < n; i++)
	{
		Point2 P;
		P = Tween(A[i], B[i],t);
		if(i == 0) 	moveTo(P.x, P.y);
		else 		lineTo(P.x, P.y);
  }
}

/* Note that Tween is left as an exercise in the text.
  It only requires two applications of the lerp function,
  one on each of the components     */