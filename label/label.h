#ifndef LABEL_H
#define LABEL_H

#include "../base2d/base2d.h"
#include "../textrenderer/textrender.h"


class Label : Base2D
{
public:
    Label(int resx, int resy, std::string path, Shader * sh);


    void Render() override;

private:
    void init();
    Base2D * _Text;
    std::string _PathToText;
};

#endif // LABEL_H
