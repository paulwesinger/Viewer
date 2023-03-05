#include "label.h"

Label::Label(int resx, int resy, std::string textpath, Shader * sh)
    : Base2D(resx,resy,sh)
{
    setColor(glm::vec4(0,0.0,0.7,0.5));
    _PathToText = textpath;
    _PathBackground = "";
    _Text = nullptr;
    init();

}

Label::Label(int resx, int resy, std::string backgroundpath, std::string textpath, Shader * sh)
    : Base2D(resx,resy,backgroundpath,sh)
{
    setColor(glm::vec4(0,0.0,0.7,0.5));
    _PathToText = textpath;
    _PathBackground = backgroundpath;

    _Text = new Base2D(resx,resy,textpath,sh);
    init();
}

void Label::init() {

    if (_Text == nullptr)
        _Text = new Base2D(_ResX,_ResY,_PathToText,shader);
}

void Label::setPos(int x, int y)
{
    Base::setPos(x,y);
    if (_Text)
        _Text->setPos(x,y);
}

void Label::setSize(int w, int h)
{
    Base::setSize(w,h);
    if (_Text)
        _Text->setSize(w,h);
}

void Label::Render ()
{
    Base2D::Render();
    if (_Text != nullptr)
        _Text->Render();
}
