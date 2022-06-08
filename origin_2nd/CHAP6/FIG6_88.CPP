void Mesh:: makeSurfaceMesh()
{
	int i, j, numValsU = numValsV = 40;// set these
	double u, v, uMin = vMin = -10.0, uMax = vMax = 10.0; 
	double delU = (uMax - uMin)/(numValsU - 1);
	double delV = (vMax - vMin)/(numValsV - 1);

	numVerts = numValsU * numValsV + 1; // total # of vertices
	numFaces = (numValsU -1) * (numValsV - 1) ; // # of faces
	numNorms = numVerts; // for smooth shading - one normal per vertex
	pt   = new Point3[numVerts];  assert(pt   != NULL); // make space 
	face = new Face[numFaces];    assert(face != NULL);
	norm = new Vector3[numNorms]; assert(norm != NULL);

	for(i = 0, u = uMin; i < numValsU; i++, u += delU)
		for(j = 0, v = vMin; j < numValsV; j++, v += delV)
		{
			int whichVert = i * numValsV + j; //index of the vertex and normal
			// set this vertex: use functions X, Y, and Z
			pt[whichVert].set(X(u, v),Y(u, v),Z(u, v));
			// set the normal at this vertex: use functions nx, ny, nz
			norm[whichVert].set(nx(u, v), ny(u, v), nz(u, v));
			normalize(norm[whichVert]);
			// make quadrilateral
			if(i > 0 && j > 0) // when to compute next face
			{
				int whichFace =(i - 1) * (numValsV - 1) + (j - 1);						face[whichFace].vert = new VertexID[4];
				assert(face[whichFace].vert != NULL);
				face[whichFace].nVerts = 4;
				face[whichFace].vert[0].vertIndex = // same as norm index
				face[whichFace].vert[0].normIndex = whichVert;
				face[whichFace].vert[1].vertIndex = 
				face[whichFace].vert[1].normIndex = whichVert - 1;
				face[whichFace].vert[2].vertIndex = 
				face[whichFace].vert[2].normIndex = whichVert - numValsV - 1;
				face[whichFace].vert[3].vertIndex = 
				face[whichFace].vert[3].normIndex = whichVert - numValsV;
			}
		}
}
