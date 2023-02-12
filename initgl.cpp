#include "initgl.h"
#include "Cad/cad2d.h"
#include "defaults.h"


/*
 * File:   Init3D.cpp
 * Author: paul
 *
 * Created on 17. Mai 2018, 21:11
 */

//#include "vecmath/vecmath.h"
#include <vector>
//#include "3DLoader/load3ds.h"

// Res for windowed Mode


const std::string PATH_HEADLINE     = PATH::ROOT + "images/darkgray.png";
const std::string PATH_TEXTFIELD    = PATH::ROOT + "images/Textfeld.png";
const std::string PATH_BOTTOM       = PATH::ROOT + "images/Bottom.png";


bool _ShowSkybox;
bool _Animate;
bool _UseBlend;
bool _ShowCockpit;
bool _ShowPanel;   // DrawPanel für 2D

using namespace irrklang;

InitGL::InitGL (const std::string titel){

    caption = titel;
    window = nullptr;
    maincontext = nullptr;

    _Frames = nullptr;
    _FramerateOut = 0;

    _Mouse.lastx = 0;
    _Mouse.lasty = 0;
    _Mouse.x     = 0;
    _Mouse.y     = 0;

    sphere1 = nullptr;
    lightSource = nullptr;
    skybox  = nullptr;
    base2d  = nullptr;
   // land = nullptr;

    cockpit = nullptr;
    PE      = nullptr;

    MousePositions = nullptr;
   // soundengine = NULL;
    projection = nullptr;

    MainMenu = nullptr;
    showMenu = false;
    _ShaderChanged = false;
    _CameraSpeed = 1.0f;

    //LoadConfiguration();
    InitUtils();
    InitMatrices();

    InitFX();

  // _HasSound = initSoundMachine();
    // Irrklan init
   // soundengine = createIrrKlangDevice();
   // if ( soundengine )
   //     loginfo(" SoundEngine created ","InitGL::InitGL");
}

InitGL::InitGL(const InitGL& orig) {
}

InitGL::~InitGL() {

    // Alten Videomode wiederherstellen
    SDL_SetWindowDisplayMode(window,&DesktopDisplayMode);

    //if (soundengine)
    //    soundengine->drop();

    safeDelete(sphere1);
    safeDelete(lightSource);
    //delete load3DS;
    //safeDelete(land);

    if ( skybox != nullptr)
        delete skybox;

    if (base2d != nullptr  )
        delete base2d;

    if (cockpit != nullptr)
        delete cockpit;

    if (ambientLight != nullptr)
        delete  ambientLight;

    DeleteUtils();
    DeleteMatrices();
    DeleteShaders();
}

void  InitGL::setRootPath(std::string path) {

    PATH::ROOT = path;
}

void InitGL::DeleteShaders() {
    glDeleteProgram(cubeshaderprog_color);
    glDeleteProgram(cubeshaderprog_tex);
    glDeleteProgram(sphereshader_color);
    glDeleteProgram(cubeshaderprog_color_normal);
}

Shader * InitGL::getShaderPtr() {
    return shader;
}

void InitGL::safeDelete(BaseObject * bo) {
    if (bo != nullptr) {
        delete bo;
        bo = nullptr;
    }
}

//------------------------------------------
//  Virtuals Render
// -----------------------------------------
void InitGL::Render() {

    if (! list3D.empty() ) {
        for (unsigned int i=0;i < list3D.size(); i++ ) {

            glm::vec3 vt(0.001,0.002,0.003);

            if (_Animate && list3D[i]->HasAnimation()) {

                list3D[i]->StepTranslate(vt,_Elapsed);
                list3D[i]->AnimateRotate(_Elapsed);
            }

            if (_UseBlend)
                list3D[i]->SetUseBlending(true);
            else
                list3D[i]->SetUseBlending(false);

            list3D[i]->UseBlinn(_UseBlinn);
            list3D[i]->Draw(camera);
      }
   }  // Not showpanel
}

void InitGL::RenderSkyBox() {
    if ( skybox != nullptr && _ShowSkybox )
        skybox->Draw(camera->GetView());
}

