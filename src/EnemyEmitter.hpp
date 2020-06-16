//
//  EnemyEmitter.hpp
//  Game_Project_1
//
//  Created by Parth Patel on 3/9/19.
//

#ifndef EnemyEmitter_hpp
#define EnemyEmitter_hpp

#include "Sprite.hpp"


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class EnemyEmitter : public BaseObject {
public:
    EnemyEmitter(SpriteSystem *);
    void draw();
    void start();
    void stop();
    void setLifespan(float);
    void setVelocity(ofVec3f);
    void setChildImage(ofImage);
    void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
    void setImage(ofImage);
    void setRate(float);
    float maxDistPerFrame();
    void update();
    void setChildHealth(float childHealth);
    void setHealth(float health);
    SpriteSystem *sys;
    float rate;
    ofVec3f velocity;
    float lifespan;
    float health;
    float childHealth;
    bool started;
    float lastSpawned;
    ofImage childImage;
    ofImage image;
    bool drawable;
    bool haveChildImage;
    bool haveImage;
    float width, height;
    float childWidth, childHeight;
    float speed;
    ofSoundPlayer bulletSound;
};

#endif
