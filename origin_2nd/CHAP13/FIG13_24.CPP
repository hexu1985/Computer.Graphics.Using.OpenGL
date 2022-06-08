class Edge{
		Point3 first, second;	// end points of the edge
		Face *face1, *face2;	// pointers to its two faces	
		Cuboid extent;		// 3D extent of edge
		Face *nextFace;		// next face for testing	
};
