#include "engine.h"
#include "../utils/utils.h"
#include "../landscape/landscape.h"
#include "../sphere/sphere.h"
#include "../menuids.h"

// Menus
const std::string MENUBACKGROUNDS = PATH::ROOT + "images/MenuBackgrounds/";
const std::string MENUTEXTS       = PATH::ROOT + "images/MenuTexts/";


const std::string MENU_MAINTEXT   = MENUTEXTS + "MainMenu.png";

const std::string MENU_BACKGROUND   = MENUBACKGROUNDS   + "MenuItemBackground.png";
const std::string MENU_BACKGROUNDSUB   = MENUBACKGROUNDS   + "MenuItemBackgroundSub.png";

//const std::string MENU_SKYBOX     = MENUTEXTS   + "ToogleSkybox.png";
const std::string MENU_FILE       = MENUTEXTS   + "Datei.png";


// Buttons
const std::string BTNBACKGROUNDS = PATH::ROOT + "images/ButtonBackgrounds/";
const std::string BTNTEXTS       = PATH::ROOT + "images/ButtonTexts/";

const std::string BTN_RELEASED   = BTNBACKGROUNDS   + "ButtonReleased.png";

// Texture for text on button
const std::string BTN_SKYBOX     = BTNTEXTS   + "ToogleSkybox.png";




// macro
#define safeDelete(pt) \
    if (pt != nullptr) \
        delete pt;

void FxFog() {
    loginfo("TestFunc2","Fog");
}

//---------------------------------
// DrawPanel
//---------------------------------
void CEngine::DrawPanel() {
    InitGL::togglePanel2D();
}

CEngine::CEngine(std::string titel) :
        InitGL(titel){
    con1                = nullptr;
    con2                = nullptr;

    txtFrameperSec      = nullptr;
    checkBoxAnimation   = nullptr;
    toogleSkyBoxBtn     = nullptr;

    TestButton1         = nullptr;
    TestButton2         = nullptr;

    cameraX             = nullptr;
    cameraY             = nullptr;
    cameraZ             = nullptr;

    toolbar             = nullptr;
    ContainerToolbar1   = nullptr;

    SubMenu1            = nullptr;

    //From InitGL:
    _ShowFramesPerSecond = true;

  //  _FullScreen = true;
  //  _ResX = FULLSCREEN_WIDTH;
  //  _ResY = FULLSCREEN_HEIGHT;
    _FullScreen = false;
    _ResX = SD_WIDTH;
    _ResY = SD_HEIGHT;
}

CEngine::~CEngine(){

    safeDelete (con1);
    safeDelete (con2);

    //safeDelete(fogBtn);
}

void CEngine::Done() {
    InitGL::Done();
}

void CEngine::Run() {

    InitGL::Run();
}

void CEngine::InitEngineObject(){
    InitGL::InitEngineObject();

    Init2D();
    Init3D();

    InitButtons();
    initMenu();
    InitToolBar();
}

void CEngine::OnMouseMove(int &x, int &y, uint32 buttonstate) {
   InitGL::OnMouseMove(x,y,buttonstate);

    if (toolbar != nullptr ) {
        if (toolbar->IsDragging())
            toolbar->OnDrag(x,y);
    }
}

void CEngine::OnMainMenuStateChanged() {
    toolbar->OnMainMenuStateChanged();
}

void CEngine::OnLeftMouseButtonUp(int &x, int &y) {
    InitGL::OnLeftMouseButtonUp(x,y);

    if ( ! toolbar->CtrlList.empty() ) {

       //     if ( _LockClick) {
                for (uint j=0; j< toolbar->CtrlList.size(); j ++) {
                    if (toolbar->CtrlList.at(j)->intersect(x, y) ) {
                        toolbar->CtrlList.at(j)->OnRelease();
                    }
                }

                if (toolbar->intersect(x,y)) {
                    toolbar->OnEndDrag(x,y);
                }
       //     }
       //     else
       //         _LockClick = false;
        }
}

void CEngine::OnLeftMouseButtonDown( int &x, int &y){
    InitGL::OnLeftMouseButtonDown(x,y);



        if ( ! toolbar->CtrlList.empty()) {

            //if ( _LockClick) {
                for (uint j=0; j< toolbar->CtrlList.size(); j ++) {
                    if (toolbar->CtrlList.at(j)->intersect(x, y) ) {
                        toolbar->CtrlList.at(j)->OnClick();

                    }
                }
                if (toolbar->intersect(x,y) ) {
                    toolbar->OnStartDrag(x,y);
                }
            //}
            //else
            //    _LockClick = false;
        }
}

