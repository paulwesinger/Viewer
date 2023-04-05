#include "menu.h"
#include "../logs/logs.h"
#include "../utils/utils.h"
#include "../defaults.h"


MenuItem::MenuItem(int resx,int resy, Shader * sh)
    :Base2D(resx,resy,sh)
{
    _TextImage = new Base2D(resx,resy,sh);
    _SubMenu  = nullptr;
    _ID = 0 ;
}

MenuItem::MenuItem(int resx, int resy, sPoint pos, sSize size, std::string bgtexture, std::string texttexture,  Shader * s)
    :Base2D(resx,resy,bgtexture,s)
{


    Base2D::setPos(pos.x,pos.y);
    Base2D::setSize(size.w, size.h);

    _TextImage = new Base2D(resx,resy,texttexture,s);
    _TextImage->setPos(pos.x, pos.y);
    _TextImage->setSize(size.w, size.h);
}

void MenuItem::setPos (int x, int y) {
    Base2D::setPos(x,y);
    if (_TextImage)
        _TextImage->setPos(x,y);
}

void MenuItem::setSize(int w, int h){
    Base2D::setSize(w, h) ;
    if (_TextImage)
        _TextImage->setSize(w,h);
}

void MenuItem::Render() {

    Base2D::Render();
    if (_TextImage)
        _TextImage->Render();
}

void MenuItem::setID(int id) {
    _ID = id;
}

int MenuItem::ID() { return _ID; }

void MenuItem::OnClick() {
  //  if (HoverFunc != nullptr)
        clickFunc();
}

void MenuItem::ClickFunc(FP handler) {
    //if (HoverFunc != nullptr)
        clickFunc = handler;
}



CMenu::CMenu(int resX, int resY, Shader * sh)
    :Base2D(resX,resY,sh)
    {
    /*
    _resY = resY;
    _resX = resX;

    //shader = sh;

    posX = 0;
    posY = 0;

    width =  MENU_WIDTH;
    height = MENU_HEIGHT;

    backgroundColor = glm::vec4(0.0,0.0,0.0,1.0);
    foregroundColor = glm::vec4(1.0,1.0,1.0,1.0);
*/
    init();
}

CMenu::CMenu(int resX, int resY, int w, int h, Shader * sh)
    :Base2D(resX,resY,sh)
{
    setWidth(w);
    setHeight(h);

    init();
}

CMenu::CMenu(int resX, int resY, int w, int h, glm::vec4 bg, glm::vec4 fg, Shader * sh)
    :Base2D(resX,resY,sh)
{
    setWidth(w);
    setHeight(h);
    setBackgroundColor(bg);
    setColor(fg);

    init();
}

CMenu::CMenu(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg, glm::vec4 fg, Shader * sh)
    :Base2D(resX,resY,sh)
{

    shader = sh;

    setPos(px,py);
    setSize(w,h);

    setBackgroundColor(bg);
    setColor(fg);

    init();
}

void CMenu::init() {

    _CurrentY = 2;
    _Active = true;

    _Header= nullptr;
    _HeaderText = nullptr;
    _PathToHeaderText = "";
   // posContainer.x = PosX();
   // posContainer.y = PosY() + 5;
}

void CMenu::addMenuHeader(std::string path, int w, int h) {


    if (_Header == nullptr)
        _Header = new Base2D(_ResX,_ResY,path,shader);

    _Header->setPos(PosX()+1,PosY());
    _Header->setWidth(Width() -2 );
    _Header->setHeight(h);
    _Header->setColor(glm::vec4(0,0,0.8,0.5));

    _CurrentY += h;
    int height = Height();
    height += h;
  //  width = w+2;
    setHeight(height);
}

void CMenu::setActive(bool active) {
    _Active = active;
}

void CMenu::DrawBackground(bool visible) {
    drawBackground = visible;
}

bool CMenu::Active() {
    return _Active;
}

int CMenu::CurrentY(){
    return _CurrentY;
}


int CMenu::PosX() { return  Base::PosX(); }
int CMenu::PosY() { return Base::PosY(); }
int CMenu:: Width() { return Base::Size().w; }
int CMenu::Height() { return Base::Size().h; }
int CMenu::ID() { return _ID; }


void CMenu::Render() {

    if (drawBackground)
        Base2D::Render();

    if (_Header != nullptr)
        _Header->Render();

 //   if (! containerList.empty() ) {
     //   for(uint i=0; i < containerList.size(); i++) {
     //       if ( ! containerList.at(i) ->controlls2D.empty() ) {
                for (uint j=0; j < controlls2D.size(); j++){
                    controlls2D.at(j)->Render();
                 }
      //      }
     //   }
   // }
}



void CMenu::setMenuHeader(std::string name) {
    sMenuStruct ms;
    ms.text = new TextRender(_ResX,_ResY, shader);
    ms.text->AddString(name);

    _CurrentY += ms.text->GetTextAreaHeight();
}

void CMenu::addControll2D(CControllContainer* con, Base2D * ctl) {

    //if ( con == nullptr  || ctl == nullptr )
    if ( ctl == nullptr )
        return;
    int  height = Height();
    ctl->setWidth(Width()-4);
    ctl->setHeight(ctl->Height());
    ctl->setPos(PosX()+2, _CurrentY-1);

    height += ctl->Height();
    setHeight(height+2);
    _CurrentY  += ctl->Height()+2;
    controlls2D.push_back(ctl);

    //con->addControll2D(ctl);
}

void CMenu::addConatiner(CControllContainer *con) {

    if ( con == nullptr )
        return;

    con->setPos(posContainer);
    containerList.push_back(con);


    _CurrentY = con->Dimensions().h + 1;
    posContainer.y += _CurrentY;

    setHeight((_CurrentY - PosY())  + 3);
    loginfo("Add Controllcontainer to Menu .... Done"  ,"CMenu::addContainer");

}

void CMenu::addTextItem(CControllContainer *con, std::string text) {
    if ( con == nullptr)
        return;

    // Hier eigentlich header text..! container anpassen !!
}

void CMenu::setID(int id) {
    _ID = id;
}

void CMenu::AlignLeft() {

    alignleft = true;
    alignright = false;
}

void CMenu::AlignRight() {

    alignright = true;
    alignleft = false;
}
