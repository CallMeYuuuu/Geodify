#include "SwelvyBG.hpp"
#include <Geode/loader/Mod.hpp>
#include <random>

bool SwelvyBG::init(float widthmult, float hightmult, float minspeed, float maxspeed) {
    if (!CCNode::init())
        return false;

    this->setID("SwelvyBG");
    
    auto winSize = CCDirector::get()->getWinSize();
    this->setContentSize(winSize);
    this->setAnchorPoint({ 0.f, 0.f });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sign(0, 1);
    std::uniform_real_distribution<float> dis(3.f, 9.f);

    float y = m_obContentSize.height + 5;
    int idx = 0;
    
    for (auto layer : std::initializer_list<std::pair<ccColor3B, const char*>> {
        { ccc3(41, 255, 184), "geode.loader/swelve-layer3.png" },
        { ccc3(0, 221, 217), "geode.loader/swelve-layer0.png" },
        { ccc3(37, 173, 223), "geode.loader/swelve-layer1.png" },
        { ccc3(21, 94, 196), "geode.loader/swelve-layer2.png" },
        { ccc3(27, 54, 186), "geode.loader/swelve-layer1.png" },
        { ccc3(17, 22, 168), "geode.loader/swelve-layer0.png" },
    }) {
        float speed = dis(gen);
        if (sign(gen) == 0) {
            speed = -speed;
        }
        ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};

        auto sprite = CCSprite::create(layer.second);
        auto rect = sprite->getTextureRect();
        sprite->setUserObject("width", CCFloat::create(rect.size.width * widthmult));
        rect.size = CCSize{winSize.width * widthmult, rect.size.height * hightmult};

        std::string layerID = fmt::format("layer-{}", idx);
        sprite->setID(layerID);
        sprite->getTexture()->setTexParameters(&params);
        sprite->setTextureRect(rect);
        sprite->setAnchorPoint({ 0, 1 });
        sprite->setContentSize({winSize.width * widthmult, sprite->getContentSize().height});
        sprite->setColor(layer.first);
        sprite->setPosition({0, y});
        sprite->schedule(schedule_selector(SwelvyBG::updateSpritePosition));
        sprite->setUserObject("speed", CCFloat::create(speed));
        this->addChild(sprite);

        y -= m_obContentSize.height / 6;
        idx += 1;
    
    }
    return true;
}

void SwelvyBG::updateSpritePosition(float dt) {
    auto speed = static_cast<CCFloat*>(this->getUserObject("speed"))->getValue();
    auto width = static_cast<CCFloat*>(this->getUserObject("width"))->getValue();

    auto sprite = typeinfo_cast<CCSprite*>(this);
    auto rect = sprite->getTextureRect();

    float dX = rect.origin.x - speed * dt;
    if(dX >= std::abs(width)) {
        dX = 0;
    }

    rect.origin = CCPoint{dX, 0};
    sprite->setTextureRect(rect);
}

SwelvyBG* SwelvyBG::create(float widthmult, float m, float minspeed, float maxspeed) {
    auto ret = new SwelvyBG();
    if (ret->init(widthmult,m,minspeed,maxspeed)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