bool CEngine::HandleMessage() {


    InitGL::HandleMessage();
    SDL_FlushEvent(SDL_MOUSEMOTION);

    SDL_Event event;
    SDL_PollEvent(&event);

    uint32_t buttons;
    switch(_Event.type) {

        //------------------------------------------------------------------------------
        // Mause Events
        //------------------------------------------------------------------------------
        case      SDL_MOUSEMOTION : {

            _Mouse.x = event.motion.x;
            _Mouse.y = event.motion.y;

            buttons = SDL_GetMouseState(&event.motion.x, &event.motion.y);
            OnMouseMove(event.motion.x, event.motion.y, buttons);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {

            if ( _Event.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonDown(event.motion.x, event.motion.y);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {

            if ( _Event.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonUp(event.motion.x, event.motion.y);
            }
            break;
        }

    }

    SDL_FlushEvent(SDL_MOUSEMOTION);
    return true;
}

void CEngine::InitUserObjects() {
    InitGL::InitUserObjects();

}

// ---------------------------------------------------------------
// Place here Functons for button handler..
// ---------------------------------------------------------------

void CEngine::functoogleAnimation(bool checked) {

    _Animate = !_Animate;
    //InitGL::toggleAnimation();  // eigntlich startanimation..

}

void CEngine::functoogleBlend(bool checked) {

       InitGL::toggleBlend();
}

void CEngine::functoggleSkybox() {
    InitGL::toggleSkyBox();
}

void CEngine::funcFog(){
    loginfo("Fog - function");
}

void CEngine::funcTestBtn1() {

}

void CEngine::funcTestBtn2() {

}



void CEngine::Render() {

    InitGL::Render();



 //   if (toolbar != nullptr)
 //       toolbar->Render();
}

void CEngine::RenderControlls() {
    InitGL::RenderControlls();

    // Here Stoff for 2D Rendering !

  //  if (SubMenu1 != nullptr)
  //      SubMenu1->Render();
}

void CEngine::RenderSkyBox() {

    if (_ShowSkybox)
        InitGL::RenderSkyBox();
}

void CEngine::functoogleCockpit(bool checked) {
    InitGL::toogleCockpit();
}


// --------------------------------------------------------------
// Prvates for Creating
// --------------------------------------------------------------
CButton * CEngine::CreateImageButton(std::string btnreleased,std::string btnpressed, std::string btnImage, sPoint nextControllPos, FP handler) {

    CButton * b = new CImageButton(_ResX, _ResY, btnreleased, btnpressed,btnImage, nextControllPos,InitGL::getShaderPtr());


    b->setPos(nextControllPos.x, nextControllPos.y);
    b->setDisablecolor(BTN_DISABLE);
    b->setSize(BTN_WIDTH,BTN_HEIGHT);
    b->AddHandler(handler);

    return b;
}

CButton * CEngine::CreateImageButton(std::string btnreleased, std::string btnpressed,std::string btnImage, FP handler) {
    CButton * b = new CImageButton(_ResX, _ResY, btnreleased,btnpressed, btnImage, InitGL::getShaderPtr());

    sPoint p(0,0);
    b->setPos(p.x,p.y);
    b->setDisablecolor(BTN_DISABLE);
    b->setSize(BTN_WIDTH,BTN_HEIGHT);
    b->AddHandler(handler);

    return b;
}


// --------------------------------------------------------------
// Init Toolbar
// --------------------------------------------------------------
void CEngine::InitToolBar() {

    if (MainMenu != nullptr)
        toolbar->setMenuPtr(MainMenu);

    TestButton1 = CreateImageButton(BTN_RELEASED,"" ,BTN_SKYBOX, toolbar->CurrentCtlPos(), CEngine::funcTestBtn1);
    TestButton2 = CreateImageButton(BTN_RELEASED, "" ,BTN_SKYBOX, toolbar->CurrentCtlPos(), CEngine::funcTestBtn2);

    toolbar->addCtrl(TestButton1);
    toolbar->addCtrl(TestButton2);
}


// --------------------------------------------------------------
// Init Methods for Engines
// --------------------------------------------------------------
void CEngine::initMenu(){



//                |Resolution|  | Position           | width,height, colors             |
    MainMenu = new CMenu(_ResX, _ResY, 0, 0, MENU_WIDTH, 0/*MENU_HEIGHT*/,
                          glm::vec4(0.1,0.1,0.1,0.8), glm::vec4(0.9,0.9,0.9,1.0), InitGL::getShaderPtr());

    MainMenu->setID(MENUIDS::MENU_Main);

    MainMenu->DrawBackground(true);



    // -------------------------------------
    // Standard Menu ist in Initgl vorhanden
    // jetzt  befüllen
    //--------------------------------------
    con1 = new CControllContainer(InitGL::getShaderPtr(), MainMenu->PosX(),MainMenu->PosY(),
                                               MainMenu->Width(), 0, LAYOUT::Vertical);
    sPoint p;
    p.x = 0;
    p.y =0;

    sSize s;
    s.w = MainMenu->Width() - CONTROLL::MARGIN_X;
    s.h = CONTROLL::HEIGHT;

    Label * menuHeader = new Label(_ResX, _ResY, MENU_BACKGROUND, MENU_MAINTEXT, InitGL::getShaderPtr());
    menuHeader->setSize(s.w, CONTROLL::HEIGHT);
    MainMenu->addControll2D(con1,menuHeader);
    p.y = MainMenu->CurrentY();

    MenuItem * item1 = new MenuItem(_ResX,_ResY,p,sSize(BTN_WIDTH, BTN_HEIGHT),MENU_BACKGROUNDSUB,BTN_SKYBOX,InitGL::getShaderPtr() );
    item1->setID(FILEMENU_IDS::ITEM_New);
    MainMenu->addControll2D(con1,item1);

    p.y = MainMenu->CurrentY();
    checkBoxAnimation = new CheckBox(_ResX, _ResY,BTN_RELEASED, p,s ,
                                     glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr() );

    checkBoxAnimation->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    checkBoxAnimation->AddHandler(CEngine::functoogleAnimation);
    checkBoxAnimation->setChecked();
    checkBoxAnimation->setLabel("Animate");
    MainMenu->addControll2D(con1,checkBoxAnimation);

    //----------------------------------------------------
    // checkbox für Blending
    //----------------------------------------------------
    SubMenu1 = new CMenu(_ResX, _ResY, item1->PosX() + item1->Width()+ 5 , item1->PosY(), MENU_WIDTH, MENU_HEIGHT,
                         glm::vec4(0.1,0.1,0.1,0.8), glm::vec4(0.9,0.9,0.9,1.0), InitGL::getShaderPtr());

    SubMenu1->setID(MENUIDS::MENU_File);
    SubMenu1->DrawBackground(true);

    // Hier aus einer liste id'S holen, submenu

    /*
    if (skybox != nullptr) {
        toogleSkyBoxBtn = CreateImageButton(BTN_RELEASED, "" ,BTN_SKYBOX,
                                            con1->NextControllPos(),CEngine::functoggleSkybox);

        con1->addControll2D(toogleSkyBoxBtn);
        con1->addSpacer();
        MainMenu->addConatiner(con1);
        curr_y = MainMenu->CurrentY() + MENU_SPACER;
    }

    //---------------------------------------------------
    // 2. container
    //---------------------------------------------------
    con2 = new CControllContainer(InitGL::getShaderPtr(),MainMenu->Pos().x,
                                             curr_y,
                                  MainMenu->Width(),0, LAYOUT::Vertical);
    //-----------------------------------------------------
    // Textedit:
    //-----------------------------------------------------
    sSize s;
    //sPoint p;
    con2->addSpacer();
    s.w = MainMenu->Width() - CONTROLL::MARGIN_X ;
    s.h = CONTROLL::HEIGHT;

    p = con2->NextControllPos();

    txtFrameperSec = new TextEdit(_ResX, _ResY,BTN_RELEASED, p,s,
                                  glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0),InitGL::getShaderPtr());


    //Setting the color for the outputlabel
    con2->addControll2D(txtFrameperSec);
    // add label for Frames to buildin textrender label
    txtFrameperSec->setLabel("FPS");

    //----------------------------------------------------
    // checkbox für Animation
    //----------------------------------------------------
    p = con2->NextControllPos();

    checkBoxAnimation = new CheckBox(_ResX, _ResY,BTN_RELEASED, p,s ,
                                     glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr() );

    checkBoxAnimation->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    checkBoxAnimation->AddHandler(CEngine::functoogleAnimation);
    checkBoxAnimation->setChecked();
    con2->addControll2D(checkBoxAnimation);
    // add label for Frames to buildin textrender label
    checkBoxAnimation->setLabel("Animate");

    //----------------------------------------------------
    // checkbox für Blending
    //----------------------------------------------------
    p = con2->NextControllPos();

    checkBoxBlending = new CheckBox(_ResX, _ResY, BTN_RELEASED, p,s,
                                    glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr() );

    checkBoxBlending->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    checkBoxBlending->AddHandler(CEngine::functoogleBlend);
    checkBoxBlending->setChecked();
    con2->addControll2D(checkBoxBlending);
    // add label for Frames to buildin textrender label
    checkBoxBlending->setLabel("Blending");

    //----------------------------------------------------
    // checkbox für Cockpit
    //----------------------------------------------------
    p = con2->NextControllPos();

    checkBoxCockpit = new CheckBox(_ResX, _ResY, BTN_RELEASED, p,s,
                                   glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr() );

    checkBoxCockpit->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    checkBoxCockpit->AddHandler(CEngine::functoogleCockpit);
    checkBoxCockpit->setChecked();
    con2->addControll2D(checkBoxCockpit);
    // add label for Frames to buildin textrender label
    checkBoxCockpit->setLabel("Cockpit");

    // ---------------------------------------------------
    // Statusfenster(pos) von Camera:
    //----------------------------------------------------

    p = con2->NextControllPos();
    toogleSkyBoxBtn = new CTextButton(_ResX,_ResY,BTN_RELEASED, "Test",p,InitGL::getShaderPtr());
    toogleSkyBoxBtn->setSize(BTN_WIDTH,BTN_HEIGHT);
    toogleSkyBoxBtn->AddHandler(CEngine::funcTestBtn2);

    //toogleSkyBoxBtn->setSize(MainMenu->)
    con2->addControll2D(toogleSkyBoxBtn);
    con2->addSpacer();

    p = con2->NextControllPos();
    cameraX = new TextEdit(_ResX, _ResY, BTN_RELEASED, p,s,
                  glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr());

    cameraX->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(cameraX);
    // add label for Frames to buildin textrender label
    cameraX->setLabel("Cam X");

    p = con2->NextControllPos();
    cameraY = new TextEdit(_ResX, _ResY, BTN_RELEASED, p,s
                  ,glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0),InitGL::getShaderPtr());

    cameraY->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(cameraY);
    // add label for Frames to buildin textrender label
    cameraY->setLabel("Cam Y");

    p = con2->NextControllPos();
    cameraZ = new TextEdit(_ResX, _ResY, BTN_RELEASED, p,s,
                           glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0),InitGL::getShaderPtr());

    cameraZ->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(cameraZ);
    // add label for Frames to buildin textrender label
    cameraZ->setLabel("Cam Z");

    p = con2->NextControllPos();
    camerayaw = new TextEdit(_ResX, _ResY, BTN_RELEASED, p,s,
                             glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr());

    camerayaw->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(camerayaw);
    // add label for Frames to buildin textrender label
    camerayaw->setLabel("Cam Yaw");

    p = con2->NextControllPos();
    cameradirZ = new TextEdit(_ResX, _ResY, BTN_RELEASED, p,s,
                              glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0), InitGL::getShaderPtr());

    cameradirZ->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(cameradirZ);
    // add label for Frames to buildin textrender label
    cameradirZ->setLabel("CamDir.Z");

    MainMenu->addConatiner(con2);

    */
}

void CEngine::ShowFramesPerSec() {
/*
    TextRender * t = txtFrameperSec->getTextPtr();
    if (t != nullptr) {
        t-> SetTextColor(glm::vec4(1.0,1.0,1.0,100));
        t->SetText(0,IntToString(_FramerateOut));
        t->Render();
    }
    */
}

void CEngine::ShowCameraPos() {
    if (cameraX != nullptr) {
        TextRender * tX = cameraX->getTextPtr();
        if (tX != nullptr) {
            tX->SetText(0, FloatToString(camera->GetPos().x));
            tX->Render();
        }
    }

    if (cameraY != nullptr ) {
        TextRender * tY = cameraY->getTextPtr();
        if (tY != nullptr) {
            tY->SetText(0, FloatToString(camera->GetPos().y));
            tY->Render();
        }
    }

    if (cameraZ != nullptr) {
        TextRender * tZ = cameraZ->getTextPtr();
        if (tZ != nullptr) {
            tZ->SetText(0, FloatToString(camera->GetPos().z));
            tZ->Render();
        }
    }

    if (camerayaw != nullptr) {
        TextRender * tyaw = camerayaw->getTextPtr();
        if (tyaw != nullptr) {
            tyaw->SetText(0, FloatToString(camera->YawCameraDEG()));
            tyaw->Render();
        }
    }

    if (cameradirZ != nullptr) {
        TextRender * tdir = cameradirZ->getTextPtr();
        if (tdir != nullptr) {
            tdir->SetText(0, FloatToString(camera->GetDir().z));
            tdir->Render();
        }
    }

    if (cameradirX != nullptr) {
        TextRender * tdir = cameradirX->getTextPtr();
        if (tdir != nullptr) {
            tdir->SetText(0, FloatToString(camera->GetDir().x));
            tdir->Render();
        }
    }
}

void CEngine::Init2D() {

    //================================
    // Init 2D Objects
    // ==========y=====================

    // Testweise mal einfügen
    loginfo("============================");
    loginfo("Erstelle 2D Objekte.........");
    loginfo("============================");
    logimage("Erstelle TestIcon..........");
    base2d = new Base2D(_ResX, _ResY,PATH::ROOT + "icons/ibus-setup-hangul.png",getShaderPtr());
    base2d->setPos(500,400);


    testToolBox = new Window(_ResX, _ResY,getShaderPtr());
   // testToolBox->setWidth(400);
   // testToolBox->setHeight(80);
    testToolBox-> setPos(1000,800);

    toolbar = new ToolBar(_ResX,_ResY,InitGL::getShaderPtr());
    add2Dobject(base2d);
    add2Dobject(testToolBox);
    add2Dobject(toolbar);
}

void CEngine::Render2DUserObject() {
    InitGL::Render2DUserObject();
}

void CEngine::Init3D(){

   logEmptyLine();
   loginfo("============================");
   loginfo("Erstelle 3D Objekte.........");
   loginfo("============================");

   fileUtil * fileutil = new fileUtil();
   bool ok;
   //--------------------------------------
   // loading Textured cubes
   //--------------------------------------
   ok = fileutil->readLine(OBJECT3D_CFG + "TexturedCubes/TexCubes.cfg", object3DTexturedList);
   if (ok) {
        if ( ! loadTexturedCubes() )
            logwarn("Fehler: Keine Textured Cubes gefunden oder Fehler im Pfad!");
   }
   else
       logwarn("Fehler: Datei  < config/TexCubes.cfg  > nicht gefunden !");


   //--------------------------------------
   // loading Textured Spheres
   //--------------------------------------
   ok = fileutil->readLine(SPHERE_CFG + "Spheres.cfg", objectTexturedSpheresList);
   if (ok) {
        if ( ! loadSpheres() )
            logwarn("Fehler: Keine Textured Spheres gefunden oder Fehler im Pfad!");
   }
   else
       logwarn("Fehler: Datei  < config/Spheres.cfg  > nicht gefunden !");

   // --------------------------------------
   // colored cubes loading
   //---------------------------------------
 //  ok = fileutil->readLine(OBJECT3D_CFG + "ColoredCubes/ColorCubes.cfg", object3DColoredList);
 //  if (ok) {
 //       if ( ! loadColorCubes() )
 //           logwarn("Fehler: Keine Colored Cubes gefunden oder Fehler im Pfad!");
 //  }
 //  else
 //      logwarn("Fehler: Datei  < config/ColorCubes.cfg  > nicht gefunden !");


   // --------------------------------------
   // Landscape loading
   //---------------------------------------
   ok = fileutil->readLine(OBJECT3D_CFG + "Landscapes/Landscapes.cfg", object3DLandscapeList);
   if (ok) {
        if ( ! loadLandscape() )
            logwarn("Fehler: Keine Landscape gefunden oder Fehler im Pfad!");
   }
   else
       logwarn("Fehler: Datei  < config/Landscape.cfg  > nicht gefunden !");

   // --------------------------------------
   // Cockpit loading
   //---------------------------------------
   ok = fileutil->readLine(COCKPIT_CFG + "Cockpit.cfg", objectCockpitList);
   if (ok) {
        if ( ! loadCockpits() )
            logwarn("Fehler: Keine Cockpits gefunden oder Fehler im Verzeichnißpfad!");
   }
   else
       logwarn("Fehler: Datei  < config/Landscape.cfg  > nicht gefunden !");


   logEmptyLine() ;
   loginfo("----------------------------");
   loginfo(" Init 3D Objects ");
   loginfo("     READY");
   loginfo("----------------------------");
}


bool CEngine::loadLandscape() {
    loginfo("Lade Datei |Landscapes.cfg|","CEngine::loadLandscapes");

     // Liste mit Objekten abarbeiten :

    if (object3DLandscapeList.empty() )
        return false;

    logwarn("Anzahl der Landscapes: " + IntToString(object3DLandscapeList.size()));

    for (unsigned int i = 0; i < object3DLandscapeList.size(); i++) {

         std::string path = OBJECT3D_CFG + "Landscapes/" + object3DLandscapeList[i];

         loginfo("Erstelle Object: .......< " + path+ " >","Engine::loadLandscapes");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);

         if( ! objconfig.empty() ) {

             loginfo("objconfig size : " + IntToString(objconfig.size()),"Engine::loadLandScape");

             sLandscape landscape;
             if (initLandscape(landscape,objconfig)) {

                 LandScape * obj = new LandScape();
                //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                /*
                if ( landscape.textures == "NONE" ) {
                     obj->SetHasTextures( false);
                }
                else {
                    if ( landscape.textures == "YES" )
                         obj->SetHasTextures( true);
                 }
                */
                 obj->SetColor(glm::vec4(landscape.color.x, landscape.color.y, landscape.color.z, landscape.color.w));
                 obj->Rotate(glm::vec3(landscape.trans.rotate.x, landscape.trans.rotate.y, landscape.trans.rotate.z) );
                 obj->Translate(glm::vec3(landscape.trans.translate.x, landscape.trans.translate.y, landscape.trans.translate.z));
                 obj->Scale(glm::vec3(landscape.trans.scale.x, landscape.trans.scale.y, landscape.trans.scale.z));

                 obj->setPatchX(landscape.patchX);
                 obj->setPatchZ(landscape.patchZ);
                 obj->setRasterX(landscape.rasterX);
                 obj->setRasterZ(landscape.rasterZ);

                 //----------------------------------------
                 // Add textures , if we have some
                 // ---------------------------------------
                 bool texturesok;
                 std::vector<std::string> images;
                 std::string path = landscape.textures;
                 if ( landscape.textures != "NONE" ) {
                     fileUtil fu;

                     texturesok =  fu.readLine(path, images);

                     if (texturesok)
                         obj->addTexture(images,"Engine::loadLandscape");
                     else
                         logwarn("Engine::loadScape :  konnte Textures nicht laden ! ","Engine::loadLandscape");
                 }
                 else {
                     int count = 0;

                     if (landscape.texture0 != "" ) {
                         images.push_back(landscape.texture0);
                         count ++;
                     }

                     if (landscape.texture1 != "" ) {
                         images.push_back(landscape.texture1);
                         count ++;
                     }

                     if (landscape.texture2 != "" ) {
                         images.push_back(landscape.texture2);
                         count ++;
                     }

                     if (landscape.texture3 != "" ) {
                         images.push_back(landscape.texture3);
                         count ++;
                     }

                     if (landscape.texture4 != "" ) {
                         images.push_back(landscape.texture4);
                         count ++;
                     }

                     if (count > 0) {
                        obj->addTexture(images,"InitGL::addLandscape");
                        loginfo("added " + IntToString(count) + " Textures ", "Engine::loadLandscapes");
                     }
                     else
                         obj->SetHasTextures(false);
                 }

                 if (obj->init() ) {
                     loginfo("Landscape initialisisert ","CEngine::loadLandscape");
                     add2List(obj,LIGHT_SHADER); //)TEXTURE_SHADER
                 }
                 else
                    logwarn("Landscape wurde nicht initialisisert ","CEngine::loadLandscape");
             }
             else
                 logwarn("konnte Structure Landscape nicht initialisieren !!", "CEngine::loadLandscpae" );
                // Hier die neuen stringpart functions einbauen

             loginfo("Prepare for next Object: ","CEngine::init3D");
             logEmptyLine();
        }
    }
    return true;
}

bool CEngine::loadTexturedCubes(){

    loginfo("Lade Datei |TexCubes.cfg|","CEngine::loadTextureCubes");

     // Liste mit Objekten abarbeiten :

    if (object3DTexturedList.empty() )
        return false;

    for (unsigned int i = 0; i < object3DTexturedList.size(); i++) {

         std::string path = OBJECT3D_CFG + "TexturedCubes/" + object3DTexturedList[i];

         loginfo("Erstelle Textured Cubes: .......< " + path+ " >","Engine::loadTexturedCubes");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);


         if( ! objconfig.empty() ) {

             s3DStruct s3D;

             if (init3DStruct(s3D,objconfig)) {

                 CCube * obj = new CCube();
                 //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 if ( s3D.textures == "" )
                     obj->SetHasTextures( false);
                 else
                     obj->SetHasTextures( true);

                 obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetFirstTranslate( ( s3D.firstTranslate == 1) ? true: false);
                 obj->Rotate(glm::vec3(s3D.trans.rotate.x, s3D.trans.rotate.y, s3D.trans.rotate.z) );
                 obj->Translate(glm::vec3(s3D.trans.translate.x, s3D.trans.translate.y, s3D.trans.translate.z));
                 obj->Scale(glm::vec3(s3D.trans.scale.x, s3D.trans.scale.y, s3D.trans.scale.z));
                 obj->SetHasAlpha(true);
                 obj->SetFrontFace(GL_CCW);

                 //----------------------------------------
                 // Add textures , if we have some
                 // ---------------------------------------
                 bool texturesok;
                 std::vector<std::string> images;

                 std::string path = PATH::ROOT + s3D.textures;
                 if ( s3D.textures != "none" ) {
                     fileUtil fu;

                     texturesok =  fu.readLine(path, images);
                     if (texturesok)
                         obj->addTexture(images,"InitGL::add3DObject");
                     else
                         logwarn("Engine::loadTexturedCube: Konnte Textures nicht laden ! ","CEngine::loadTexturedCube");
                 }
                 else {

                     int count = 0;

                     if (s3D.texture0 != "" ) {
                         images.push_back(PATH::ROOT + s3D.texture0);
                         count ++;
                     }

                     if (s3D.texture1 != "" ) {
                         images.push_back(PATH::ROOT + s3D.texture1);
                         count ++;
                     }

                     if (s3D.texture2 != "" ) {
                         images.push_back(PATH::ROOT + s3D.texture2);
                         count ++;
                     }

                     if (s3D.texture3 != "" ) {
                         images.push_back(PATH::ROOT + s3D.texture3);
                         count ++;
                     }

                     if (s3D.texture4 != "" ) {
                         images.push_back(PATH::ROOT + s3D.texture4);
                         count ++;
                     }

                     if (count > 0) {
                        obj->addTexture(images,"InitGL::add3DObject");
                        loginfo("added " + IntToString(count) + " Textures ", "Engine::loadTexturedCubes");
                     }
                     else
                         obj->SetHasTextures(false);                 }

                 loginfo("s3D initialisisert ","CEngine::init3D");
                 add2List(obj,LIGHT_SHADER); //LIGHT_SHADER)

             }
             else
                 logwarn("konnte s3D nicht initialisieren !!", "CEngine::init3D" );

             loginfo("Prepare for next Object: ","CEngine::init3D");
             logEmptyLine();
        }
    }

    for (int i=0; i< 1; i++)
        setupTemplate(i);
    return true;
}

void CEngine::setupTemplate(int i) {

    CCube * obj = new CCube();
    glm::vec3 translate = glm::vec3(1+ static_cast<float>(i), 1 +static_cast<float>(i),0 ) ;
    glm::vec3 rotate = glm::vec3(1.0,1.0,1.0);
    glm::vec3 scale = glm::vec3(1.0,1.0,1.0);
    glm::vec4 color = glm::vec4(0,0,1,1);

    std::string tex0 = PATH::ROOT + "images/bluefish.png";
    std::string tex1 = PATH::ROOT + "images/wall.png";


    obj->SetHasTextures( true);

    obj->SetColor(color);
    obj->SetFirstTranslate( ((i % 2) == 0 ? true:false));
    obj->Rotate(rotate );
    obj->Translate(translate);
    obj->Scale(scale);
    obj->SetHasAlpha(true);
    obj->SetFrontFace(GL_CCW);

    //----------------------------------------
    // Add textures , if we have some
    // ---------------------------------------

    std::vector<std::string> images;
    images.push_back(tex0);
    images.push_back(tex1);
    obj->addTexture(images,"InitGL::add3DObject");
    add2List(obj,LIGHT_SHADER); //LIGHT_SHADER)
}

bool CEngine::loadCockpits() {
    loginfo("Lade Datei |Cockpits.cfg | ","CEngine::loadCockpits");

     // Liste mit Cockpits abarbeiten :

    if (objectCockpitList.empty() )
        return false;

    for (unsigned int i = 0; i < objectCockpitList.size(); i++) {

         std::string path = COCKPIT_CFG + objectCockpitList[i];

         loginfo("Erstelle Object: .......< " + path+ " >","Engine::loadCockpits");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);


         if( ! objconfig.empty() ) {

             s3DStruct s3D;

             if (init3DStruct(s3D,objconfig)) {

                 CCube * obj = new CCube();
                 if ( s3D.textures == "" )
                     obj->SetHasTextures( false);
                 else
                     obj->SetHasTextures( true);

                 obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetFirstTranslate( ( s3D.firstTranslate == 1) ? true: false);
                 obj->Rotate(glm::vec3(s3D.trans.rotate.x, s3D.trans.rotate.y, s3D.trans.rotate.z) );
                 obj->Translate(glm::vec3(s3D.trans.translate.x, s3D.trans.translate.y, s3D.trans.translate.z));
                 obj->Scale(glm::vec3(s3D.trans.scale.x, s3D.trans.scale.y, s3D.trans.scale.z));
                 obj->SetHasAlpha(true);
                 obj->SetFrontFace(GL_CW);

                 //----------------------------------------
                 // Add textures , if we have some
                 // ---------------------------------------
                 bool texturesok;
                 std::vector<std::string> images;

                 std::string path =PATH::ROOT + s3D.textures;
                 if ( s3D.textures != "" ) {
                     fileUtil fu;

                     texturesok =  fu.readLine(path, images);
                     if (texturesok)
                         obj->addTexture(images,"InitGL::addCockpitTexture");
                     else
                         logwarn("Engine::loadCockpits: Konnte Textures nicht laden ! ","CEngine::loadCockpits");
                 }
                 loginfo("Cockpit initialisisert ","CEngine::loadCockpit");

                 obj->initShader(COLOR_SHADER,cubeshaderprog_color);
                 obj->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
                 obj->initShader(LIGHT_SHADER, cubeshaderprog_normals);
                 obj->initShader(LIGHT_COLOR_SHADER, cubeshaderprog_color_normal);
                 obj->initShader(GLASS_SHADER,glasshader);
                 obj->setActiveShader(TEXTURE_SHADER);
                 obj->SetFrontFace(GL_CW);

                 //cockpit->setMesh(obj);
                 //add2List(obj,LIGHT_SHADER); //LIGHT_SHADER)

             }
             else
                 logwarn("konnte Cockpit nicht initialisieren !!", "CEngine::loadCockpits" );
                // Hier die neuen stringpart functions einbauen

             loginfo("Prepare for next cockpit: ","CEngine::loadcockpit");
             logEmptyLine();
        }
    }
    return true;
}

