class Region{
public:
	Rect r; // a region is a rectangle
	int getSize();  / how big isit?
	bool isSimple_DrawIt(FaceList& faces); // if it’s simple, draw it
	void drawClosestFace(FaceList& faces);
	bool isInvolved(Face& face); //is the region involved with face?
	int buildQuadrants(Region& nw, Region& ne, Region& sw, Region& se);
	void draw(FaceList& faces) //the main drawing method
	{
		int size = getSize(); // how big is it?
		if(size < 1)return; // nothing to draw
		if(size == 1) drawClosestFace(faces); // covers one pixel center
		else if(isSimple_drawIt(faces))return; // draw it and exit
		else // it’s not simple enough
		{
			Region NW, NE, SW, SE;
 			buildQuadrants(NW, NE, SW, SE); / make subregions
			NW.draw(faces); // draw each of them
			NE.draw(faces);
			SW.draw(faces);
			SE.draw(faces);
		}
  }
};
