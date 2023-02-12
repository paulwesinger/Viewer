#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <string>
#include "../window/window.h"
#include "../menu/menu.h"
#include "../shaders/shader.h"
#include "../menu/menu.h"
#include "../controllcontainer/controllcontainer.h"
#include <glm/vec4.hpp>



class ToolBar : public Window
{
public:
    ToolBar(int resX, int resY,Shader * sh);
    ToolBar(int resX, int resY, int w, int h,Shader * sh);
    ToolBar(int resX, int resY, int w, int h, glm::vec4 bg,  glm::vec4 fg, Shader * sh, std::string path="");
    ToolBar(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg, glm::vec4 fg, Shader *sh, std::string path="");

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
    void CalcDragArea();
    void CalcCtrlPos();

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