bool CEngine::loadColorCubes() {

    loginfo("Lade Datei |ColorCubes.cfg|","CEngine::loadColorCubes");

     // Liste mit Objekten abarbeiten :

    if (object3DColoredList.empty() )
        return false;

    for (unsigned int i = 0; i < object3DColoredList.size(); i++) {

         std::string path = OBJECT3D_CFG + "ColoredCubes/" + object3DColoredList[i];

         loginfo("Erstelle Colored Cubes: .......< " + path+ " >","Engine::LoadColoredCubes");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);


         if( ! objconfig.empty() ) {

             s3DStruct s3D;

             if (init3DStruct(s3D,objconfig)) {

                 CColorCube * obj = new CColorCube();
                 //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetHasTextures( false);
                 obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetFirstTranslate( ( s3D.firstTranslate == 1) ? true: false);
                 obj->Rotate(glm::vec3(s3D.trans.rotate.x, s3D.trans.rotate.y, s3D.trans.rotate.z) );
                 obj->Translate(glm::vec3(s3D.trans.translate.x, s3D.trans.translate.y, s3D.trans.translate.z));
                 obj->Scale(glm::vec3(s3D.trans.scale.x, s3D.trans.scale.y, s3D.trans.scale.z));
                 obj->SetFrontFace(GL_CCW);

                 loginfo("s3D initialisisert ","CEngine::init3D");

                 add2List(obj,LIGHT_COLOR_SHADER);
             }
             else
                 logwarn("konnte s3D nicht initialisieren !!", "CEngine::init3D" );
                // Hier die neuen stringpart functions einbauen

             loginfo("Prepare for next Object: ","CEngine::init3D");
             logEmptyLine();
        }
    }
    return true;
}