void InitGL::RenderLight() {
    if (lightSource != nullptr) {

        if (_UseBlend)
            lightSource->SetUseBlending(true);
           // glDisable(GL_DEPTH_TEST);
        else
            //glEnable(GL_DEPTH_TEST);
            lightSource->SetUseBlending(false);

        lightSource->SetColor(glm::vec4(0.5,0.5,0.5,0.3));
        lightSource->SetProjection(projection->GetPerspective());
        lightSource->SetFirstTranslate(false);
        if (_Animate && lightSource->HasAnimation() )
            lightSource ->AnimateRotate(_Elapsed);

        lightSource->Draw(camera);
    }
}

void InitGL::RenderCockpit() {

    if (cockpit != nullptr) {

        if (cockpit->HasMesh()  && _ShowCockpit) {

            cockpit->getCockpitMesch()->SetUseBlending(true);
            cockpit->getCockpitMesch()->setGlasShader(true);
            cockpit->setProjectionMatrix(projection->GetPerspective());
            cockpit->Translate(camera->GetPos());
            cockpit->Rotate(camera->MoveDirectionDEG());
            cockpit->Draw(camera);
            cockpit->getCockpitMesch()->setGlasShader(true);
        }
    }
}

void InitGL::Render2D(){

    if ( !  objects2D.empty() ) {
        for (uint i =0; i < objects2D.size(); i++) {
            (objects2D[i])->Render();
        }
    }
}

void InitGL::RenderControlls() {

    //------------------------------------
    // MainMenu rendern
    // -----------------------------------

    if ( MainMenu != nullptr  && showMenu) {
        MainMenu ->Render();
    }
}



bool InitGL::hasSkyBox() { return _HasSkyBox; }
void InitGL::setHasSkybox(bool enable) { _HasSkyBox = enable; }

void InitGL::LoadConfiguration(){

/*
    _FullScreen = false;
    _ResX = SD_WIDTH;
    _ResY = SD_HEIGHT;

    fileUtil * fu = new fileUtil();
    std::vector<std::string> enginecfg;
    bool ok = fu->readLine(MAIN_CFG,enginecfg);

    if (ok) {
        for (uint i = 0; i< enginecfg.size(); i++) {

            std::vector<std::string> parts = split(enginecfg.at(i), SPACE);
            if ( ! parts.empty()) {

                if ( parts.at(0) == "FullScreen") {
                    if (parts.size() > 1) {  // wenigstens 2 einträge:
                        _FullScreen = (StringToInt(parts.at(1)) == 1 ? true : false);
                        if (_FullScreen)
                            loginfo("Schalte auf Fullscreen.......Done");
                        else
                            loginfo("Schalte auf Window-Mode.......Done");
                    }
                }
                else if ( parts.at(0) == "Resolution") {
                    if (parts.size() > 2) {  // wenigstens 3 einträge:
                        _ResX = StringToInt(parts.at(1));
                        _ResY = StringToInt(parts.at(2));
                        loginfo("Setting Resolution " +parts.at(1) + "x" + parts.at(2) + ".......Done");
                    }
                    else {
                        _ResX = SD_WIDTH;
                        _ResY = SD_HEIGHT;
                        logwarn("Warnung: konnte Resolution nich lesen -> verwende Default 1920x1200");
                    }
                }
            }
        }
    }
    else
        logwarn("Konnte keine .cfg Datei finden ","CEngine::LoadConfiguration");
        */

}

std::vector<std::string> InitGL::split(std::string const& input, std::string const& separator = " ")
{
  std::vector<std::string> result;
  std::string::size_type position, start = 0;

  while (std::string::npos != (position = input.find(separator, start)))
  {
    result.push_back(input.substr(start, position-start));
    start = position + separator.size();
  }

  result.push_back(input.substr(start));
  return result;
}


//------------------------------------------
// effects
//------------------------------------------
void InitGL::InitFX(){

}

