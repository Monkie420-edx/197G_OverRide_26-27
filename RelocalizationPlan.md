# 3-Sensor Distance Re-Localization Implementation Plan

## Overview

This document provides a step-by-step implementation plan for adding 3-sensor distance-based re-localization to Team 197G's robot. This technique corrects odometry drift during autonomous routines by using distance sensors to measure actual position relative to field walls.

**Sensor Configuration:**
- **Front Distance Sensor**: Mounted on intake side, facing forward → X-axis correction
- **Right Distance Sensor**: Mounted on right side, facing outward → Y-axis correction  
- **Left Distance Sensor**: Mounted on left side, facing outward → Y-axis correction (alternate)

**Goal:** Achieve sub-1-inch positioning accuracy at strategic checkpoints during autonomous.

---

## Current Codebase Analysis

### File Structure
```
src/
├── main.cpp           - Entry points (initialize, autonomous, opcontrol)
├── subsystems.cpp     - Hardware definitions (motors, sensors, chassis)
└── autos.cpp          - All autonomous routines

include/
├── subsystems.hpp     - Hardware declarations
├── autos.hpp          - Autonomous function declarations
└── main.h             - Main header
```

### Existing Hardware (subsystems.cpp)
- `ParkSensor` on Port 7 (distance sensor - can be repurposed as one of the 3 sensors)
- IMU on Port 1
- Vertical tracking wheel on Port 15
- Horizontal tracking wheel on Port 20

### Issue Found
The header file `subsystems.hpp` already declares:
```cpp
extern pros::v5::Distance FrontDistSensor;
extern pros::v5::Distance RightDistSensor;
extern pros::v5::Distance LeftDistSensor;
```
But `subsystems.cpp` does NOT define these sensors. This needs to be fixed.

---

## Implementation Steps

### Step 1: Update subsystems.cpp - Add Distance Sensor Definitions

**File:** `src/subsystems.cpp`

**Action:** Add the 3 distance sensor definitions. Replace the existing `ParkSensor` line with the new sensors.

**Find this line:**
```cpp
pros::v5::Distance ParkSensor(7);
```

**Replace with:**
```cpp
// Distance sensors for re-localization
pros::v5::Distance FrontDistSensor(8);   // Front (X correction) - Port 8
pros::v5::Distance RightDistSensor(9);   // Right (Y correction) - Port 9
pros::v5::Distance LeftDistSensor(10);   // Left  (Y correction) - Port 10

// ParkSensor removed - repurposed to the 3 distance sensors above
```

**Port Assignment:**
- Front Distance Sensor: **Port 8**
- Right Distance Sensor: **Port 9**
- Left Distance Sensor: **Port 10**

---

### Step 2: Create Relocalization Header File

**File:** `include/relocalization.hpp` (NEW FILE)

**Action:** Create this new header file with the following content:

```cpp
#pragma once
#include "lemlib/chassis/chassis.hpp"
#include "pros/distance.hpp"

namespace Relocalization {
    // Field wall positions (VEX field is 144" x 144", origin at center)
    namespace Field {
        constexpr double LEFT_WALL = -72.0;
        constexpr double RIGHT_WALL = 72.0;
        constexpr double TOP_WALL = 72.0;
        constexpr double BOTTOM_WALL = -72.0;
    }
    
    // Distance from robot CENTER to each sensor face
    namespace Offsets {
        constexpr double FRONT_SENSOR_X = 5.0;  // inches - robot center to front sensor face
        constexpr double RIGHT_SENSOR_Y = 4.0;  // inches - robot center to right sensor face
        constexpr double LEFT_SENSOR_Y = 4.0;   // inches - robot center to left sensor face
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
```

---

### Step 3: Create Relocalization Implementation File

**File:** `src/relocalization.cpp` (NEW FILE)

**Action:** Create this new source file with the following content:

```cpp
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
```

---

### Step 4: Update subsystems.hpp - Clean Up Declarations

**File:** `include/subsystems.hpp`

**Action:** The declarations for the 3 distance sensors already exist. Remove the `ParkSensor` declaration since it's being replaced.

**Remove this line:**
```cpp
extern pros::v5::Distance ParkSensor;
```

**Keep these lines (already present):**
```cpp
extern pros::v5::Distance FrontDistSensor;
extern pros::v5::Distance RightDistSensor;
extern pros::v5::Distance LeftDistSensor;
```

---

### Step 5: Add Debug Testing to main.cpp

**File:** `src/main.cpp`

**Action:** Add include and debug code for testing sensors during driver control.

**Add this include near the top (after other includes):**
```cpp
#include "relocalization.hpp"
```

**Add this inside the `opcontrol()` while loop (before the `pros::delay(20)`):**
```cpp
// Press Y to display distance sensor readings
if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y)) {
    Relocalization::printSensorDebug(FrontDistSensor, RightDistSensor, LeftDistSensor);
}
```

---

### Step 6: Integrate into Autonomous Routines

**File:** `src/autos.cpp`

**Action:** Add the include and insert correction calls at strategic checkpoints.

**Add this include at the top of the file:**
```cpp
#include "relocalization.hpp"
using namespace Relocalization;
```

**Example integration into Skills() routine:**

Find a location where the robot is near a corner and add a correction. For example, after exiting the first matchloader in Skills():

