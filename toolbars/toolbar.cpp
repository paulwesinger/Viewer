#include "toolbar.h"

ToolBar::ToolBar(int resX, int resY,Shader * sh)
    : Base2D(resX,resY,sh)
{

}

ToolBar::ToolBar(int resX, int resY, int w, int h,Shader * sh)
    : Base2D(resX, resY, sh)
{

}
ToolBar::ToolBar(int resX, int resY, int w, int h, glm::vec4 bg, std::string path, glm::vec4 fg,Shader * sh)
    : Base2D(resX, resY,path, sh)
{

}
ToolBar::ToolBar(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg, std::string path, glm::vec4 fg, Shader *sh)
    : Base2D(resX,resY,path,sh)
{

}
