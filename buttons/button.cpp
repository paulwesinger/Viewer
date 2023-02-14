#include "button.h"
#include "../logs/logs.h"
#include "../defaults.h"
#include "../utils/utils.h"

CButton::CButton(int resx, int resy, Shader * sh) :
    Base2D(resx,resy, sh) {

    init();
}
CButton::CButton(int resx, int resy, std::string path, std::string text, Shader * sh) :
    Base2D(resx, resy,path, sh){
    _Text = text;
    init();
}

CButton::CButton(int resx, int resy, std::string imagereleased,std::string imagepressed, std::string text, Shader *sh) :
    Base2D(resx,resy,imagereleased,sh)
{
    ImageReleased = imagereleased;
    ImagePressed = imagepressed;
    _Text = text;
    init();
}

void CButton::init() {

    clicked = false;

     _ButtonColors = colorscheme.setScheme(DARK);
     _BtnPressed  = new Base2D(_ResX,_ResY,ImagePressed,shader);

    _BtnPressed->setPos(_Pos.x,_Pos.y);
    _BtnPressed->setSize(Width(),Height());

    setWidth(BUTTON::DEFAULT_WIDTH);
    setHeight(BUTTON::DEFAULT_HEIGHT);
    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;

    _AlphaText    = TEXT::ALPHA_ENABLED;
    _Alpha_Image  = BUTTON::ALPHA_IMAGE_ENABLED;

    _ImageCol = BUTTON::COLOR_DEFAULT_IMAGE;
    _TextCol  = BUTTON::COLOR_DEFAULT_TEXT;
}
CButton::~CButton() {
    delete _BtnPressed;

}

// ----------------------------------------------
// Look and feel
// ----------------------------------------------

void CButton::setText(std::string t) { _Text = t; }

void CButton::enable() {
    Base2D::disable();
    _Enabled = true;
    _AlphaText   = TEXT::ALPHA_ENABLED;
    _Alpha_Image = BUTTON::ALPHA_IMAGE_ENABLED;
}

void CButton::disable() {
    Base2D::disable();
    _Enabled = false;
    _AlphaText    = TEXT::ALPHA_DISABLED;
    _Alpha_Image  = BUTTON::ALPHA_IMAGE_DISABLED;
}

std::string CButton::Text() {return  _Text; }
bool CButton::Enabled() { return _Enabled; }


// Handler Func
void CButton::AddHandler(FP handler) {
    clickFunc = handler;
}

void CButton::animateClick(){

}
void CButton::releaseClick() {}

void CButton::alignToSize(int w, int h) {}

void CButton::setbuttonColors(glm::vec3 imagecol, glm::vec3 textcol) {
    _ImageCol = imagecol;
    _TextCol  = textcol;

    Base2D::setColor(glm::vec4(imagecol,_Alpha_Image));
}

void CButton::setScale(float s) {}

// -----------------------------------------------
// Event Handling
// -----------------------------------------------
/*
void CButton::OnClick() {
    //clickFunc();
}  // Must be overwritten
*/
//void CButton::OnRelease(){}

void CButton::OnFocus() {
    loginfo("OnFocus","CButton::OnFocus");
}

void CButton::Render( ) {}


//--------------------------------------------------
// Defaullt Buttons for use
// -------------------------------------------------

CTextButton::CTextButton( int resx, int  resy, Shader * sh) :
    CButton(resx, resy, sh)

{
    _Text = "NO";
    _Pos.x = 0;
    _Pos.y = 0;
    btnText = new TextRender(resx, resy,sh);
    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;
}

CTextButton::CTextButton(int resx, int resy, std::string path, std::string text, sPoint pos, Shader * sh):
    CButton(resx, resy, path,text,sh){

    _Pos = pos;

    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;

    setText(text);
    btnText = new TextRender(resx, resy,sh);
    btnText->AddString(text);
    btnText->SetPos(textPos);
    btnText->SetTextColor(glm::vec4(_TextCol,1.0));
    btnText->SetHasBottom(false);
    btnText->SetHasHeader(false);
    btnText->SetHasBackground(false);
    btnText->SetHasTexture(false);
    btnText->SetAlignRight(false);
}

void CTextButton::animateClick() {
    if (btnText != nullptr) {
        sPoint p;
        p = btnText->Pos();
        p.x += 2;
        p.y += 2;

        btnText->SetPos(p);
    }
}

void CTextButton::releaseClick() {
    if (btnText != nullptr) {
        sPoint p;
        p = btnText->Pos();
        p.x -= 2;
        p.y -= 2;
        btnText->SetPos(p);
    }
}

void CTextButton::setbuttonColors(glm::vec3 imagecol, glm::vec3 textcol)  {

    CButton::setbuttonColors(imagecol, textcol);
    btnText->SetTextColor(glm::vec4(textcol,1.0));
}

void CTextButton::alignToSize(int w, int h){
    if (btnText != nullptr)
        btnText->AlignToRectSize(w,h);
}

void CTextButton::setScale(float s) {
    _TextScale = s;
    if (btnText)
        btnText->SetScale(_TextScale);
}

// overritten

