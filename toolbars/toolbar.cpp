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

    Window::Render();
}

void ToolBar::Init() {
    _Pos.x = 100;
    _Pos.y = 0;
    _Size.w = _ResX - 100;
    _Size.h = 30;

    _Color = glm::vec4(0.8,0.8,1,1);
}
