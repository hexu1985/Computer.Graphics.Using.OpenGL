float fakeSphere(float s, float t)
{
	float r = sqrt((s-0.5)*(s–0.5)+(t-0.5)*(t–0.5));
	if(r < 0.3) return 1 - r/0.3; // sphere intensity
	else return 0.2;  // dark background
}
