#ifndef CMENU_H
#define CMENU_H

#include "../textrenderer/textrender.h"
#include "../controllcontainer/controllcontainer.h"
#include "../buttons/button.h"
#include "../checkbox/checkbox.h"
#include "../base2d/base2d.h"
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include "../defines.h"
//#include "../menuitem/menuitem.h"

#include <vector>


const int MENU_WIDTH  = 150;
const int MENU_HEIGHT = 700;

typedef struct {
    sRect Pos;
    TextRender * text;
} sMenuStruct ;

class CMenu;


class MenuItem : public Base2D
{
public:
    MenuItem(int resx, int resy, Shader * s);
    MenuItem(int resx, int resy, sPoint pos, sSize size, std::string bgtexture, std::string texttexture,  Shader * s);

    void Render() override;
    void setPos(int x, int y) override;
    void setSize(int w, int h) override;
    virtual void OnClick() override;

    void ClickFunc(FP handler);
    void setID(int id);

    int ID();

protected:
    Base2D * _TextImage;
    FP clickFunc = nullptr;

private:

    CMenu * _SubMenu;
    int _ID;
};


class CMenu :Base2D
{
public:
    CMenu(int resX, int resY,Shader * sh);
    CMenu(int resX, int resY, int w, int h,Shader * sh);
    CMenu(int resX, int resY, int w, int h, glm::vec4 bg, glm::vec4 fg,Shader * sh);
    CMenu(int resX, int resY, int px, int py, int w, int h, glm::vec4 bg, glm::vec4 fg, Shader *sh);

    void Render() override;
    int PosX() override;
    int PosY() override;
    int Width() override;
    int Height() override;

    void setMenuHeader(std::string name);
    void setActive(bool active);
    void setID(int id);
    void addMenuHeader(std::string path, int w, int h);

    void addMenuItem(MenuItem * pitem);
    MenuItem * menuItemByIndex(int listindex);
    MenuItem * menuItemByID(int id);


    //----------------------------------------
    // container functions
    //----------------------------------------
    void addConatiner( CControllContainer* con);
    void addControll2D(CControllContainer * con, Base2D * ctl);
    void addTextItem(CControllContainer * con, std::string text);
    void AlignLeft();
    void AlignRight();
    void DrawBackground(bool visible);

    std::vector<sMenuStruct> menuItems;
    std::vector<CControllContainer *> containerList;
    std::vector<MenuItem> itemList;
    std::vector <Base2D *> controlls2D;

    bool Active();
    int CurrentY();
    int ID();

protected:

    bool alignright;
    bool alignleft;
    bool drawBackground = false;

private:

    Base2D *_Header;
    Base2D * _HeaderText;
    std::string _PathToHeaderText;

    void init();
    int _CurrentY; //hilfsvariable für nächste freie y-positon im Menu - relativ zu bildschirm
    bool _Active;

    sPoint posContainer;
    int _ID;

};

#endif // CMENU_H
