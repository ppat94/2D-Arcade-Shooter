//
//  EnemyEmitter.cpp
//  Game_Project_1
//
//  Created by Parth Patel on 3/9/19.
//

#include "ofMain.h"
#include "EnemyEmitter.hpp"

//  Create a new Emitter - needs a SpriteSystem
//
EnemyEmitter::EnemyEmitter(SpriteSystem *spriteSys) {
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
    childWidth = 10;
    childHeight = 10;
    speed = 0.3;
    health = 1000;
//    bulletSound.load("music/bullet.wav");
    //    velocity = ofVec3f(0, 0, 0);
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void EnemyEmitter::draw() {
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
void EnemyEmitter::update() {
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
//        bulletSound.play();
    }
    sys->update();
}

// Start/Stop the emitter.
//
void EnemyEmitter::start() {
    //    if (started)
    //        return;
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void EnemyEmitter::stop() {
    started = false;
}


void EnemyEmitter::setLifespan(float life) {
    lifespan = life;
}

void EnemyEmitter::setVelocity(ofVec3f v) {
    velocity = v;
}

void EnemyEmitter::setChildImage(ofImage img) {
    childImage = img;
    haveChildImage = true;
    childWidth = img.getWidth();
    childHeight = img.getHeight();
}

void EnemyEmitter::setImage(ofImage img) {
    image = img;
    haveImage = true;
}

void EnemyEmitter::setHealth(float hp) {
    health = hp;
}

void EnemyEmitter::setChildHealth(float childHp) {
    childHealth = childHp;
}

void EnemyEmitter::setRate(float r) {
    rate = r;
}

float EnemyEmitter::maxDistPerFrame() {
    return  velocity.length() / ofGetFrameRate();
}