// fog
void InitGL::fogParam(){
    float col[4] = {0.5f,0.5f,0.5f,1.0f};
    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR ,col);
    glFogf(GL_FOG_DENSITY, 0.75f);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
    glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
    glEnable(GL_FOG);

}
void InitGL::setFog(bool enable) {
        //Shader use forfogging
}

// ******************************************
// Utils
// ------------------------------------------
void InitGL::InitUtils() {
    filestream = new fileUtil();
    if (filestream == nullptr) {
        logwarn ("Konnte Util FileStream nicht erstellen !!","InitGL::InitUtils");
    }
    else
        loginfo("Erstelle FileUtil...... Done","InitGL::InitUtils");
}

void InitGL::InitShaders() {
    //================================================================
    //Test für CustomShader

    shader = new Shader();
    _CurrentShader = ShaderType::LIGHT_SHADER;
    // ========================================================================
}

void InitGL::DeleteUtils() {
    if ( filestream != nullptr) {
        delete filestream;
        loginfo("Deleted filestream","InitGL::DeleteUtils");
    }
}

void InitGL::DeleteMatrices() {
    if (projection != nullptr) {
        delete projection;
        loginfo("Delete Prokjection Matrix Class","InitGL::DeleteMatrtices");
    }
}

// ******************************************
// MAtrices
// ------------------------------------------
void InitGL::InitMatrices() {
    projection = new Projection(0,_ResX,0,_ResY,0.01f,1000.0f);
    if (projection != nullptr) {
        loginfo("Created Procjection Class","InitGL::InitMatrices");
    }
}

void InitGL::SetResolution(int resx,int resy) {
    _ResX = resx;
    _ResY = resy;
}

// ******************************************
// Display-Modi anzeigen
// ------------------------------------------
void InitGL::PrintDisplayModes() {

    int numDisplaymodes = SDL_GetNumDisplayModes(0);
    loginfo("Num Display modes: " + IntToString(numDisplaymodes), "InitGL::Init");

    // Alle Display modes auflisten:

     for(int j = 0; j < SDL_GetNumVideoDisplays(); j++)
     {
        for (int i = 0; i < numDisplaymodes; i++ ) {
            SDL_DisplayMode dpm;
            SDL_GetDisplayMode(j,i,&dpm);
            std::string w = IntToString(dpm.w);
            std::string h = IntToString(dpm.h);
            std::string index =IntToString(i);
            try {

                // todo: Pixelformat auswerten
                loginfo("Display : " + IntToString(j) + "  Resolution Mode[" + index+ "] : " + w + "x" + h ,"InitGL::Init");
            }
            catch ( ...) {
                logwarn("Konnte mode[" + index + "] nicht ermitteln");
            }

        }
     }
}

