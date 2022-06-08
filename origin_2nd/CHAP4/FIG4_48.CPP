int CyrusBeckClip(LineSegment& seg, LineList L)
{ 
	double numer, denom; // used to find hit time for each line
	double tIn = 0.0, tOut = 1.0; 
	Vector2 c, tmp;
  <form vector: c = seg.second - seg.first>
	for(int i = 0; i < L.num; i++) // chop at each bounding line
	{
		<form vector tmp = L.line[i].pt - first>
		numer = dot(L.line[i].norm, tmp);
		denom = dot(L.line[i].norm, c);
		if(!chopCI(numer, denom, tIn, tOut)) return 0; // early out
	}
  // adjust the endpoints of the segment; do second one 1st.
	if (tOut < 1.0 ) // second endpoint was altered
	{
		 seg.second.x  = seg.first.x + c.x * tOut;
		 seg.second.y  = seg.first.y + c.y * tOut;
	}
	if (tIn > 0.0)  // first endpoint was altered
	{
		 seg.first.x  = seg.first.x + c.x * tIn;
		 seg.first.y  = seg.first.y + c.y * tIn;
}
	return 1; // some segment survives
}
