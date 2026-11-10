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
#include <utility>



void WinchBrake() {
    Winch.move(0);
      Winch1.brake();
      Winch2.brake();
      Winch3.brake();
}





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


 chassis.setPose(0, -62.5, 180); 
 
 Claw.set_value(true);
  pros::delay(200);
 Winch.move(127);
 pros::delay(200);
  Winch.move(0);
   pros::delay(100);
    Winch1.set_brake_mode(pros::MotorBrake::hold);
    Winch1.brake();
    Winch2.set_brake_mode(pros::MotorBrake::hold);
    Winch2.brake();
    Winch3.set_brake_mode(pros::MotorBrake::hold);
    Winch3.brake();
 chassis.moveToPoint(0, -65, 300,{.minSpeed = 127});
 chassis.moveToPoint(0, -62.5, 500, {.forwards = false, .minSpeed = 127});
 chassis.moveToPoint(0, -65, 300,{.minSpeed = 127});
 chassis.moveToPoint(0, -48, 1000,{.forwards = false, .minSpeed = 0});
chassis.turnToHeading(-90, 400);
 Winch.move(127);
  pros::delay(300);
  Winch.move(0);
WinchBrake();
 chassis.moveToPoint(20.5, -48, 800,{.forwards = false},false);
  Winch.move(-127);
 pros::delay(200);
  Winch.move(0);
   pros::delay(50);
 Claw.set_value(false);
 WinchBrake();
  chassis.moveToPoint(0, -60, 1000);
    chassis.moveToPoint(54, -60, 1500,{.forwards = false});
   chassis.turnToHeading(0, 400);
    chassis.moveToPoint(54, -62, 1000,{.forwards = false});
       pros::delay(300);
 Claw.set_value(true);
  Winch.move(127);
 pros::delay(200);
  Winch.move(0);
   pros::delay(100);
 WinchBrake();
   chassis.moveToPoint(54, -48, 750);
    chassis.turnToHeading(90, 400);
       chassis.moveToPoint(27.5, -48, 750,{.forwards = false});
         Winch.move(-127);
 pros::delay(300);
  Winch.move(0);
   pros::delay(50);
 Claw.set_value(false);
 WinchBrake();
   chassis.moveToPoint(54, -48, 750);
      chassis.turnToHeading(0, 400);

//chassis.moveToPose(-24, -36, 135, 1000);
// Claw.set_value(false);
// I love curves - Ethan
}




// Our Autonomous for the Top Middle goal during our Qualification matches
void BottomRed() {
  
 chassis.setPose(0, -62.5, 180); 
 
 Claw.set_value(true);
  pros::delay(200);
 Winch.move(127);
 pros::delay(200);
  Winch.move(0);
   pros::delay(100);
    Winch1.set_brake_mode(pros::MotorBrake::hold);
    Winch1.brake();
    Winch2.set_brake_mode(pros::MotorBrake::hold);
    Winch2.brake();
    Winch3.set_brake_mode(pros::MotorBrake::hold);
    Winch3.brake();
 chassis.moveToPoint(0, -65, 300,{.minSpeed = 127});
 chassis.moveToPoint(0, -62.5, 500, {.forwards = false, .minSpeed = 127});
 chassis.moveToPoint(0, -65, 300,{.minSpeed = 127});
 chassis.moveToPoint(0, -48, 1000,{.forwards = false, .minSpeed = 0});
chassis.turnToHeading(90, 400);
 Winch.move(127);
  pros::delay(300);
  Winch.move(0);
WinchBrake();
 chassis.moveToPoint(-20.5, -48, 800,{.forwards = false},false);
  Winch.move(-127);
 pros::delay(300);
  Winch.move(0);
   pros::delay(50);
 Claw.set_value(false);
 WinchBrake();
  chassis.moveToPoint(0, -48, 1000);
  chassis.turnToHeading(135, 250);

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