bool CEngine::loadSpheres(){
    loginfo("Lade Datei | TexturedSpheres.cfg|","CEngine::loadTexturedSpheres");

    // Liste mit Objekten abarbeiten :
    if (objectTexturedSpheresList.empty() )
        return false;

    for (unsigned int i = 0; i < objectTexturedSpheresList.size(); i++) {

         std::string path = SPHERE_CFG + objectTexturedSpheresList[i];

         loginfo("Erstelle Textured Spheres: .......< " + path+ " >","Engine::LoadTexturedSpheres");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);


         if( ! objconfig.empty() ) {

             sSphereStruct sphere;

             if (initSphereStruct(sphere,objconfig)) {

                 CSphere * obj = new CSphere();
                 //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 bool texturesok;
                 std::vector<std::string> images;

                 std::string path = PATH::ROOT + sphere.textures;
                 if ( sphere.textures != "none" ) {
                     fileUtil fu;

                     texturesok =  fu.readLine(path, images);
                     if (texturesok)
                         obj->addTexture(images,"InitGL::add3DObject");
                     else
                         logwarn("Engine::loadTexturedCube: Konnte Textures nicht laden ! ","CEngine::loadTexturedCube");
                 }
                 else {

                     int count = 0;

                     if (sphere.texture0 != "" ) {
                         images.push_back(PATH::ROOT + sphere.texture0);
                         count ++;
                     }

                     if (sphere.texture1 != "" ) {
                         images.push_back(PATH::ROOT + sphere.texture1);
                         count ++;
                     }

                     if (sphere.texture2 != "" ) {
                         images.push_back(PATH::ROOT + sphere.texture2);
                         count ++;
                     }

                     if (sphere.texture3 != "" ) {
                         images.push_back(PATH::ROOT + sphere.texture3);
                         count ++;
                     }

                     if (sphere.texture4 != "" ) {
                         images.push_back(PATH::ROOT + sphere.texture4);
                         count ++;
                     }

                     if (count > 0) {
                        obj->addTexture(images,"InitGL::add3DObject");
                        loginfo("added " + IntToString(count) + " Textures ", "Engine::loadTexturedSphere");
                     }
                     else
                         obj->SetHasTextures(false);
                  }

                 obj->SetColor(glm::vec4(sphere.color.x, sphere.color.y, sphere.color.z, sphere.color.w));
                 obj->SetFirstTranslate( ( sphere.firstTranslate == 1) ? true: false);
                 obj->Rotate(glm::vec3(sphere.trans.rotate.x, sphere.trans.rotate.y, sphere.trans.rotate.z) );
                 obj->Translate(glm::vec3(sphere.trans.translate.x, sphere.trans.translate.y, sphere.trans.translate.z));
                 obj->Scale(glm::vec3(sphere.trans.scale.x, sphere.trans.scale.y, sphere.trans.scale.z));
                 obj->setRadius(sphere.radius);
                 obj->setCountMeshPoints(sphere.meshpoints);

                 obj->setUp();
                 obj->SetFrontFace(GL_CCW);
                 loginfo("Sphere initialisisert ","CEngine::initSphere");

                 add2List(obj,LIGHT_COLOR_SHADER);
             }
             else
                 logwarn("konnte Sphere nicht initialisieren !!", "CEngine::iniSpheres" );
                // Hier die neuen stringpart functions einbauen

             loginfo("Prepare for next Object: ","CEngine::initSpheres");
             logEmptyLine();
        }
    }
    return true;
}

