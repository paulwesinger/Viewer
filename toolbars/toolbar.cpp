#include "toolbar.h"
//#include <glm/vec4.hpp>

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
    else{
        Stretch();
        Window::Render();
    }
    DragIcon->Render();

    for (int i = 0;i < containerList.size(); i++) {
        for (int j=0; j<containerList[i]->controlls2D.size(); j++ ) {
            containerList[i]->controlls2D[j]->Render();
        }
    }
}

void ToolBar::Stretch() {
    if (mainmenu != nullptr)  {
        if (mainmenu->Active()) {
            _Pos.x = mainmenu->Pos().x + mainmenu->Width();
            _Size.w = _ResX -_Pos.x;
            CalcDragArea();
        }
        else{
            _Pos.x = 0;
            _Size.w = _ResX;
            CalcDragArea();
        }
    }
    else {
            _Pos.x = 0;
            _Size.w = _ResX;
            CalcDragArea();
         }
}

void ToolBar::CalcDragArea() {
    DragArea.x  = _Pos.x;
    DragArea.y  = _Pos.y;
    DragArea.x1 = _Pos.x + _Size.h;
    DragArea.y1 = _Pos.y + _Size.h;

    DragIcon->setPos(DragArea.x,DragArea.y);
}

void ToolBar::Init() {
    _Pos.x = 100;
    _Pos.y = 0;
    _Size.w = _ResX - 100;
    _Size.h = 30;
    _Color = glm::vec4(0.8,0.8,1,0.5);

    DragArea.x  = _Pos.x;
    DragArea.y  = _Pos.y;
    DragArea.x1 = _Pos.x + _Size.h;
    DragArea.y1 = _Pos.y + _Size.h;

    DragIcon = new Base2D(_ResX,_ResY,shader);
    DragIcon->setPos(DragArea.x,DragArea.y);
    DragIcon->setSize(DragArea.x1-DragArea.x, DragArea.y1-DragArea.y);
    DragAreaColor = glm::vec4(1,1,0,1);

    mainmenu = nullptr;

    _Layout = LAYOUT::Horizontal;
}

void ToolBar::setMenuPtr(CMenu * ptr) {
    mainmenu = ptr;
}

CMenu * ToolBar::getMenuPtr() {
    return mainmenu;
}

void ToolBar::addConatiner(CControllContainer *con) {

    if ( con == nullptr )
        return;
    containerList.push_back(con);

    /*
    _currentY += con->Dimensions().h + 1;
    loginfo("Add Controllcontainer to Menu .... Done"  ,"CMenu::addContainer");
    */

}
