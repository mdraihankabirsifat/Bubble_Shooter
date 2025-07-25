#include "iGraphics.h"

int mouseX = 0, mouseY = 0;

Image bg;
void iDraw()
{
	// place your drawing codes here
	iClear();
	iShowImage(0, 0, "assets/images/bouncing ball/girl.jpg");
	// iResizeImage(&bg, 800, 750);
	// Display mouse coordinates
	char coords[50];
	sprintf(coords, "Mouse: (%d, %d)", mouseX, mouseY);
	iSetColor(255, 255, 255);
	iText(10, 720, coords, GLUT_BITMAP_HELVETICA_18);
}

void iKeyPress(unsigned char key)
{
	if (key == 'q')
	{
		iExitMainLoop();
	}
}

void iMouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

int main(int argc, char *argv[])
{
	iWindowedMode(800, 750, "ImageDemo");
	iStartMainLoop();
	return 0;
}