void CEngine::InitButtons() {

    logEmptyLine();
    loginfo("============================");
    loginfo("Erstelle Buttons ...........");
    loginfo("============================");

    loadButtons();
}

void CEngine::loadButtons() {
    // Open file
    bool ok;
    fu = new fileUtil();

    ok = fu->readLine(BUTTONS_CFG + "ButtonList.cfg", btnList);

    if (ok) {
        loginfo("Lade Datei |ButtonList.cfg|","CEngine::loadbuttons");
        logEmptyLine();

        for (unsigned int i = 0; i < btnList.size(); i++  ){

            std::string path = BUTTONS_CFG + btnList[i];

            fileUtil * btnreader = new fileUtil;
            std::vector<std::string> btnconfig;
            btnreader->readLine(path, btnconfig);

            if ( ! btnconfig.empty() ) {

                loginfo("Erstelle Button: .......< " + path+ " >", "Engine::loadButtons");

                sButtonStruct btnStruct;
                if  (initButtonStruct(btnStruct,btnconfig) ) {
                    sPoint pos;
                    CButton * btn = new CTextButton(_ResX,_ResY ,btnStruct.path,"",pos,getShaderPtr());//   btnStruct.path);
                    btn->setPos(btnStruct.PosX,btnStruct.PosY);
                    btn->setSize(btnStruct.SizeX, btnStruct.SizeY);
                    btn->setbuttonColors(glm::vec3(btnStruct.ImageRed,btnStruct.ImageGreen,btnStruct.ImageBlue),
                                         glm::vec3(btnStruct.TextRed,btnStruct.TextGreen,btnStruct.TextBlue));

                    if (btnStruct.Enable > 0 )
                        btn->enable();
                    else
                        btn->disable();
                    btn->setText("Edit");
                    addButton(btn);

                    loginfo(".........Done", "Engine::loadButtons");
                    logEmptyLine();
                }
            }
        }
    }
    else
        logwarn("Fehler: Datei | config/ButtonList nicht gefunden ! |");

    logEmptyLine() ;
    loginfo("----------------------------");
    loginfo(" Load Button Objects ");
    loginfo("        READY");
    loginfo("----------------------------");

}

