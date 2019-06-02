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
#include "ui/UIButton.h"

USING_NS_CC;

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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    auto button = ui::Button::create("");
    button->setTitleText("Generate Texture (Bugged via event handler)");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    button->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eventType)
        {
            if (eventType == ui::Widget::TouchEventType::ENDED)
            {
                this->CreateTexture("output_bad.png");
            }
        });
    addChild(button, 40);

    button = ui::Button::create("");
    button->setTitleText("Generate Texture (Working using scheduleOnce)");
    button->setTitleFontSize(50);
    button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
    button->addTouchEventListener([this](Ref*, ui::Widget::TouchEventType eventType)
        {
            if (eventType == ui::Widget::TouchEventType::ENDED)
            {
                this->scheduleOnce([this](float)
                    {
                        this->CreateTexture("output_good.png");
                    }, 0.0f, "GenTex");
            }
        });
    addChild(button, 40);

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //}
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::CreateTexture(const std::string& outputFilename)
{
    auto renderTexture = RenderTexture::create(2048, 2048, Texture2D::PixelFormat::RGBA8888);
    if (nullptr == renderTexture)
    {
        return;
    }

    renderTexture->beginWithClear(0, 0, 0, 0);

    std::stringstream ss;
    float x = 0;
    float y = 0;
    for (size_t i = 0; x < 2048 && y < 2048; ++i)
    {
        ss.str("");
        ss << "x:" << x << ", y:" << y;
        auto label = Label::createWithTTF(ss.str(), "fonts/Marker Felt.ttf", 30);
        label->setTextColor(Color4B(RandomHelper::random_int(0, 255), RandomHelper::random_int(0, 255), RandomHelper::random_int(0, 255), 255));
        label->setPositionNormalized(Vec2(0.5f, 0.5f));
        auto node = Node::create();
        node->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        node->setContentSize(label->getContentSize());
        
        node->setPosition(Vec2(x, y));
        node->addChild(label);
        node->visit();

        x += 100;
        y += 100;
    }
    renderTexture->end();
    renderTexture->saveToFile(outputFilename);
    Director::getInstance()->getRenderer()->render();

    ss.str("");
    ss << ">>> FINAL COORDS >>> " << "x:" << x << ", y:" << y;
    CCLOG("%s", ss.str().c_str());
}