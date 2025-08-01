#include "iGraphics.h"

int pic_x, pic_y;

/*
    function iDraw() is called again and again by the system.
*/

Image smurf;

void iDraw()
{
    // place your drawing codes here
    iClear();
    iShowImage(pic_x, pic_y, "assets/images/sprites/Golem_2/Idle/0_Golem_Idle_000.png");
    iText(10, 10, "Use cursors to move the picture.");
}

/*
    function iKeyPress() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
*/
void iKeyPress(unsigned char key)
{
    if (key == 'x')
    {
        // do something with 'x'
        exit(0);
    }
    // place your codes for other keys here
}

/*
    function iSpecialKeyPress() is called whenver user hits special keys like-
    function keys, home, end, pg up, pg down, arraows etc. you have to use
    appropriate constants to detect them. A list is:
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
    GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
    GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyPress(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    if (key == GLUT_KEY_LEFT)
    {
        pic_x--;
    }
    if (key == GLUT_KEY_RIGHT)
    {
        pic_x++;
    }
    if (key == GLUT_KEY_UP)
    {
        pic_y++;
    }
    if (key == GLUT_KEY_DOWN)
    {
        pic_y--;
    }
    // place your codes for other keys here
}

int main(int argc, char *argv[])
{
    pic_x = 0;
    pic_y = 0;
    iOpenWindow(900, 900, "PictureDemo");
    return 0;
}