#pragma once
#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
//#include "pros/abstract motor.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/distance.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "subsystems.hpp"//Our motors sensors and electronics
#include "autos.hpp"
#include "relocalization.hpp"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            // lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */




///////////
///////talinn likes men a lot

////////





/////////////////////////
void autonomous() {
    // set chassis pose

//chassis.moveToPoint(0, 3, 2000);
 //ElimsHigh() ;
//TopGoalQUALS();
//BottomGoalQUALS();
Skills();
//SAWP();
//ElimsHigh();
//ElimsLow();
//FourPush();
//distance();
}
////////


 bool clawTrue = false;
/**
 * Runs in driver control
 */
void opcontrol() {

    // controller
    // loop to continuously update motors
    pros::Controller controller(pros::E_CONTROLLER_MASTER);
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);

    double Sens =  ClawDist.get_distance();
    
    if (Sens <= 40){
        Claw.set_value(true);
        clawTrue = true;
    }


  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
  if (clawTrue == false){
    Claw.set_value(true);
    clawTrue = true;
    pros::delay(200);
  }
  else if(clawTrue == true){
    Claw.set_value(false);
    clawTrue = false;
    pros::delay(200);
  }
    
//    break; 
//}   

    //Delay so it doesn't hog resources
    pros::delay(50);

    // }
}


 if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)){

Relocalization::printSensorDebug(FrontDistSensor, RightDistSensor, LeftDistSensor);

} 

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    Winch.move(-75);
 }
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
    Winch.move(127);
 }
    else {
    Winch.move(0);
    Winch1.set_brake_mode(pros::MotorBrake::hold);
    Winch1.brake();
    Winch2.set_brake_mode(pros::MotorBrake::hold);
    Winch2.brake();
      Winch3.set_brake_mode(pros::MotorBrake::hold);
    Winch3.brake();
    }
      

        


        // delay to save resources
        pros::delay(20);
    }
}