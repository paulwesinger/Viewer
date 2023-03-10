#ifndef CCONTROLLCONTAINER_H
#define CCONTROLLCONTAINER_H

#include <vector>
#include "../base2d/base2d.h"
#include "../baseobject/baseobject.h"
#include "../buttons/button.h"
#include "../textrenderer/textrender.h"
#include "../defines.h"
#include "../shaders/shader.h"



const int SPACER = 10;

// Container Class für
// 2D Steuerelemente(Buttons ,...
class CControllContainer
{
public:
    CControllContainer(Shader * sh, LAYOUT l = LAYOUT::Vertical);
    CControllContainer(Shader * sh, int px=0, int py=0, int w=100, int h=100, LAYOUT l = LAYOUT::Vertical);
    ~CControllContainer();

    void setLayout(LAYOUT l);
    bool removeContainer(CControllContainer *container);
    void rename(std::string theNewName);

    void enableChilds();
    void disableChilds();
    void disableControll(Base2D * con);
    void enableControll(Base2D * con);

    bool addControll2D(Base2D *control);
    bool addControll3D(BaseObject * baseobject);
    bool addText(std::string text,int resx, int resy);
    bool addSpacer();

    void setPos(sPoint pos);
    void setDimensions(sSize size);

    sSize Dimensions();
    sPoint Pos();
    void DragContainer(sPoint newp);
    void DragContainer(int newx, int newy);
    sPoint NextControllPos();

    std::vector<Base2D *> controlls2D;
    std::vector<BaseObject*> controlls3D;
private:

    void CalcNextPos(int value);
    void releaseConterItems();

    std::string _Name;

    int _Height; // errechnete höhe
    int _Width;
    int _CurrentY; //position für nächstes element
    int _CurrentX;

    sSize _Dimensions; // von user seite
    sPoint _Pos; // absoluter wert

    Shader * shader;
    LAYOUT layout;
};

#endif // CCONTROLLCONTAINER_H
