void Mesh:: draw() // use OpenGL to draw this mesh
{            
	for(int f = 0; f < numFaces; f++) // draw each face
	{
		glBegin(GL_POLYGON);
		  for(int v = 0; v < face[f].nVerts; v++) // for each one..
		  {
			  int in = face[f].vert[v].normIndex ; // index of this normal
			  int iv =  face[f].vert[v].vertIndex ; // index of this vertex
			  glNormal3f(norm[in].x, norm[in].y, norm[in].z);
			  glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		  }
		glEnd();
	}
}
