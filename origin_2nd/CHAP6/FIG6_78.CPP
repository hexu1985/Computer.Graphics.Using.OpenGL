int Mesh:: readmesh(char * fileName)
{
	fstream infile;
	infile.open(fileName, ios::in);
	if(infile.fail()) return -1; // error - can't open file
	if(infile.eof())  return -1; // error - empty file
	infile >> numVerts >> numNorms >> numFaces;
pt = new Point3[numVerts];
norm = new Vector3[numNorms];
face = new Face[numFaces];
//check that enough memory was found:
if( !pt || !norm || !face)return -1; // out of memory
for(int p = 0; p < numVerts; p++) // read the vertices
	infile >> pt[p].x >> pt[p].y >> pt[p].z;
for(int n = 0; n < numNorms; n++) // read the normals
	infile >> norm[n].x >> norm[n].y >> norm[n].z;
for(int f = 0; f < numFaces; f++)// read the faces
{
	infile >> face[f].nVerts;
	face[f].vert = new VertexId[face[f].nVerts];
	for(int i = 0; i < face[f].nVerts; i++)
		infile >> face[f].vert[i].vertIndex 
                   >> face[f].vert[i].normIndex;
} 
return 0; // success
} 
