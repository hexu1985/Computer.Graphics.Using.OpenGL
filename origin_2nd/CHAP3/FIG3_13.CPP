float cx = 0.3, cy = 0.2; //center of the window
float H, W = 1.2, aspect = 0.7; // window properties
set the viewport
for(int frame = 0; frame < NumFrames; frame++) // for each frame
{
	clear the screen		  // erase the previous figure
W *= 0.7;		// reduce the window width
H = W * aspect;	// maintain the same aspect ratio
setWindow(cx - W, cx + W, cy - H, cy + H); //set the next window
hexSwirl();   // draw the object
}	
