


#include "ofMain.h"
#include "Emitter.hpp"

//  Parth Patel (013705718) - CS 134 SJSU

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
    sys = spriteSys;
    lifespan = 3000;    // milliseconds
    started = false;
    
    lastSpawned = 0;
    rate = 1;    // sprites/sec
    haveChildImage = false;
    haveImage = false;
    velocity = ofVec3f(100, 100, 0);
    drawable = true;
    width = 50;
    height = 50;
    childWidth = 20;
    childHeight = 20;
    speed = 500;
    health = 500;
    bulletSound.load("music/bullet.wav");
//    velocity = ofVec3f(0, 0, 0);
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
    if (drawable) {
        
        if (haveImage) {
            image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
        }
        else {
            ofSetColor(255, 0, 0);
            ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
        }
    }
    
    // draw sprite system
    //
    sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
    if (!started) return;
    
    float time = ofGetElapsedTimeMillis();
    if ((time - lastSpawned) > (1000.0 / rate)) {
        // spawn a new sprite
        Sprite sprite;
        if (haveChildImage) sprite.setImage(childImage);
        sprite.velocity = velocity;
        sprite.lifespan = lifespan;
        sprite.setPosition(trans);
        sprite.birthtime = time;
        sprite.width = childWidth;
        sprite.height = childHeight;
        sys->add(sprite);
        lastSpawned = time;
        bulletSound.setVolume(0.5);
        bulletSound.play();
    }
    sys->update();
}

// Start/Stop the emitter.
//
void Emitter::start() {
//    if (started)
//        return;
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
    started = false;
}


void Emitter::setLifespan(float life) {
    lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
    velocity = v;
}

void Emitter::setChildImage(ofImage img) {
    childImage = img;
    haveChildImage = true;
    childWidth = img.getWidth();
    childHeight = img.getHeight();
}

void Emitter::setImage(ofImage img) {
    image = img;
    haveImage = true;
}

void Emitter::setHealth(float hp) {
    health = hp;
}

void Emitter::setChildHealth(float childHp) {
    childHealth = childHp;
}

void Emitter::setRate(float r) {
    rate = r;
}

float Emitter::maxDistPerFrame() {
    return  velocity.length() / ofGetFrameRate();
}
