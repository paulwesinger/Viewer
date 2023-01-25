#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <string>
#include "../base2d/base2d.h"
#include "../menu/menu.h"
#include "../shaders/shader.h"

class ToolBar : public Base2D
{
public:
    ToolBar(int resX, int resY,Shader * sh);
    ToolBar(int resX, int resY, int w, int h,Shader * sh);
    ToolBar(int resX, int resY, int w, int h, glm::vec4 bg, std::string path, glm::vec4 fg, Shader * sh);
    ToolBar(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg, std::string path, glm::vec4 fg, Shader *sh);

private:


};

#endif // TOOLBAR_H
