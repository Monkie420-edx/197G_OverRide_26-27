#pragma once
#include "api.h"
#include "lemlib/api.hpp"
#include "pros/distance.hpp"
//Our motors used in intaking


pros::MotorGroup Winch({-10, 9, -1});
//Our Ai Vision sensor for our Color sort

pros::Distance FrontDistSensor(16);
pros::Distance RightDistSensor(14);
pros::Distance LeftDistSensor(15);
//Pistons for our Wings and our Matchloader
pros::ADIDigitalOut Wing(3);
pros::ADIDigitalOut Loader(1);
pros::ADIDigitalOut MidGoal(2);


pros::MotorGroup leftMotors({11, -12, -13});
// right motor group
pros::MotorGroup rightMotors({-20, 19, 18});

// Inertial Sensor on port 10
pros::Imu imu(20);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
// pros::Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
pros::Rotation verticalEnc(-10);

// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
// lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, -5);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, -0.5);



// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450, // drivetrain rpm is 360
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(5, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            60, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(6.7, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             65, // derivative gain (kD)
                                             0, // anti windup
                                             0, // small error range, in inches
                                             0, // small error range timeout, in milliseconds
                                             0, // large error range, in inches
                                             0, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            nullptr, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);