```cpp
// After: chassis.moveToPoint(28, 40, 1500, {.forwards = false});
// Before moving across field, correct position at top-left area

// Turn to face right (0°) for sensor alignment
chassis.turnToHeading(0, 500);
chassis.waitUntilDone();

// At 0°: Front faces +X, Right faces -Y (bottom)
// Near top-left: use LEFT_WALL for X, TOP_WALL for Y (but right sensor faces bottom, so use left sensor)
// This example assumes robot is near top-left corner
correctAtCorner(chassis, FrontDistSensor, LeftDistSensor,
                Field::LEFT_WALL,   // X wall (front sensor faces left at 180°, but we're at 0° so this won't work)
                Field::TOP_WALL,    // Y wall
                false,              // front faces +X (away from left wall at 0°)
                true);              // left sensor faces +Y (top wall) at 0°

// Continue with corrected position
chassis.moveToPoint(90, 35, 2000, {.forwards = false, .maxSpeed = 80});
```

**⚠️ IMPORTANT ORIENTATION REFERENCE:**

| Robot Heading | Front Faces | Right Faces | Left Faces | Best Sensor Combo |
|---------------|-------------|-------------|------------|-------------------|
| 0° (→ right)  | +X (right)  | -Y (bottom) | +Y (top)   | Front+Right for bottom-right |
| 90° (↑ up)    | +Y (top)    | +X (right)  | -X (left)  | Front+Right for top-right |
| 180° (← left) | -X (left)   | +Y (top)    | -Y (bottom)| Front+Left for top-left |
| 270° (↓ down) | -Y (bottom) | -X (left)   | +X (right) | Front+Left for bottom-left |

---

### Step 7: Verify Sensor Offsets

**File:** `include/relocalization.hpp`

**Action:** The sensor offsets are pre-configured with placeholder values. Verify these are reasonably close to your robot's actual measurements:

```cpp
namespace Offsets {
    constexpr double FRONT_SENSOR_X = 5.0;  // inches - robot center to front sensor face
    constexpr double RIGHT_SENSOR_Y = 4.0;  // inches - robot center to right sensor face
    constexpr double LEFT_SENSOR_Y = 4.0;   // inches - robot center to left sensor face
}
```

**Current Values:**
- `FRONT_SENSOR_X`: **5.0 inches**
- `RIGHT_SENSOR_Y`: **4.0 inches**
- `LEFT_SENSOR_Y`: **4.0 inches**

**If corrections are off by a consistent amount**, re-measure and update these values:
1. Find the robot's center point (usually center of drivetrain)
2. Measure perpendicular distance from center to each sensor's face (the emitter side)
3. Update the values in `include/relocalization.hpp`

---

## Testing Checklist

### Phase 1: Sensor Verification
- [ ] Upload code and enter driver control
- [ ] Press Y button to see sensor readings on brain LCD
- [ ] Move robot closer/farther from walls - values should change
- [ ] Verify readings are stable (±0.5" when stationary)
- [ ] Test each sensor individually near its target wall

### Phase 2: Single Axis Correction
- [ ] Test `correctX()` alone near a side wall
- [ ] Test `correctY()` alone near top/bottom wall
- [ ] Verify chassis.getPose() shows corrected values

### Phase 3: Corner Correction
- [ ] Test `correctAtCorner()` in each corner
- [ ] Verify both X and Y are corrected
- [ ] Check that correction doesn't cause erratic movement

### Phase 4: Autonomous Integration
- [ ] Run baseline: 5 Skills runs WITHOUT correction, note final positions
- [ ] Run with correction: 5 Skills runs WITH checkpoints, note final positions
- [ ] Compare consistency and accuracy
- [ ] Tune checkpoint locations and sensor offsets as needed

---

## Troubleshooting

| Problem | Likely Cause | Solution |
|---------|--------------|----------|
| Sensor returns -1 | Low confidence/bad angle | Check sensor mounting angle, clean lens |
| Correction worsens position | Wrong `facingPositive` parameter | Review orientation chart above |
| Values off by 6-7" | Offset not measured correctly | Re-measure from robot CENTER to sensor face |
| Works in testing, fails in match | Field variation | Add tolerance margins |
| Correction causes jerk | Large position jump | Add sanity check for max correction distance |

---

## File Summary

| File | Action | Description |
|------|--------|-------------|
| `src/subsystems.cpp` | MODIFY | Add 3 distance sensor definitions |
| `include/subsystems.hpp` | VERIFY | Declarations already exist |
| `include/relocalization.hpp` | CREATE | New header with namespace and functions |
| `src/relocalization.cpp` | CREATE | New implementation file |
| `src/main.cpp` | MODIFY | Add include and debug code |
| `src/autos.cpp` | MODIFY | Add include and checkpoint corrections |

---

## Quick Reference: Function Calls

```cpp
// Include at top of file
#include "relocalization.hpp"
using namespace Relocalization;

// Single axis correction
correctX(chassis, FrontDistSensor, Offsets::FRONT_SENSOR_X, Field::RIGHT_WALL, true);
correctY(chassis, RightDistSensor, Offsets::RIGHT_SENSOR_Y, Field::BOTTOM_WALL, false);

// Corner correction (most common)
correctAtCorner(chassis, FrontDistSensor, RightDistSensor,
                Field::RIGHT_WALL, Field::BOTTOM_WALL,
                true,   // front faces +X
                false); // right faces -Y

// Debug display
printSensorDebug(FrontDistSensor, RightDistSensor, LeftDistSensor);
```

---

## Notes for Claude Code Execution

1. **Port Numbers:** Front=8, Right=9, Left=10 (configured in Step 1)
2. **Sensor Offsets:** Front=5", Right=4", Left=4" (configured in Step 2)
3. **Build After Each Step:** Run `make` to verify no compilation errors
4. **Test Incrementally:** Don't add all checkpoints at once
5. **Keep Backup:** The autonomous routines work now - don't break them!
6. **Fine-tune Later:** If corrections are consistently off, measure actual offsets and update Step 7

---

*Document created for Team 197G - Harriton High School*
*VEX V5 VRC OverRide 2026-2027 Season*
