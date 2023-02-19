#ifndef MENUITEM_H
#define MENUITEM_H

#include "../base2d/base2d.h"
//#include "../shaders/shader.h"
#include "../menu/menu.h"
//#include "../defines.h"


class MenuItem : public Base2D
{
public:
    MenuItem(int resx, int resy, Shader * s);
    MenuItem(int resx, int resy, sPoint pos, sSize size, std::string bgtexture, std::string texttexture,  Shader * s);

    void Render() override;

protected:
    Base2D * _TextImage;
private:

    CMenu * _SubMenu;
    int _ID;
};

#endif // MENUITEM_H