// ******************************************
// Heier der graphische Teil
// ------------------------------------------
bool InitGL::InitSDL2()  {


    if (  SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
            logwarn("Konnte SDL nicht initialisieren ! " + (std::string) SDL_GetError());
            return(false);
    }
    atexit(SDL_Quit);

    printf("Init success \n");
    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    PrintDisplayModes();

     //------------------------------------------------------------
     // Get the current Displaymode for restore after ending
     //------------------------------------------------------------
     SDL_GetDesktopDisplayMode(0, &DesktopDisplayMode );


    //------------------------------------------------------------
    // _ResX und _ResY werden im Constructor von Engine gesetzt !!
    //------------------------------------------------------------
    if ( _FullScreen) {

        window = SDL_CreateWindow(
                caption.c_str(),
                0, //SDL_WINDOWPOS_UNDEFINED,
                0, //SDL_WINDOWPOS_UNDEFINED,
                _ResX,_ResY,
                //SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
                SDL_WINDOW_OPENGL
            );
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

    }
    else {

            window = SDL_CreateWindow(
                caption.c_str(),
                500,
                200,
                _ResX,_ResY,
                SDL_WINDOW_OPENGL
            );

    glViewport(0,0,_ResX, _ResY);
}


   if ( window == nullptr)  {

        sdl_die("Konnte Fenster nicht erzeugen");
        return false;
    }

    maincontext = SDL_GL_CreateContext(window);
    if ( ! maincontext) {
        sdl_die (" Konnte Context nicht erzeugen ");
        return false;
    }

    printf(" Geladene OpenGL version :");

    // Alles Ok ,checken was initialisiert ist :
    printf("Vendor :  %s\n",glGetString(GL_VENDOR));
    printf("Renderer  %s\n",glGetString(GL_RENDERER));
    printf("Version   %s\n",glGetString(GL_VERSION));


       GLenum err = glGetError();
       switch (err){
           case GL_NO_ERROR         :   loginfo (" Kein Fehler ","Base2D Render");  break;
           case GL_INVALID_ENUM     :   loginfo ("Invalid Enum","Base2D Render");   break;
           case GL_INVALID_VALUE    :   loginfo ("Invalid Value","Base2D REnder");  break;
           case GL_INVALID_OPERATION:   loginfo ("Invalid operation","Base2D Render"); break;
           case GL_INVALID_FRAMEBUFFER_OPERATION: loginfo ("Invalid Framebuffer Operation","Base2D Render"); break;
           case GL_OUT_OF_MEMORY    :   loginfo("Out of Memory","Base2D Render"); break;
           case GL_STACK_UNDERFLOW  :   loginfo ("Stack UNDERFLOW","Base2D Render"); break;
           case GL_STACK_OVERFLOW   :   loginfo ("Stack Overflow","Base2D Render"); break;
           default: loginfo("Undefined");
       }

    //loginfo("Erstelle Viewport " + IntToString(_ResX) + " x " + IntToString(_ResY),"InitGL::InitEngineObject");
    //glViewport(0,0, _ResX , _ResY );

    glewExperimental = GL_TRUE;
    glewInit();
    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    newDisplayMode.w = _ResX;
    newDisplayMode.h = _ResY;
    newDisplayMode.format = SDL_PIXELFORMAT_RGBX8888;


    //if (_FullScreen)
    SDL_SetWindowDisplayMode(window,&newDisplayMode);  // Only in fullscreen available

     // Testweise Displaymode ermitteln
    SDL_DisplayMode current;
    int error = SDL_GetCurrentDisplayMode(1, &current);
    if ( error == 0) {
       printf ("InitGL -- WindowSize:  %i x %i h \n",current.w,current.h);
    }
    else
        printf ("InitGL -- RES_X,RES_Y :  %i x %i h \n",_ResX,_ResY);

    MouseResX = (float)_ResX / (float)current.w;
    MouseResY = (float)_ResY / (float)current.h;

    InitEngineObject();

    InitUserObjects();

    return true;
}

void InitGL::setClearColor(float r, float g, float b) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = 1.0f;
    glClearColor(_ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);
}


void InitGL::SetClearColor(float r, float g, float b, float a) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = a;
    glClearColor(_ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);
}

