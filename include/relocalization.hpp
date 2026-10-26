#pragma once
#include "lemlib/chassis/chassis.hpp"
#include "pros/distance.hpp"

namespace Relocalization {
    // Field wall positions (VEX field is 144" x 144", origin at center)
    namespace Field {
        constexpr double LEFT_WALL = -70.87;
        constexpr double RIGHT_WALL = 70.87;
        constexpr double TOP_WALL = 70.87;
        constexpr double BOTTOM_WALL = -70.87;
    }

    // Distance from robot CENTER to each sensor face
    namespace Offsets {
        constexpr double FRONT_SENSOR_X = 8.5;  // inches - robot center to front sensor face
        constexpr double RIGHT_SENSOR_Y = 4;  // inches - robot center to right sensor face
        constexpr double LEFT_SENSOR_Y = 4;   // inches - robot center to left sensor face
    }

    // Sensor filtering configuration
    namespace Config {
        constexpr int SAMPLE_COUNT = 5;           // Number of samples to average
        constexpr int SAMPLE_DELAY_MS = 5;        // Delay between samples
        constexpr int MIN_CONFIDENCE = 30;        // Minimum confidence (0-63)
        constexpr int MIN_VALID_SAMPLES = 3;      // Minimum valid samples required
        constexpr double MAX_VALID_DISTANCE = 60.0; // Max reliable distance in inches
        constexpr double MM_TO_INCHES = 25.4;     // Conversion factor
    }

    /**
     * Get filtered distance reading from a sensor
     * @param sensor The distance sensor to read
     * @return Filtered distance in inches, or -1 if invalid
     */
    double getFilteredDistance(pros::Distance& sensor);

    /**
     * Correct X position using a distance sensor facing a wall
     * @param chassis The LemLib chassis object
     * @param sensor The distance sensor to use
     * @param sensorOffset Distance from robot center to sensor face
     * @param wallX The X coordinate of the wall being measured
     * @param facingPositiveX True if sensor faces toward +X (right wall)
     * @return True if correction was applied successfully
     */
    bool correctX(lemlib::Chassis& chassis, pros::Distance& sensor,
                  double sensorOffset, double wallX, bool facingPositiveX);

    /**
     * Correct Y position using a distance sensor facing a wall
     * @param chassis The LemLib chassis object
     * @param sensor The distance sensor to use
     * @param sensorOffset Distance from robot center to sensor face
     * @param wallY The Y coordinate of the wall being measured
     * @param facingPositiveY True if sensor faces toward +Y (top wall)
     * @return True if correction was applied successfully
     */
    bool correctY(lemlib::Chassis& chassis, pros::Distance& sensor,
                  double sensorOffset, double wallY, bool facingPositiveY);

    /**
     * Correct both X and Y at a corner using two sensors
     * @param chassis The LemLib chassis object
     * @param xSensor Sensor measuring X (typically front sensor)
     * @param ySensor Sensor measuring Y (typically right or left sensor)
     * @param wallX The X coordinate of the wall (use Field:: constants)
     * @param wallY The Y coordinate of the wall (use Field:: constants)
     * @param facingPositiveX True if xSensor faces toward +X
     * @param facingPositiveY True if ySensor faces toward +Y
     * @return True if both corrections were applied successfully
     */
    bool correctAtCorner(lemlib::Chassis& chassis,
                         pros::Distance& xSensor, pros::Distance& ySensor,
                         double wallX, double wallY,
                         bool facingPositiveX, bool facingPositiveY);

    /**
     * Print sensor debug info to brain LCD
     * @param front Front distance sensor
     * @param right Right distance sensor
     * @param left Left distance sensor
     */
    void printSensorDebug(pros::Distance& front, pros::Distance& right,
                          pros::Distance& left);
}
