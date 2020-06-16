#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.hpp"
#include "EnemyEmitter.hpp"
#include "GameController.hpp"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "TransformObject.h"


typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

class Emitter;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void checkCollisions();
        void checkBoundary();
        float modulateAccel(float);
        void updateSprite();
        void moveSprite(MoveDir);
        void stopSprite();
        void startAccel();
        void stopAccel();
        void startLevel(int);
        void levelOne();
        void levelTwo();
        void levelThree();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        Emitter *player;
        vector<EnemyEmitter *> invaders;
        vector<Emitter *> minions;
        GameController::GameStates gameState;
    
        ParticleEmitter explosion;
        TurbulenceForce *turbForce;
        GravityForce *gravityForce;
        ImpulseRadialForce *radialForce;
    
        ParticleEmitter boosterParticle;
        TurbulenceForce *boosterTurbForce;
        GravityForce *boosterGravityForce;
        ImpulseRadialForce *boosterRadialForce;
    
//        ParticleEmitter minionBoosterParticle1;
//        TurbulenceForce *minionBoosterTurbForce1;
//        GravityForce *minionBoosterGravityForce1;
//        ImpulseRadialForce *minionBoosterRadialForce1;
//    
//        ParticleEmitter minionBoosterParticle2;
//        TurbulenceForce *minionBoosterTurbForce2;
//        GravityForce *minionBoosterGravityForce2;
//        ImpulseRadialForce *minionBoosterRadialForce2;
    
        int numEmitters;
        int numMinions;
        int score;
        bool gameOver = false;
        bool win = false;

		ofImage playershipImage;
        ofImage minionsImage;
        ofImage enemyshipImage;
        ofImage backgroundImage;
        ofImage playerBulletImage;
        ofImage minionBulletImage;
		ofVec3f mouse_last;
        ofTrueTypeFont font;
        ofVec3f startAccelPoint;
        ofSoundPlayer gameplaySong;
        ofSoundPlayer sfxEnemyCollision;
        ofVideoPlayer menuScreenBg;

		ofxFloatSlider playerRate;
		ofxFloatSlider playerLife;
		ofxVec3Slider playerVelocity;
        ofxFloatSlider invaderRate;
        ofxFloatSlider invaderLife;
        ofxVec3Slider invaderVelocity;
		ofxLabel screenSize;

		ofxPanel guiPlayer;
        ofxPanel guiInvader;

        MoveDir moveDir;
        bool accel;
        bool spacebarDown = false;
        bool bulletFire = false;
        bool spaceHold = false;
        bool bHelpText = true;
        bool imageLoaded;
        bool bHide;
        bool isLevelOne;
        bool isLevelTwo;
        bool isLevelThree;
        bool reset;
    
        float spacebarStrX;
        float spacebarStrY;
        int numberOfTicks = 0;
        float xPos;
        float yPos;
        unsigned long actualTime, sucessTimer;
        unsigned int sucessTimeDelta;
        int count;
    
        float margin;
        float emitterWidth;
        ofPoint winsize;
        float spacing;
        float x;
    
        ofVec2f bgPos1;
        ofVec2f bgPos2;
    
        ofImage background;
        ofImage background2;
    
        float minionMargin;
        float minionEmitterWidth;
        float minionSpacing;
        float xMinionDist;
};
