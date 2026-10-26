#include "relocalization.hpp"
#include "pros/rtos.hpp"
#include "pros/llemu.hpp"



namespace Relocalization {
    
    double getFilteredDistance(pros::Distance& sensor) {
        double sum = 0;
        int validCount = 0;
        
        for (int i = 0; i < Config::SAMPLE_COUNT; i++) {
            if (sensor.get_confidence() >= Config::MIN_CONFIDENCE) {
                double reading = sensor.get();
                if (reading > 0) {
                    sum += reading;
                    validCount++;
                }
            }
            pros::delay(Config::SAMPLE_DELAY_MS);
        }
        
        // Check if we got enough valid samples
        if (validCount < Config::MIN_VALID_SAMPLES) {
            return -1;
        }
        
        // Convert from mm to inches and check range
        double avgInches = (sum / validCount) / Config::MM_TO_INCHES;
        return (avgInches > Config::MAX_VALID_DISTANCE) ? -1 : avgInches;
    }
    
    bool correctX(lemlib::Chassis& chassis, pros::Distance& sensor, 
                  double sensorOffset, double wallX, bool facingPositiveX) {
        double dist = getFilteredDistance(sensor);
        if (dist < 0) return false;
        
        lemlib::Pose currentPose = chassis.getPose();
        double correctedX;
        
        if (facingPositiveX) {
            // Sensor faces +X (toward right wall)
            // Robot X = wall X - distance - offset
            correctedX = wallX - dist - sensorOffset;
        } else {
            // Sensor faces -X (toward left wall)
            // Robot X = wall X + distance + offset
            correctedX = wallX + dist + sensorOffset;
        }
        
        chassis.setPose(correctedX, currentPose.y, currentPose.theta);
        return true;
    }
    
    bool correctY(lemlib::Chassis& chassis, pros::Distance& sensor, 
                  double sensorOffset, double wallY, bool facingPositiveY) {
        double dist = getFilteredDistance(sensor);
        if (dist < 0) return false;
        
        lemlib::Pose currentPose = chassis.getPose();
        double correctedY;
        
        if (facingPositiveY) {
            // Sensor faces +Y (toward top wall)
            // Robot Y = wall Y - distance - offset
            correctedY = wallY - dist - sensorOffset;
        } else {
            // Sensor faces -Y (toward bottom wall)
            // Robot Y = wall Y + distance + offset
            correctedY = wallY + dist + sensorOffset;
        }
        
        chassis.setPose(currentPose.x, correctedY, currentPose.theta);
        return true;
    }
    
    bool correctAtCorner(lemlib::Chassis& chassis, 
                         pros::Distance& xSensor, pros::Distance& ySensor,
                         double wallX, double wallY,
                         bool facingPositiveX, bool facingPositiveY) {
        // Correct X first, then Y
        bool xSuccess = correctX(chassis, xSensor, Offsets::FRONT_SENSOR_X, 
                                 wallX, facingPositiveX);
        bool ySuccess = correctY(chassis, ySensor, Offsets::RIGHT_SENSOR_Y, 
                                 wallY, facingPositiveY);
        return xSuccess && ySuccess;
    }
    
    void printSensorDebug(pros::Distance& front, pros::Distance& right, 
                          pros::Distance& left) {
        pros::lcd::print(4, "Front: %.1f in", getFilteredDistance(front));
        pros::lcd::print(5, "Right: %.1f in", getFilteredDistance(right));
        pros::lcd::print(6, "Left:  %.1f in", getFilteredDistance(left));
    }
}