std::string &CEngine::getValueItem(std::string &s, std::string erasestring) {
    return s.erase(0,erasestring.length() ) ;
}

bool CEngine::initLandscape(sLandscape &ls, std::vector<std::string> &cfg){
    if (cfg.size() >= CFG_3D_SIZE ) {

        //+---------------------------------------------------------------------+
        //+     VORGEHEN :                                                      |
        //+     Liste abarbeiten, Teilstring bis " " ermitteln,                 |
        //+     Variablen name = Teilstring --> 2. Teilstring in Wert wandeln   |
        //+     und in der s3DStruct zuweisen                                   |
        //+---------------------------------------------------------------------+

        for (uint i = 0; i < cfg.size(); i++) {
            std::vector<std::string> parts = split(cfg.at(i), SPACE);

            if (parts.at(0) == "colorRed")
                ls.color.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorGreen")
                ls.color.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorBlue")
                ls.color.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorAlpha")
                ls.color.w = StringToFloat(parts.at(1));

            if (parts.at(0) == "hasLight")
                ls.hasLight = StringToInt(parts.at(1));

            if (parts.at(0) == "textures")
                ls.textures = parts.at(1);

            if (parts.at(0) == "translateX")
                ls.trans.translate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateY")
                ls.trans.translate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateZ")
                ls.trans.translate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateX")
                ls.trans.rotate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateY")
                ls.trans.rotate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateZ")
                ls.trans.rotate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleX")
                ls.trans.scale.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleY")
                ls.trans.scale.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleZ")
                ls.trans.scale.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "patchX")
                ls.patchX = StringToInt(parts.at(1));

            if (parts.at(0) == "patchZ")
                ls.patchZ = StringToInt(parts.at(1));

            if (parts.at(0) == "rasterX")
                ls.rasterX = StringToFloat(parts.at(1));

            if (parts.at(0) == "rasterZ")
                ls.rasterZ = StringToFloat(parts.at(1));

            if (parts.at(0) == "texture0")
                ls.texture0 = parts.at(1);

            if (parts.at(0) == "texture1")
                ls.texture1 = parts.at(1);

            if (parts.at(0) == "texture2")
                ls.texture2 = parts.at(1);

            if (parts.at(0) == "texture3")
                ls.texture3 = parts.at(1);

            if (parts.at(0) == "texture4")
                ls.texture4 = parts.at(1);

        }
        return true;

    }
    return false;
}

