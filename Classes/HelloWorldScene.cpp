/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "PluginHMS/PluginHMS.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;
/******************
 * Show logs
 ******************/
std::vector<std::string> msgbuf;
static void showMsg(const std::string& msg) {
    //
    Label *label = dynamic_cast<Label*>(Director::getInstance()->getNotificationNode());
    if (label == nullptr) {
        auto size = Director::getInstance()->getWinSize();
        label = Label::createWithSystemFont("test", "arial", 12);
        label->setAnchorPoint(Vec2(0,0));
        label->setTextColor(Color4B(0, 255, 0, 255));
        label->setPosition(10, size.height*0.1);
        Director::getInstance()->setNotificationNode(label);
    }

    msgbuf.push_back(msg);
    if (msgbuf.size() > 10) {
        msgbuf.erase(msgbuf.cbegin());
    }


    std::string text = "";
    for (int i = 0; i < msgbuf.size(); i++) {
        std::stringstream buf;
        buf << i << " " << msgbuf[i] << "\n";
        text = text + buf.str();
    }

    label->setString(text);
    cocos2d::log("Log: %s", msg.c_str());
}

class PluginHMSListener : public sdkbox::HMSListener {
public:
    PluginHMSListener(HelloWorld* scene): hw(scene) {
    }
    void onLogin(int code, const std::string& errorOrJson) {
        cocos2d::log("HMS onLogin: %d, %s", code, errorOrJson.c_str());
        if (0 != code) {
            showMsg("login failed:" + errorOrJson);
            return;
        }
        rapidjson::Document doc;
        doc.Parse(errorOrJson.c_str());

        std::ostringstream ss;
        ss << "Login success:" << doc["displayName"].GetString();
        showMsg(ss.str());
    }
private:
    HelloWorld* hw;
};
Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //mTitle = Label::createWithSystemFont("Main", "arial", 24);
    //mTitle->setPosition(cocos2d::Vec2(visibleSize.width/2, visibleSize.height - 100));
    //addChild(mTitle);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr) {
        problemLoading("'HelloWorld.png'");
    } else {
        // position the sprite on the center of the screenVec2(visibleSize.width  + origin.x, visibleSize.height  + origin.y)
        sprite->setScale(0.8);
        sprite->setPosition(Vec2(visibleSize.width/2 + 150, visibleSize.width/2 - origin.x));
        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }

    auto spriteHuawei = Sprite::create("pngwave.png");
    if (spriteHuawei == nullptr) {
        problemLoading("'pngwave.png'");
    } else {
        // position the sprite on the center of the screenVec2(visibleSize.width  + origin.x, visibleSize.height  + origin.y)
        spriteHuawei->setScale(0.3);
        spriteHuawei->setPosition(Vec2(visibleSize.height/2 , visibleSize.width/2 - origin.x));
        // add the sprite as a child to this layer
        this->addChild(spriteHuawei, 0);
    }

    mMenu = Menu::create();
    addChild(mMenu);
    showMenu("");

    sdkbox::PluginHMS::setListener(new PluginHMSListener(this));
    sdkbox::PluginHMS::buoyShow();

    return true;
}
void HelloWorld::showMenu(const std::string& menuName) {
    mMenu->removeAllChildrenWithCleanup(true);
    std::string title = menuName;
    if (title.empty()) {
        title = "Main";
    }
    //mTitle->setString(title);
    if (0 == title.compare("Main")) {
        genMainMenu();
    } else if (0 == title.compare("Account")) {
        genAccountMenu();
    } else {
        CCLOG("Error, Unknow menu type");
    }

    mMenu->alignItemsVerticallyWithPadding(10);
}

void HelloWorld::genMainMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Account Test", "arial", 24), [this](Ref*){
        this->showMenu("Account");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Achievement", "arial", 24), [this](Ref*){
        this->showMenu("");
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("IAP", "arial", 24), [this](Ref*){
        this->showMenu("");
    }));
}
void HelloWorld::genAccountMenu() {
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Login", "arial", 24), [](Ref*){
        showMsg("to login...");
        // sdkbox::PluginHMS::login(0); // slient login
        sdkbox::PluginHMS::login(1); // login (id token)
        // sdkbox::PluginHMS::login(2); // login (author code)
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Logout", "arial", 24), [](Ref*){
        showMsg("to logout...");
        sdkbox::PluginHMS::logout();
    }));
    mMenu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Back", "arial", 24), [this](Ref*){
        this->showMenu("");
    }));
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
