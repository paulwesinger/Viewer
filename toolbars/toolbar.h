#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <string>
#include "../window/window.h"
#include "../menu/menu.h"
#include "../shaders/shader.h"


class ToolBar : public Window
{
public:
    ToolBar(int resX, int resY,Shader * sh);
    ToolBar(int resX, int resY, int w, int h,Shader * sh);
    ToolBar(int resX, int resY, int w, int h, glm::vec4 bg,  glm::vec4 fg, Shader * sh, std::string path="");
    ToolBar(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg, glm::vec4 fg, Shader *sh, std::string path="");

    void Render() override;

private:

    void Init();

};

#endif // TOOLBAR_H
