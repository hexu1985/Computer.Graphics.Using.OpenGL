class Point4{
public:
	float x,y,z,d;
	Point4(float a, float b, float c, float d);
	void mult(Matrix4 M);
	// other fields and methods 
};

class Vector4{
public:
	float x,y,z,d;
	Vector4(float a, float b, float c, float d);
	void mult(Matrix4 M);
	// other fields and methods 
};