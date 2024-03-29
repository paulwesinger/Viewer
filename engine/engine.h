#ifndef CENGINE_H
#define CENGINE_H

#include <vector>

#include "../initgl.h"
#include "../buttons/button.h"
#include "../textrenderer/textrender.h"
#include "../fileutils/fileutil.h"
#include "../textedit/textedit.h"
#include "../checkbox/checkbox.h"
#include "../defines.h"
#include "../defaults.h"
#include "../controllcontainer/controllcontainer.h"
#include "../menu/menu.h"
#include "../window/window.h"
#include "../toolbars/toolbar.h"
#include "../label/label.h"

const std::string BUTTONS_CFG  = PATH::ROOT + "config/Buttons/";
const std::string OBJECT3D_CFG = PATH::ROOT + "config/3DObjects/";
const std::string COCKPIT_CFG  = PATH::ROOT + "config/3DObjects/Cockpits/";
const std::string SPHERE_CFG   = PATH::ROOT + "config/3DObjects/TexturedSpheres/";

const int CFG_BUTTON_SIZE = 11;
const int CFG_3D_SIZE = 18;
const int CFG_SPHERE_SIZE = 26;

const glm::vec4 BTN_ENABLE = glm::vec4(0.2, 0.2, 0.2, 1.0);
const glm::vec4 BTN_DISABLE = glm::vec4(0.2, 0.2, 0.2, 0.3);

const int MENU_SPACER = 10;


//--------------------------------------
// Look and feel
//--------------------------------------
const int BTN_WIDTH = 150;
const int BTN_HEIGHT = 30;


class CEngine : public InitGL
{
public:
    CEngine(std::string titel = "OpenGL 4.5 Engine");
    ~CEngine();
    void Run() override;
    void Done() override;
    bool HandleMessage() override;
    void InitUserObjects() override;




protected:
    void loadButtons();
    virtual void Render()  override;
    virtual void RenderSkyBox() override;
    virtual void RenderControlls() override;

    virtual void Render2DUserObject() override;

    virtual void OnMouseMove(int &x, int &y, uint32 buttonstate) override;
    virtual void OnLeftMouseButtonUp(int &x, int &y) override;
    virtual void OnLeftMouseButtonDown( int &x, int &y) override;

    virtual void OnMainMenuStateChanged() override;

    // Lists for all objects:
    std::vector<std::string> btnList;
    std::vector<std::string> object2DList;
    std::vector<std::string> object3DTexturedList;
    std::vector<std::string> object3DColoredList;
    std::vector<std::string> object3DLandscapeList;
    std::vector<std::string> objectCockpitList;
    std::vector<std::string> objectTexturedSpheresList;

    // Submenue's and items -> this holds ALL submenus and menuitems
    std::vector<CMenu> subMenuList;

    CControllContainer *con1;
    CControllContainer *con2;
    CControllContainer * ContainerToolbar1;


    CMenu * SubMenu1;

    CButton * toogleSkyBoxBtn;
    CButton * TestButton1;
    CButton * TestButton2;

    static void functoggleSkybox();
    static void funcFog();
    static void funcTestBtn1();
    static void funcTestBtn2();


    static void functoogleAnimation(bool checked);
    static void functoogleBlend();
    static void functoogleCockpit(bool checked);
    static void DrawPanel();

    static void funcToogleSkyBox();

 private:

    void InitButtons();
    void Init2D();
    void Init3D();
    void initMenu();
    void initLandscape();
    void InitToolBar();

    // virtuelle functions
    void ShowFramesPerSec() override;
    void ShowCameraPos() override;
    void InitEngineObject() override;

    bool initButtonStruct(sButtonStruct & bs,std::vector<std::string> cfg);
    bool init3DStruct(s3DStruct & d3s, std::vector<std::string> &cfg);
    bool initLandscape(sLandscape &ls, std::vector<std::string> &cfg);
    bool initSphereStruct(sSphereStruct & sSphere, std::vector<std::string> &cfg);

    bool loadSpheres();
    bool loadTexturedCubes();
    bool loadColorCubes();
    bool loadLandscape();
    bool loadCockpits();
    //----------------------------------------------------------------
    // Menu controlls
    //----------------------------------------------------------------
    TextEdit * txtFrameperSec;
    CheckBox * checkBoxAnimation;
    CheckBox * checkBoxBlending;
    CheckBox * checkBoxCockpit;

    // Label für CameraPos :
    TextEdit * cameraX;
    TextEdit * cameraY;
    TextEdit * cameraZ;
    TextEdit * camerayaw;
    TextEdit * cameradirZ;
    TextEdit * cameradirX;

    // Toolboxes, sonstige Fenster
    Window * testToolBox;

    ToolBar * toolbar;
    std::string &getValueItem(std::string &s, std::string erasestring);
    TextRender * textrenderer;
    fileUtil * fu;

    //-----------------------------------------------------------------
    //tamplate
    //-----------------------------------------------------------------
    CButton * CreateImageButton(std::string btnreleased,std::string btnpressed,std::string btnimage, sPoint nextControllPos, FP handler);
    CButton * CreateImageButton(std::string btnreleased,std::string btnpressed,std::string btnimage, FP handler);

    //-----------------------------------------------------------------
    //tamplate
    //-----------------------------------------------------------------
    void setupTemplate(int i);
};


#endif // CENGINE_H
