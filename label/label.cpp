#include "label.h"

Label::Label(int resx, int resy, std::string path, Shader * sh)
    : Base2D(resx,resy,sh)
{
    setColor(glm::vec4(0,0.0,0.7,0.5));
    _PathToText = path;
    _Text = nullptr;
    init();

}

void Label::init() {
    _Text = new Base2D(_ResX,_ResY,_PathToText,shader);
}

void Label::Render ()
{
    Base2D::Render();
    if (_Text != nullptr)
        _Text->Render();
}
