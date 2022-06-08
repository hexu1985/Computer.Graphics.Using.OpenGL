void drawVisibleEdges(FaceList faces)
{
    EdgeList edges; // the edge list
    build the edge list of all potentially visible edges
    while(edges is not empty) 
    {
	Edge E = getNext(edges); // remove the next edge
	bool isVis = true;   // suppose it’s visible
for(F = E.nextFace; isVis && F != NULL; <F is next face>) 
{
         if(E belongs to F)continue; // no need to test this face
	   isVis = edgeTest(E,F); //put survivors on edge list 
      } 
    if (isVis) E.draw(); // E is visible; draw it
    } 
}
