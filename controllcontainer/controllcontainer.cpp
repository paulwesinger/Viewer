#include "controllcontainer.h"
#include "../logs/logs.h"
#include "../utils/utils.h"
#include "../defaults.h"

CControllContainer::CControllContainer(Shader * sh, LAYOUT l) {

    _Height = 10;
    _Width = 10;
    _CurrentY = 5;
    _CurrentX = 5;

    shader = sh;
    layout = l;

    _Pos = sPoint(0,0);
}


CControllContainer::CControllContainer(Shader * sh, int px, int py, int w, int h, LAYOUT l) {

    _Height = h;
    _Width = w ;   // 2* 5
    _CurrentY = py + 5;
    _CurrentX = px + 5;

    _Pos = sPoint(px,py);

    shader = sh;
    layout = l;
}

CControllContainer::~CControllContainer() {
    releaseConterItems();
}

void CControllContainer::setLayout(LAYOUT l) {
    layout = l;
}

void CControllContainer::disableControll(Base2D *con){
    if (con != nullptr) {
        con->disable();
    }

}

void CControllContainer::enableControll(Base2D *con){
    if (con != nullptr)
        con->enable();
}

bool CControllContainer::removeContainer(CControllContainer *container) {
    // do somthing later
    return true;
}

// Remove items and give allocated memory free
void CControllContainer::releaseConterItems() {

    if ( ! controlls2D.empty()) {
        for (uint i=0; i< controlls2D.size(); i++) {
            delete controlls2D.at(i);
        }
        controlls2D.clear();
    }

    /*
    if (! controlls3D.empty() ) {
        for (uint i=0; i< controlls3D.size(); i++) {
            delete controlls3D.at(i);
        }
        controlls3D.clear();
    }

    if (! buttons.empty() ) {
        for (uint i=0; i< buttons.size(); i++) {
            delete buttons.at(i);
        }
        buttons.clear();
    }

    if (! texts.empty() ) {
        for (uint i=0; i< texts.size(); i++) {
            delete texts.at(i);
        }
        texts.clear();
    }

    */
}

void CControllContainer::setPos(sPoint pos) {
    _Pos = pos;
}
sPoint CControllContainer::Pos() {
    return _Pos;
}

sPoint CControllContainer::NextControllPos() {
    return sPoint(_CurrentX,_CurrentY);
}

void CControllContainer::setDimensions(sSize size) {
    _Dimensions = size;
}
sSize CControllContainer::Dimensions() {
    return _Dimensions;
}

void CControllContainer::enableChilds() {

}

void CControllContainer::disableChilds() {

}

void CControllContainer::DragContainer(sPoint newp) {

    int relX = newp.x - _Pos.x;
    int relY = newp.y - _Pos.y;

    _Pos.x += relX;
    _Pos.y += relY;

    _CurrentX += relX;
    _CurrentY += relY;

    for (uint i = 0; i < controlls2D.size(); i++) {
        sPoint p = controlls2D[i]->Pos();
        p.x += relX;
        p.y += relY;
        controlls2D[i]->setPos(p.x,p.y);
    }

}

void CControllContainer::DragContainer(int newx, int newy) {
    int relX = newx - _Pos.x;
    int relY = newy - _Pos.y;

    _Pos.x += relX;
    _Pos.y += relY;

    _CurrentX += relX;
    _CurrentY += relY;

    for (uint i = 0; i < controlls2D.size(); i++) {
        sPoint p = controlls2D[i]->Pos();
        p.x += relX;
        p.y += relY;
        controlls2D[i]->setPos(p.x,p.y);
    }
}

void CControllContainer::rename(std::string theNewName){
    _Name = theNewName;
}

void CControllContainer::CalcNextPos(int value) {
    if (layout == LAYOUT::Vertical)
        _CurrentY += value;
    else
        if (layout == LAYOUT::Horizontal)
            _CurrentX += value;
}

bool CControllContainer::addSpacer(){

    CalcNextPos(SPACER);
    return true;
}

bool CControllContainer::addButton(CButton *btn) {


    btn->setPos(_CurrentX,_CurrentY);
    // Gesammthöhe des container
    _Height += btn->Height();

    // Nächste Position im Container
    if (layout == LAYOUT::Vertical) {
        CalcNextPos(btn->Height() + 1);
        _Dimensions.h += _Height;
    }
    else {
        CalcNextPos(btn->Width() + 1);
        _Dimensions.w += _Height;
    }


    //buttons.push_back(btn);
    controlls2D.push_back(btn);

    loginfo("Add Button to Container ...... Done ", "CControllcontainer::addbutton");
    return  true;
}

bool CControllContainer::addControll2D(Base2D *controll) {

    controll->setPos(_CurrentX, _CurrentY);

    if (layout == LAYOUT::Vertical) {
        CalcNextPos(controll->Height()+1);
        _Dimensions.h += _Height;
    }
    else {
        CalcNextPos(controll->Width()+1);
        _Dimensions.h += _Width;
    }

    controlls2D.push_back(controll);
    loginfo("added controll2D");
    return true;
}

bool CControllContainer::addControll3D(BaseObject *baseobject)
{
    controlls3D.push_back(baseobject);
    return  true;
}

bool CControllContainer::addText(std::string text, int resx, int resy){


    // ----------------------
    // Need tobe testet !!!
    // Not used yet
    //-----------------------
    sPoint p;
    p.x = _Pos.x+ 1;
    p.y = _CurrentY;


    TextRender * t = new TextRender(resx,resy,shader);
    t->SetPos(p);
    t->AddString(text);

    if (layout == LAYOUT::Vertical ){
        CalcNextPos(t->GetTextAreaHeight() + 1);
        _Height += t->GetTextAreaHeight();
    }
    else {
        CalcNextPos(t->GetWidth() + 1);
        _Width += t->GetWidth();
    }


    return  true;
}
