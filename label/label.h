#ifndef LABEL_H
#define LABEL_H

#include "../base2d/base2d.h"
#include "../textrenderer/textrender.h"


class Label : public Base2D
{
public:
    Label(int resx, int resy, std::string textpath, Shader * sh);
    Label(int resx, int resy, std::string backroundpath, std::string textpath, Shader * sh);

    void Render() override;
    void setPos(int x, int y) override;
    void setSize(int w, int h) override;


private:
    void init();
    Base2D * _Text;
    std::string _PathToText;
    std::string _PathBackground;
};

#endif // LABEL_H
