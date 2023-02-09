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

    for (uint i = 0; i<CtrlList.size(); i++)    {
        CtrlList.at(i)->Render();
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
    DragArea.x1 = _Pos.x + 30;
    DragArea.y1 = _Pos.y + 30;

    DragIcon->setPos(DragArea.x,DragArea.y);
}

void ToolBar::CalcCtrlPos() {
    _CurrentCtrlPos.x = DragArea.x1 + 2;
    _CurrentCtrlPos.y = DragArea.y + 2;
}

void ToolBar::Init() {
    _Pos.x = 100;
    _Pos.y = 0;
    _Size.w = _ResX - 100;
    _Size.h = 34;
    _Color = glm::vec4(0.8,0.8,1,0.5);


    DragArea.x  = _Pos.x;
    DragArea.y  = _Pos.y;
    DragArea.x1 = _Pos.x + 30;
    DragArea.y1 = _Pos.y + 30;



    DragIcon = new Base2D(_ResX,_ResY,shader);
    DragIcon->setPos(DragArea.x,DragArea.y);
    DragIcon->setSize(DragArea.x1-DragArea.x, DragArea.y1-DragArea.y);
    DragAreaColor = glm::vec4(1,1,0,1);

    mainmenu = nullptr;
    CalcCtrlPos();
    _Layout = LAYOUT::Horizontal;
}

bool ToolBar::intersect(int x, int y) {


    return  ( ((x > DragArea.x) && (x < DragArea.x1) ) &&
              ((y > DragArea.y) && (y < DragArea.y1) ) ) ? true : false;
}

void ToolBar::OnStartDrag(int mx, int my) {

    Base::OnStartDrag(mx,my);

    CalcDragArea();

    CalcCtrlPos();

    for (uint i = 0; i < CtrlList.size(); i ++) {

        if (_Layout == LAYOUT::Horizontal) {
            CtrlList.at(i)->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
            _CurrentCtrlPos.x += CtrlList.at(i)->Width()+2;
        }
        else
        {
            CtrlList.at(i)->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
            _CurrentCtrlPos.y += CtrlList.at(i)->Height() + 2;
        }
    }
}

void ToolBar::OnDrag(int mx, int my) {

    Base::OnDrag(mx,my);

    CalcCtrlPos();

    for (uint i = 0; i < CtrlList.size(); i ++) {

        if (_Layout == LAYOUT::Horizontal) {
            CtrlList.at(i)->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
            _CurrentCtrlPos.x += CtrlList.at(i)->Width()+2;
        }
        else
        {
            CtrlList.at(i)->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
            _CurrentCtrlPos.y += CtrlList.at(i)->Height() + 2;
        }
    }

}

void ToolBar::OnEndDrag(int mx, int my){
    Base::OnEndDrag(mx,my);

    CalcDragArea();

    CalcCtrlPos();

    for (uint i = 0; i < CtrlList.size(); i ++) {

        if (_Layout == LAYOUT::Horizontal) {
            CtrlList.at(i)->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
            _CurrentCtrlPos.x += CtrlList.at(i)->Width()+2;
        }
        else
        {
            CtrlList.at(i)->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
            _CurrentCtrlPos.y += CtrlList.at(i)->Height() + 2;
        }
    }
}

void ToolBar::setMenuPtr(CMenu * ptr) {
    mainmenu = ptr;
}

CMenu * ToolBar::getMenuPtr() {
    return mainmenu;
}

void ToolBar::setLayOut(LAYOUT l){
    _Layout = l;
}

void ToolBar::addConatiner(CControllContainer *con) {

    if ( con == nullptr )
        return;
    containerList.push_back(con);
}

void ToolBar::addCtrl(Base2D * ctl) {


    if (ctl != nullptr ) {
        ctl->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);
        if (_Layout == LAYOUT::Horizontal)
            _CurrentCtrlPos.x += ctl->Width() + 2;
        else
            _CurrentCtrlPos.y += ctl->Height() + 2;

        CtrlList.push_back(ctl);
    }
}