void CTextButton::setPos(int x, int y) {
    Base2D::setPos(x,y);

    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;

    sPoint p;
    p.x = (int) textPos.x;
    p.y = (int) textPos.y;
    btnText->SetPos(p);
}

void CTextButton::setSize(int w, int h) {
    Base2D::setSize(w, h);

    _BtnPressed->setSize(w,h);

    // Todo : setSize in TextRender

}

void CTextButton::Render() {

    Base2D::setColor(glm::vec4(1.0,1.0,1.0,_Alpha_Image));
    Base2D::Render();

    btnText->Render();
}

void CTextButton::OnClick() {
    clickFunc();
    animateClick();
}
void CTextButton::OnRelease(){
    releaseClick();
}

//--------------------------------------------------
// Image Button for use
// -------------------------------------------------

CImageButton::CImageButton( int resx, int  resy, Shader * sh) :
    CButton(resx, resy, sh)

{
    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;
    // init stuff
    textImage = new Base2D(resx, resy, "images/Add.png",sh);
    textImage->setPos(0,0);
}

CImageButton::CImageButton(int resx, int resy, std::string releaseimage, std::string pressedimage, std::string pathtext, Shader * sh):
    CButton(resx,resy,releaseimage,pressedimage, pathtext,sh) {

    _TextPath = pathtext;
    _Pos = sPoint(0,0);

    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;

    sPoint p;
    p.x = (int) textPos.x;
    p.y = (int) textPos.y;

    textImage = new Base2D(resx, resy,_TextPath,sh);
    textImage->setColor(glm::vec4(BUTTON::COLOR_DEFAULT_TEXT,_AlphaText));
    textImage->setPos(0,0);
    // init stuff
}

/*
CImageButton::CImageButton( int resx, int resy, std::string releaseimage, std::string pressedimage, std::string pathtext, sPoint pos, Shader * sh):
    CButton(resx,resy,releaseimage,pressedimage,pathtext,sh) {

    _TextPath = pathtext;
    _Pos = pos;

    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;

    sPoint p;
    p.x = (int) textPos.x;
    p.y = (int) textPos.y;

    textImage = new Base2D(resx, resy,_TextPath,sh);
    textImage->setColor(glm::vec4(BUTTON::COLOR_DEFAULT_TEXT,_AlphaText));
    textImage->setPos(0,0);

}
*/

CImageButton::CImageButton(int resx, int resy, std::string releaseimage, std::string pressedimage , std::string pathtext, sPoint pos,Shader * sh):
    CButton(resx, resy, releaseimage, pressedimage, pathtext,sh){

    _BtnPressed->setPos(pos.x,pos.y);
    _TextPath = pathtext;

    textPos.x = _Pos.x + BUTTON::X_MARGIN;
    textPos.y = _Pos.y + BUTTON::Y_MARGIN;

    sPoint p;
    p.x = (int) textPos.x;
    p.y = (int) textPos.y;

    textImage = new Base2D(resx, resy,_TextPath,sh);
    textImage->setColor(glm::vec4(BUTTON::COLOR_DEFAULT_TEXT,_AlphaText));
    textImage->setPos(0,0);
    // init stuff
}

void CImageButton::animateClick() {

    if (textImage != nullptr) {
        sPoint p = textImage->Pos();
        textImage->setPos(p.x+2, p.y+2);
    }
}

void CImageButton::releaseClick() {


    if (textImage != nullptr) {
        sPoint p = textImage->Pos();

        textImage->setPos(p.x-2, p.y-2);
    }

    if (clicked) {
        _BtnPressed->setSize(Width()+2,Height()+2);
    }
    clicked = false;
}

void CImageButton::setbuttonColors(glm::vec3 imagecol, glm::vec3 textcol)  {

    CButton::setbuttonColors(imagecol, textcol);
    textImage->setColor(glm::vec4(textcol,_AlphaText));
}
// overritten

void CImageButton::setPos(int x, int y) {
    Base2D::setPos(x,y);
    textImage->setPos(x,y);  //_Pos.x,_Pos.y);

    _BtnPressed->setPos(x,y);
}

void CImageButton::setScale(float s) {}
void CImageButton::setSize(int w, int h) {
    Base2D::setSize(w, h);
    _BtnPressed->setSize(w,h);

    if (textImage != nullptr) {
        textImage->setSize(w - 2, h - 2);
    }
}

void CImageButton::Render() {

    Base2D::setColor(glm::vec4(1.0,0.0,0.0,_Alpha_Image));
    Base2D::Render();

    if ( clicked) {
        _BtnPressed->setSize(Width() - 1,Height() - 1);
        setColor(glm::vec4(1,1,1,1));
        _BtnPressed->Render();
    }
    //else
        //setColor(glm::vec4(1,0,0,1));

    textImage->setColor(glm::vec4(1.0,1.0,1.0,_AlphaText));
    textImage->Render();
}

void CImageButton::OnClick() {
    if ( ! clicked) {
        clickFunc();
        clicked = true;
        _BtnPressed->setSize(Width()-2,Height()-2);
        setColor(glm::vec4(1,1,1,1));
    }
    animateClick();
}
void CImageButton::OnRelease(){
    releaseClick();
    setColor(glm::vec4(1,0,0,1));
}








