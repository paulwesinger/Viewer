#include "toolbar.h"
#include <glm/vec4.hpp>

ToolBar::ToolBar(int resX, int resY,Shader * sh)
    : Window(resX,resY,sh)
{
    Init();
}

ToolBar::ToolBar(int resX, int resY, int w, int h,Shader * sh)
    : Window(resX, resY, sh)
{
    Init();
}
ToolBar::ToolBar(int resX, int resY, int w, int h, glm::vec4 bg, glm::vec4 fg,Shader * sh, std::string path)
    : Window(resX, resY,path, sh)
{
    Init();
}
ToolBar::ToolBar(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg,glm::vec4 fg, Shader *sh, std::string path)
    : Window(resX,resY,path,sh)
{
    Init();
}

void ToolBar::Render() {

    if (mainmenu == nullptr)
        Window::Render();
    else
        Stretch();
}

void ToolBar::Stretch() {
    if (mainmenu != nullptr)  {
        if (mainmenu->Active()) {
            _Pos.x = mainmenu->Pos().x + mainmenu->Width();
            _Size.w = _ResX -_Pos.x;
        }
        else{
            _Pos.x = 0;
            _Size.w = _ResX;
        }
    }
    else {
            _Pos.x = 0;
            _Size.w = _ResX;
         }
}

void ToolBar::Init() {
    _Pos.x = 100;
    _Pos.y = 0;
    _Size.w = _ResX - 100;
    _Size.h = 30;

    _Color = glm::vec4(0.8,0.8,1,1);
    mainmenu = nullptr;
}

void ToolBar::setMenuPtr(CMenu * ptr) {
    if (mainmenu != nullptr)
        mainmenu = ptr;
}

CMenu * ToolBar::getMenuPtr() {
    return mainmenu;
}
