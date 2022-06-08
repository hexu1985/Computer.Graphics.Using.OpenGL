void superCopier(RealPolyArray pts, int k)
{// Draw k-th iterate of input point list pts for the IFS
	int i;
	RealPolyArray newpts;               // reserve space for new list
	if(k == 0) drawPoints(pts);
	else for(i = 1; i <= N; i++)	      // apply each affine
	{ 									
		newpts.num = N * pts.num;		  // the list size grows fast
		for(j = 0; j < newpts.num; j++)  // transform the j-th pt
				transf(affines[i], pts.pt[j], newpts.pt[j]);
		superCopier(newpts, k - 1);
	}
}