bool CEngine::init3DStruct(s3DStruct &d3s, std::vector<std::string> &cfg){
    if (cfg.size() >= CFG_3D_SIZE ) {

        //+---------------------------------------------------------------------+
        //+     VORGEHEN :                                                      |
        //+     Liste abarbeiten, Teilstring bis " " ermitteln,                 |
        //+     Variablen name = Teilstring --> 2. Teilstring in Wert wandeln   |
        //+     und in der s3DStruct zuweisen                                   |
        //+---------------------------------------------------------------------+

        for (uint i = 0; i < cfg.size(); i++) {
            std::vector<std::string> parts = split(cfg.at(i), SPACE);

            if (parts.at(0) == "originX")
                d3s.origin.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "originY")
                d3s.origin.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "originZ")
                d3s.origin.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorRed")
                d3s.color.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorGreen")
                d3s.color.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorBlue")
                d3s.color.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorAlpha")
                d3s.color.w = StringToFloat(parts.at(1));

            if (parts.at(0) == "hasLight")
                d3s.hasLight = StringToInt(parts.at(1));

            if (parts.at(0) == "textures")
                d3s.textures = parts.at(1);

            if (parts.at(0) == "translateX")
                d3s.trans.translate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateY")
                d3s.trans.translate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateZ")
                d3s.trans.translate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateX")
                d3s.trans.rotate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateY")
                d3s.trans.rotate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateZ")
                d3s.trans.rotate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleX")
                d3s.trans.scale.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleY")
                d3s.trans.scale.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleZ")
                d3s.trans.scale.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "firstTranslate")
                d3s.firstTranslate = StringToInt(parts.at(1));

            if (parts.at(0) == "texture0")
                d3s.texture0 = parts.at(1);

            if (parts.at(0) == "texture1")
                d3s.texture1 = parts.at(1);

            if (parts.at(0) == "texture2")
                d3s.texture2 = parts.at(1);

            if (parts.at(0) == "texture3")
                d3s.texture3 = parts.at(1);

            if (parts.at(0) == "texture4")
                d3s.texture4 = parts.at(1);

        }
        return true;

    }
    return false;
}

