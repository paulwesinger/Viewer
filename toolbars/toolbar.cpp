#include "toolbar.h"
//#include <glm/vec4.hpp>



const sPoint operator+ (sPoint const &p1 , sPoint const &p2 ){
    sPoint tmp;
    tmp.x = p1.x + p2.x;
    return tmp;
}


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

    Window::Render();
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
            CalcCtrlPos();
        }
        else{
            _Pos.x = 0;
            _Size.w = _ResX;
            CalcDragArea();
            CalcCtrlPos();
        }
    }
    else {
            _Pos.x = 0;
            _Size.w = _ResX;
            CalcDragArea();
            CalcCtrlPos();
         }
}

int ToolBar::CurrentCtlPos_X() {return _CurrentCtrlPos.x;}
int ToolBar::CurrentCtlPos_Y() {return _CurrentCtrlPos.y;}
sPoint ToolBar::CurrentCtlPos() {return _CurrentCtrlPos;}



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

    CalcCtrlPos();

    DragIcon = new Base2D(_ResX,_ResY,shader);
    DragIcon->setPos(DragArea.x+2,DragArea.y+2);
    DragIcon->setColor(glm::vec4(0,0,1,0.5));
    DragIcon->setSize(DragArea.x1-DragArea.x, DragArea.y1-DragArea.y);

    mainmenu = nullptr;

    _Layout = LAYOUT::Horizontal;
}

bool ToolBar::intersect(int x, int y) {


    return  ( ((x > DragArea.x) && (x < DragArea.x1) ) &&
              ((y > DragArea.y) && (y < DragArea.y1) ) ) ? true : false;
}

void ToolBar::OnMainMenuStateChanged() {
    Stretch();
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

    if (_Pos.x < 0)
        _Pos.x = 0;

    if (_Pos.x + Width() > _ResX )
        _Pos.x = _ResX -Width();

    if (_Pos.y < 0)
        _Pos.y = 0;

    if (_Pos.y + Height() > _ResY )
        _Pos.y = _ResY  - Height();


    Stretch();
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

    if (ptr != nullptr) {
        mainmenu = ptr;

        if (ptr->Active() ) {
            _Pos.x = ptr->Pos().x + ptr->Width();
            _Pos.y = ptr->Pos().y;
        }
        else{
            _Pos.x = 0;
        }
    }
    else {
        _Pos.x = 0;
        _Pos.y = 0;
    }
    CalcDragArea();
    CalcCtrlPos();
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
        ctl->setPos(_CurrentCtrlPos.x,_CurrentCtrlPos.y);

        if (_Layout == LAYOUT::Horizontal)
            _CurrentCtrlPos.x += ctl->Width() + 2;
        else
            _CurrentCtrlPos.y += ctl->Height() + 2;

        CtrlList.push_back(ctl);
    }
}
