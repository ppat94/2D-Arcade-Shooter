#pragma once
//----------------------------------------------------------------------------------
//
//  Basic Sprite Class
//
//
//  Parth Patel (013705718) - CS 134 SJSU

#include "ofMain.h"

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
    BaseObject();
    ofVec3f trans, scale;
    float    rot;
    bool    bSelected;
    void setPosition(ofVec3f);
    ofVec3f getPosition();
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
    Sprite();
    void update();
    void draw();
    float age();
    void setImage(ofImage);
    float speed;    //   in pixels/sec
    ofVec3f velocity; // in pixels/sec
    ofImage image;
    float birthtime; // elapsed time in ms
    float lifespan;  //  time in ms
    string name;
    bool haveImage;
    float width, height;
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
    void add(Sprite);
    void remove(int);
    void update();
    int removeNear(ofVec3f point, float dist);
    void draw();
    vector<Sprite> sprites;
    
};
