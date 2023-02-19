#include "menuitem.h"

MenuItem::MenuItem(int resx,int resy, Shader * sh)
    :Base2D(resx,resy,sh)
{
    _TextImage = new Base2D(resx,resy,sh);
    _SubMenu  = nullptr;
    _ID = 0 ;
}

MenuItem::MenuItem(int resx, int resy, sPoint pos, sSize size, std::string bgtexture, std::string texttexture,  Shader * s)
    :Base2D(resx,resy,bgtexture,s)
{


    Base2D::setPos(pos.x,pos.y);
    Base2D::setSize(size.w, size.h);

    _TextImage = new Base2D(resx,resy,texttexture,s);
    _TextImage->setPos(pos.x, pos.y);
    _TextImage->setSize(size.w, size.h);
}

void MenuItem::Render() {

    Base2D::Render();
    _TextImage->Render();
}
