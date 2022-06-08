
class Point
{
public:
	float cx;
	float cy;
public:
	Point():cx(0), cy(0){}
	Point(float x, float y):cx(x), cy(y){}
	Point(Point &p)
	{
		cx = p.cx;
		cy = p.cy;
	}
	void operator = (Point t)
	{
		cx = t.cx;
		cy = t.cy;
	}
};
int random(int m)
{
	return rand()%m;
}
void Sierpinskl()
{
	Point *temp = new Point[3];
	temp[0].cx = 10;  temp[0].cy = 10;
	temp[1].cx = 400; temp[1].cy = 10;
	temp[2].cx = 200; temp[2].cy = 300;
	int index = random(3);
	Point point = temp[index];
	drawDot(point);
	for(int i=0; i<3000; ++i)
	{
		index = random(3);
		point.cx = (point.cx + temp[index].cx)/2;
		point.cy = (point.cy + temp[index].cy)/2;
		drawDot(point);
	}
}