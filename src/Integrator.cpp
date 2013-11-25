//
//  Integrator.cpp
//  motionMonstersMaskMaker
//
//  Created by Surya Mattu on 24/11/13.
//
//

#include "Integrator.h"

Integrator::Integrator(float _value) {
    value = _value;
}


Integrator::Integrator(float _value, float _damping, float _attraction) {
    value = _value;
    damping = _damping;
    attraction = _attraction;
}


void Integrator:: set(float v) {
    value = v;
}


void Integrator:: update() {
    if (targeting) {
        force += attraction * (target - value);
    }
    
    accel = force / mass;
    vel = (vel + accel) * damping;
    value += vel;
    
    force = 0;
}


void Integrator:: setTarget(float t) {
    targeting = true;
    target = t;
}


void Integrator:: noTarget() {
    targeting = false;
}