bool CEngine::initSphereStruct(sSphereStruct &sSphere, std::vector<std::string> &cfg){
    if (cfg.size() <= CFG_SPHERE_SIZE ) {

        //+---------------------------------------------------------------------+
        //+     VORGEHEN :                                                      |
        //+     Liste abarbeiten, Teilstring bis " " ermitteln,                 |
        //+     Variablen name = Teilstring --> 2. Teilstring in Wert wandeln   |
        //+     und in der s3DStruct zuweisen                                   |
        //+---------------------------------------------------------------------+

        for (uint i = 0; i < cfg.size(); i++) {
            std::vector<std::string> parts = split(cfg.at(i), SPACE);

            if (parts.at(0) == "originX")
                sSphere.origin.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "originY")
                sSphere.origin.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "originZ")
                sSphere.origin.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorRed")
                sSphere.color.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorGreen")
                sSphere.color.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorBlue")
                sSphere.color.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorAlpha")
                sSphere.color.w = StringToFloat(parts.at(1));

            if (parts.at(0) == "hasLight")
                sSphere.hasLight = StringToInt(parts.at(1));

            if (parts.at(0) == "textures")
                sSphere.textures = parts.at(1);

            if (parts.at(0) == "translateX")
                sSphere.trans.translate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateY")
                sSphere.trans.translate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateZ")
                sSphere.trans.translate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateX")
                sSphere.trans.rotate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateY")
                sSphere.trans.rotate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateZ")
                sSphere.trans.rotate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleX")
                sSphere.trans.scale.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleY")
                sSphere.trans.scale.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleZ")
                sSphere.trans.scale.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "firstTranslate")
                sSphere.firstTranslate = StringToInt(parts.at(1));

            if (parts.at(0) == "texture0")
                sSphere.texture0 = parts.at(1);

            if (parts.at(0) == "texture1")
                sSphere.texture1 = parts.at(1);

            if (parts.at(0) == "texture2")
                sSphere.texture2 = parts.at(1);

            if (parts.at(0) == "texture3")
                sSphere.texture3 = parts.at(1);

            if (parts.at(0) == "texture4")
                sSphere.texture4 = parts.at(1);

            if (parts.at(0) == "radius")
                sSphere.radius = StringToFloat(parts.at(1));

            if (parts.at(0) == "meshpoints")
                sSphere.meshpoints = StringToFloat(parts.at(1));
        }
        return true;

    }
    return false;
}

bool CEngine::initButtonStruct(sButtonStruct &bs, std::vector<std::string> cfg) {

    if (cfg.size() >= CFG_BUTTON_SIZE ) {
        bs.path = getValueItem(cfg[0],"Image: ");
        loginfo("Image Path: "+ bs.path);

        bs.PosX = StringToFloat(getValueItem(cfg[1],"PositionX: "));

        bs.PosY = StringToFloat(getValueItem(cfg[2],"PositionY: "));

        bs.SizeX =  StringToFloat(getValueItem(cfg[3],"SizeX: "));

        bs.SizeY =  StringToFloat(getValueItem(cfg[4],"SizeY: "));

        bs.Enable = StringToInt(getValueItem(cfg[5],"Enable: "));

        bs.ImageRed = StringToFloat(getValueItem(cfg[6],"BGRed: "));

        bs.ImageGreen = StringToFloat(getValueItem(cfg[7],"BGGreen: "));

        bs.ImageBlue = StringToFloat(getValueItem(cfg[8],"BGBlue: "));

        bs.TextRed = StringToFloat(getValueItem(cfg[9],"TextColorRed: "));

        bs.TextGreen = StringToFloat(getValueItem(cfg[10],"TextColorGreen: "));

        bs.TextBlue = StringToFloat(getValueItem(cfg[11],"TextColorBlue: "));

        return true;
    }
    else
        return false;
}





