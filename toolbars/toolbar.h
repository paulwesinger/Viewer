#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <string>
#include "../window/window.h"
#include "../menu/menu.h"
#include "../shaders/shader.h"
#include "../menu/menu.h"
#include "../controllcontainer/controllcontainer.h"
#include <glm/vec4.hpp>


const glm::vec4 ENABLECOL = glm::vec4(0.5,0.5,0.5,1);
const glm::vec4 DISABLECOL = glm::vec4(0.5,0.5,0.5,0.3);
const glm::vec4 DRAGICONCOL = glm::vec4(0,0,1,1);


class ToolBar : public Window
{
public:
    ToolBar(int resX, int resY,Shader * sh);
    ToolBar(int resX, int resY, int w, int h,Shader * sh);
    ToolBar(int resX, int resY, int w, int h, glm::vec4 enablecol,  glm::vec4 disablecol, Shader * sh, std::string path="");
    ToolBar(int resX, int resY, int px, int py, int w, int h, glm::vec4 enablecol, glm::vec4 disablecol, Shader *sh, std::string path="");

    void Render() override;

    void setMenuPtr(CMenu* ptr);
    CMenu* getMenuPtr();

    void addConatiner(CControllContainer *con);
    void addCtrl(Base2D * ctl);
    void setLayOut(LAYOUT l);

    virtual void OnStartDrag(int mx, int my) override;
    virtual void OnDrag(int mx, int my) override;
    virtual void OnEndDrag(int mx, int my) override;
    virtual bool intersect(int x, int y) override;

    virtual void OnMainMenuStateChanged() ;

    int CurrentCtlPos_X();
    int CurrentCtlPos_Y();
    sPoint CurrentCtlPos();

public:
    void Stretch();
    std::vector<CControllContainer *> containerList;
    std::vector<Base2D*> CtrlList;


private:

    void Init();
    void InitSize(int w, int h);
    void InitPos(int px, int py);
    void InitColor(glm::vec4 enablecol, glm::vec4 disablecol);



    void CalcDragArea();
    void CalcCtrlPos();
    void CalcIcon();
    //------------------------------
    //nullptr, wenn keines vorhanden
    //------------------------------
    CMenu * mainmenu;
    CControllContainer * container;

    sRect DragArea;
    Base2D * DragIcon;
    LAYOUT _Layout;
    sPoint _CurrentCtrlPos;
};

#endif // TOOLBAR_H