void InitGL::InitEngineObject() {
// ===============================================================
// Hier werden alle Objeckte initialisiert
// ===============================================================

    loginfo("===========================");
    loginfo("Erstelle Shaders...........");
    loginfo("===========================");
    InitShaders();

    loginfo("Setze ClearColor auf Hell Blau ...... done","InitGL::InitEngineObject");
    setClearColor(0.57f,0.72f,0.92f);
    // ---------------------------------------
    // Camera View
    // ---------------------------------------
    loginfo("Erstelle Camera Objekt","InitGL::InitEngineObject");
    camera = new (Camera);
    loginfo("Erstelle Camera Objekt ...... done","InitGL::InitEngineObject");

    // ---------------------------------------
    // Light init
    //----------------------------------------
    ambientLight = new light;
    ambientLight->setColor(glm::vec3(1.0,1.0,1.0));
    ambientLight->setPos(glm::vec3(0.0,10.0,-5.0));

    loginfo("Erstelle Standard Ambientes Licht ","InitGL::InitEngineObjects");

    //-----------------------------------------
    // Lightsource as a spere
    //-----------------------------------------
    std::vector<std::string> images;
    fileUtil fu;
    // Texture loading
    bool texturesok;

    loginfo("Erstelle LichtQuelle als sphere....","InitGL::InitEngineObjects");
    lightSource = new CSphere(ambientLight->getPos(),glm::vec4(0.0,0.0,1.0,0.5),projection->GetPerspective(),18,(GLfloat)2.0,shader );
    lightSource->SetFrontFace(GL_CW);
    //Texture loading
    texturesok =  fu.readLine("config/cube2textures.cfg",images);
    if (texturesok)
        lightSource->addTexture(images,"Ad glob to lightsource");
    else
        logwarn("Init::Sphere1 konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
    images.clear();
    lightSource->initShader(COLOR_SHADER,shader->getColor3DShader());//cubeshaderprog_color);
    lightSource->initShader(TEXTURE_SHADER, shader->getTexture3DShader());//cubeshaderprog_tex);
    lightSource->initShader(LIGHT_SHADER,shader->getLightShader());  //cubeshaderprog_normals);
    lightSource->initShader(LIGHT_COLOR_SHADER, shader->getLightColorShader());  //cubeshaderprog_color_normal);
    lightSource->initShader(GLASS_SHADER, shader->getGlasShader() );//   glasshader);
    lightSource->setActiveShader(LIGHT_SHADER);
}

void InitGL::InitUserObjects() {

    loginfo("Starte InitUserObject","InitGL::InitUserObject");

    /*
    logimage("Erstelle Text Renderer.....");
    sPoint p;
    p.x =   100;
    p.y =   400;

    MousePositions = new TextRender(_ResX, _ResY, p,PATH_HEADLINE, PATH_TEXTFIELD,getShaderPtr());

    MousePositions->SetGlyphShader(shader->getGlyphShader());
    MousePositions->SetTextureShader(shader->getTexture2DShader());
    MousePositions->SetColorShader(shader->getColor2DShader());

    MousePositions->AddString("X: ");
    MousePositions->AddString("Y: ");

    MousePositions->SetHasBottom(false);
    MousePositions->SetHasHeader(true);
    MousePositions->SetHasBackground(true);
    MousePositions->SetHasTexture(false);
    MousePositions->SetAlignRight(false);

    textfields.push_back(MousePositions);
*/



}


// --------------------------------------------
// Adding's
// --------------------------------------------
void InitGL::add2List(BaseObject *obj, ShaderType s) {

    obj->initShader(COLOR_SHADER,shader->getColor3DShader()); //   cubeshaderprog_color);
    obj->initShader(TEXTURE_SHADER,shader->getTexture3DShader());//     cubeshaderprog_tex);
    obj->initShader(LIGHT_SHADER, shader->getLightShader()); //   cubeshaderprog_normals);
    obj->initShader(LIGHT_COLOR_SHADER, shader->getLightColorShader());  //cubeshaderprog_color_normal);
    obj->initShader(GLASS_SHADER,shader -> getGlasShader());
    obj->setActiveShader(s);
    obj -> SetProjection(projection->GetPerspective());

    obj->addLight(ambientLight);
    list3D.push_back(obj);
}

void InitGL::add2Dobject(Base2D *obj) {
    objects2D.push_back(obj);
}

void InitGL::addButton(CButton* obj) {
    buttons.push_back(obj);
}

void InitGL::toogleFullScreen (){
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
    SDL_SetWindowFullscreen(window,IsFullscreen ? 0 : FullscreenFlag);

    if (FullscreenFlag ) {

        //_ResX = FULLSCREEN_WIDTH;
        //_ResY = FULLSCREEN_HEIGHT;

        glViewport(0,0,FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);//FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);


    }
    else {
     //   _ResX = SD_WIDTH;
     //   _ResY = SD_HEIGHT;
        glViewport(0,0,_ResX, _ResY);
    }
}

void InitGL::ShowFramesPerSec(){}
void InitGL::ShowCameraPos() {}
void InitGL::toggleAnimation() { _Animate = toggleVal(_Animate); }
void InitGL::toogleCockpit() { _ShowCockpit = toggleVal(_ShowCockpit); }
void InitGL::toggleBlend() {_UseBlend = toggleVal(_UseBlend); }
void InitGL::togglePanel2D() {_ShowPanel = toggleVal(_ShowPanel);}

void InitGL::toggleSkyBox() { _ShowSkybox = toggleVal(_ShowSkybox);}
bool InitGL::toggleVal(bool val){return ! val; }


bool InitGL::initSoundMachine() {
  //  soundengine = irrklang::createIrrKlangDevice();
  //  if (soundengine) {
 //       irrklang::vec3df position(23,70,200);

        // start the sound paused:


//        if (_Sound)
//        {

          //irrklang::vec3df velPerSecond(0,0,0);    // only relevant for doppler effects
 //          _Sound ->setVolume( 100);
          //_Sound->setMinDistance(30.0f); // a loud sound
//           _Sound->setIsPaused(true); // unpause the sound
//           return true;
//        }
//        return false;
//    }
    return false;
}

bool InitGL::HandleMessage() {

    SDL_PollEvent(&_Event);

    switch(_Event.type) {

    //case SDL_KEYDOWN:
    case SDL_KEYUP : {
            switch(_Event.key.keysym.sym) {

                case SDLK_b: _UseBlinn = toggleVal(_UseBlinn);
                   break;

                case SDLK_ESCAPE : _QuitGame = true;
                    break;

                case SDLK_a : camera->MoveLeft(_Elapsed);
                   break;

                case SDLK_m :
                    showMenu = toggleVal(showMenu);
                    MainMenu->setActive(showMenu);
                    OnMainMenuStateChanged();
                    break;

                case SDLK_q: toggleAnimation();
                    break;

                case SDLK_d:
                    camera->MoveRight(_Elapsed);
                    //cockpit->setPosition(camera);
                    //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));

                    break;

                case SDLK_e:
                     camera->MoveForward(_Elapsed);
                     //cockpit->setPosition(camera);
                     //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));

                     break;


                case SDLK_s:
                    camera->MoveBackward(_Elapsed);
                    //cockpit->setPosition(camera);
                    //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));

                    break;

                case SDLK_c     : _CurrentShader = ShaderType::COLOR_SHADER;    _ShaderChanged = true;  break;
                case SDLK_t     : _CurrentShader = ShaderType::TEXTURE_SHADER;  _ShaderChanged = true;  break;

                case SDLK_F7    : _CurrentShader = ShaderType::GLASS_SHADER;    _ShaderChanged = true;  break;
                case SDLK_F8    : _CurrentShader = ShaderType::LIGHT_TEXTURE_SHADER;  _ShaderChanged = true; break;
                case SDLK_F9    : _CurrentShader = ShaderType::LIGHT_SHADER;    _ShaderChanged = true;  break;
                case SDLK_F10   : _CurrentShader = ShaderType::LIGHT_COLOR_SHADER;    _ShaderChanged = true;  break;
                case SDLK_F11   : toogleFullScreen(); break;


                // ORtho oder perspective mode:
                case SDLK_o :

                    for (uint i = 0; i < list3D.size(); i++) {
                        list3D[i]->SetProjection(projection->GetOrtho(), true);
                    }
                    break;

                case SDLK_p:
                    for (uint i = 0; i < list3D.size(); i++) {
                        list3D[i]->SetProjection(projection->GetPerspective(),false);
                    }
                    break;

                //------------------------------------------------------------------------------
                // Num pad
                //------------------------------------------------------------------------------
                case SDLK_KP_1:
                    camera->YawEyeLeft(_Elapsed);
                    break;

                // Roll Eye right
                case SDLK_KP_3:
                    camera-> YawEyeRight(_Elapsed);
                    break;

                case SDLK_KP_2:
                    // Pitch Eye Doun
                    camera -> PitchEyeDown(_Elapsed);
                    break;

                case SDLK_KP_5:
                    // Pitch eye Up
                    camera -> PitchEyeUp(_Elapsed);//
                    break;

                case SDLK_KP_PLUS:{
                    float s = camera->GetSpeed();
                    camera -> SetSpeed(++s);//
                    break;
                }

                case SDLK_KP_MINUS:{
                    float s = camera->GetSpeed();
                    s--;
                    (s < 0.0f) ? camera -> SetSpeed(0.0f) : camera->SetSpeed(s);//
                    break;
                }

                case SDLK_LEFT:
                     camera->YawCameraLeft(_Elapsed);

                     //cockpit->SetDir(camera->GetDir());
                     //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));
                     //cockpit->Rotate(glm::vec3(camera->PitchCameraDEG(),camera->YawCameraDEG(),camera->RollCameraDEG()));  //
                     //cockpit->setPosition(camera);
                     break;

                case SDLK_RIGHT:
                     camera->YawCameraRight(_Elapsed);

                     //cockpit->SetDir(camera->GetDir());
                     //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));
                     //cockpit->Rotate(glm::vec3(camera->PitchCameraDEG(),camera->YawCameraDEG(),camera->RollCameraDEG()));   //
                     //cockpit->setPosition(camera);
                     break;

                case SDLK_UP:
                     camera->PitchCameraUp(_Elapsed);

                     //cockpit->SetDir(camera->GetDir());
                     //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));
                     //cockpit->Rotate(glm::vec3(camera->PitchCameraDEG(),camera->YawCameraDEG(),camera->RollCameraDEG()));   //
                     //cockpit->setPosition(camera);
                     break;

                case SDLK_DOWN:
                    camera ->PitchCameraDown(_Elapsed);

                    //cockpit->SetDir(camera->GetDir());
                    //cockpit->Translate(glm::vec3(0.0,0.0,-15.0));
                    //cockpit->Rotate(glm::vec3(camera->PitchCameraDEG(),camera->YawCameraDEG(),camera->RollCameraDEG()));   //
                    //cockpit->setPosition(camera);
                    break;

            }  // case Keyup

            _Mouse.x = _Event.motion.x;
            _Mouse.y = _Event.motion.y;
            _MouseButtons = SDL_GetMouseState(&_Event.motion.x, &_Event.motion.y);
            OnMouseMove(_Event.motion.x, _Event.motion.y, _MouseButtons);
        }

        //------------------------------------------------------------------------------
        // Mause Events
        //------------------------------------------------------------------------------
        case      SDL_MOUSEMOTION : {

            _Mouse.x = _Event.motion.x;
            _Mouse.y = _Event.motion.y;

            _MouseButtons = SDL_GetMouseState(&_Event.motion.x, &_Event.motion.y);
            OnMouseMove(_Event.motion.x, _Event.motion.y, _MouseButtons);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {

            if ( _Event.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonDown(_Event.motion.x, _Event.motion.y);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {

            if ( _Event.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonUp(_Event.motion.x, _Event.motion.y);
            }
            break;
        }
    }
    return true;
}

void InitGL::Run() {

    bool quit = false;

    //--------------------------------------------------
    // framerate berechene
    //--------------------------------------------------
    int frames = 0;
    Uint64 ms = 0;
    showMenu = true;

    glEnable(GL_DEPTH_TEST);
    //--------------------------------------------------
    // Uncomment, if needed
    //--------------------------------------------------
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //      if (_HasSound)
    //          _Sound = soundengine->play2D("/home/paul/workspace/SpaceEngine/sounds/bell.wav");

    _Elapsed = 0;
    auto start = Clock::now();
    auto end = Clock::now();

    while ( ! _QuitGame) {

        HandleMessage();

        // Delete the eventque for movemove
      //  SDL_FlushEvent(SDL_MOUSEMOTION);

        ms += _Elapsed;
        frames++;
        if (ms > 1000) {
            _FramerateOut = frames;
            frames = 0;
            ms = 0;
        }

        if (_ShowFramesPerSecond )
            ShowFramesPerSec();

        if (_ShowCameraPos)
            ShowCameraPos();


        if (_ShowMousePositions && MousePositions != nullptr)  {
            MousePositions->SetText(0,"Mouse X " + IntToString(_Mouse.x) );
            MousePositions->SetText(1,"Mouse Y " + IntToString(_Mouse.y) );
        }

       glDepthFunc(GL_LEQUAL);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       if ( ! _ShowPanel) {

            RenderLight();
            Render();
            RenderCockpit();

            // ===================================
            // Engine Objekte
            // ===================================
            if (_ShaderChanged) {
                if (! list3D.empty() ) {
                    for (unsigned int i=0;i < list3D.size(); i++ )
                        list3D[i]->setActiveShader(_CurrentShader);
                }

                if (lightSource != nullptr)
                    lightSource->setActiveShader(_CurrentShader);

                if (cockpit != nullptr) {

                    if (cockpit->HasMesh())
                        cockpit->getCockpitMesch()->setActiveShader(_CurrentShader);
                }
            }
        }

        // ===================================ee
        // Alles für 2D Projektion vorbereiten
        //====================================
        Prepare2D();

        Render2D();
        RenderControlls();

        Render2DUserObject();

        Restore3D();

        SDL_GL_SwapWindow(window);
        end = Clock::now();
        _Elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = end;
     }
}

void InitGL::Done() {
}

void InitGL::Prepare2D() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
}

void InitGL::Restore3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDepthMask(1);
}

void InitGL::OnMainMenuStateChanged() {} // Override it in subclasses!

void InitGL::OnMouseMove(int &x, int &y, uint32 buttonstate) {

    for (uint i = 0; i < textfields.size(); i++) {

        if (textfields.at(i)->IsDragging() && (buttonstate & SDL_BUTTON_LMASK) != 0) {
            textfields.at(i)->OnDrag(x, y);
        }
    }


    for (uint i = 0; i < objects2D.size(); i++) {

        if (objects2D.at(i)->IsDragging() && (buttonstate & SDL_BUTTON_LMASK) != 0) {
            objects2D.at(i)->OnDrag(x, y);
        }
    }
}

void InitGL::OnLeftMouseButtonDown(int &x, int &y) {

    for (uint i = 0; i < textfields.size(); i++) {
        if (textfields.at(i)->Intersect(x, y))
            textfields.at(i)->OnStartDrag(x, y);
    }

    if (!objects2D.empty() ) {


        for (uint i = 0; i < objects2D.size(); i++)
        {
            if (objects2D.at(i)->intersect(x, y))
                objects2D[i]->OnStartDrag(x, y);

        }

    }

    if ( ! MainMenu->containerList.empty() && MainMenu != nullptr && MainMenu->Active()) {

        for ( uint i = 0; i< MainMenu->containerList.size(); i++) {

            if ( ! MainMenu->containerList.at(i)->controlls2D.empty() ) {

                _LockClick = true;
                for (uint j=0; j< MainMenu->containerList.at(i)->controlls2D.size(); j ++) {
                    if (MainMenu->containerList.at(i)->controlls2D.at(j)->intersect( x, y) ) {
                        MainMenu->containerList.at(i)->controlls2D.at(j)->OnClick();
                    }
                }
            }
        }
    }
}

void InitGL::OnLeftMouseButtonUp(int &x, int &y) {

    if ( ! MainMenu->containerList.empty() && MainMenu != nullptr && MainMenu->Active()) {

        for ( uint i = 0; i< MainMenu->containerList.size(); i++) {

            if ( ! MainMenu->containerList.at(i)->controlls2D.empty() ) {

                if ( _LockClick) {
                    for (uint j=0; j< MainMenu->containerList.at(i)->controlls2D.size(); j ++) {
                        if (MainMenu->containerList.at(i)->controlls2D.at(j)->intersect(x, y) ) {
                            MainMenu->containerList.at(i)->controlls2D.at(j)->OnRelease();
                        }
                    }
                }
                else
                    _LockClick = false;
            }
        }
    }

    for (uint i = 0; i < textfields.size(); i++) {
        if (textfields.at(i)->IsDragging() ) {
            textfields.at(i)->OnEndDrag(x, y);
        }
    }

    for (uint i = 0; i < objects2D.size(); i++) {
        if (objects2D.at(i)->IsDragging() ) {
            objects2D.at(i)->OnEndDrag(x, y);
        }
    }
}

void InitGL::sdl_die( std::string msg)
{
    fprintf(stderr,"%s: %s \n", msg.c_str(),SDL_GetError());

}



