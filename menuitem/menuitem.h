#ifndef MENUITEM_H
#define MENUITEM_H

#include "../base2d/base2d.h"
#include "../shaders/shader.h"

class MenuItem : public Base2D
{
public:
    MenuItem(int resx, int resy, Shader * s);
};

#endif // MENUITEM_H
