#pragma once

#include "lemlib/api.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include "subsystems.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "main.h"
#include "relocalization.hpp"
#include "subsystems.cpp"








void FourPush() {

}




void SAWP() {
  chassis.setPose(-49.25, -15.75, 180);
 
}



void NewSkills() {
 chassis.setPose(-49.25, 15.75, 0);
 
}


//-16, -50


void Skills() {

  chassis.setPose(-49.25, 15.75, 0);

}




// Our Autonomous for the Top Middle goal during our Qualification matches
void TopGoalQUALS() {
     chassis.setPose(0, -2, 0);
 
}

void BottomGoalQUALS() {
      chassis.setPose(0, -2, 0);

}

// //if other team has solo awp
// void noAuto() {
//   chassis.setPose(0, 0, 0);
//   chassis.moveToPoint(0, 5, 2000, {.maxSpeed = 45});
// } 



void ElimsHigh() {
 
}


void NewFOURTHREE(){

  chassis.setPose(0, -2, 0);

}
