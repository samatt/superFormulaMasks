//
//  Integrator.h
//  motionMonstersMaskMaker
//
//  Created by Surya Mattu on 24/11/13.
//
//

#ifndef __motionMonstersMaskMaker__Integrator__
#define __motionMonstersMaskMaker__Integrator__

#include "ofMain.h"

class Integrator {
public:
    
    Integrator() { };
    Integrator(float _value);
    Integrator(float _value, float _damping, float _attraction);
    
    const float DAMPING = 0.5f;
    const float ATTRACTION = 0.2f;
    
    float value;
    float vel;
    float accel;
    float force;
    float mass = 1;
    
    float damping = DAMPING;
    float attraction = ATTRACTION;
    bool targeting;
    float target;

    void set(float v);
    void update();
    void setTarget(float t);
    void noTarget();
    

    
};

#endif /* defined(__motionMonstersMaskMaker__Integrator__) */
