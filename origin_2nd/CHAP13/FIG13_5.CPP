class Face{
  int nVerts;       // number of vertices in vert array
  Point* pt;  	  // array of vertices in real screen coord’s
  float* depth;     // array of vertex depths
  Plane plane;      // data for the plane of the face
  Cuboid extent;    // the extent of the face
  // other properties  
};
