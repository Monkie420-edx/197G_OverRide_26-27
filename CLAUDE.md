# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

> **Note**: This branch contains student-friendly code with a simplified structure. For advanced refactored code with modular architecture and touchscreen UI, see the `bot2` branch.

## Project Overview

This is a **VEX V5 robotics project** for the 2025-2026 season, built using:
- **PROS OS v4.2.1** - Real-time operating system for VEX V5
- **LemLib v0.5.6** - Motion control library for autonomous movement
- **liblvgl v9.2.0** - Graphics library for brain screen display
- **Target Platform**: VEX V5 Brain (ARM Cortex-A9, NEON FP16)

The robot is designed for competitive VEX Robotics matches with capabilities for:
- Autonomous routines (Skills, Qualifications, Eliminations)
- Color sorting intake system using optical sensor
- Pneumatic wings and match loader
- Odometry-based navigation with tracking wheels

## Build and Development Commands

### Building the Project
```bash
# Quick build (default, uses hot/cold linking)
make

# Full clean build
make all

# Clean build artifacts
make clean
```

**Build Output Locations:**
- Hot package: `bin/hot.package.bin` (user code - 12MB+)
- Cold package: `bin/cold.package.bin` (library code - cached)
- Monolith: `bin/monolith.bin` (fallback single binary)

### Compilation Details
- **C Standard**: GNU C2x (`gnu2x`)
- **C++ Standard**: GNU C++23 (`gnu++23`)
- **Compiler**: `arm-none-eabi-gcc` with Cortex-A9 optimizations
- **Key Flags**: `-Os -g -mthumb -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard`

### Hot/Cold Linking
This project uses PROS hot/cold linking (`USE_PACKAGE=1` in Makefile):
- **Cold image**: Pre-compiled libraries (PROS kernel, LemLib, liblvgl) - rarely changes
- **Hot image**: User code (src/*.cpp) - recompiles quickly
- Benefits: Faster iteration cycles, smaller uploads to robot brain

## Code Architecture

### File Structure
```
src/
├── main.cpp           - Entry points (initialize, autonomous, opcontrol)
├── subsystems.cpp     - Hardware definitions (motors, sensors, chassis)
└── autos.cpp          - All autonomous routines in one file

include/
├── main.h             - Main header declarations
├── subsystems.hpp     - Hardware declarations
├── autos.hpp          - Autonomous function declarations
├── lemlib/            - LemLib motion control library headers
└── pros/              - PROS OS API headers
```

### Key Hardware Configuration (subsystems.cpp)

**Drive System:**
- 6-motor drivetrain (3 left, 3 right) using 3.25" omni wheels
- Track width: 10 inches
- Gear ratio: 450 RPM
- Control: Arcade drive with expo curves (1.019 gain, 3 deadband)

**Odometry System:**
- IMU: Port 1 (inertial measurement unit)
- Vertical tracking wheel: Port 15 (reversed, 2.75" diameter, 0" offset)
- Horizontal tracking wheel: Port 20 (2.75" diameter, -5" offset)
- LemLib PID tuning:
  - Linear: kP=5, kD=60
  - Angular: kP=6.7, kD=65

**Intake/Scoring System:**
- TopRoller: Port 4
- TopIntake: Port -2 (reversed)
- BottomIntake: Port -5 (reversed)
- Optical sensor: Port 6 (color sorting)
- Distance sensor: Port 7 (parking detection)

**Pneumatics:**
- Wing: ADI port 2
- Loader: ADI port 1
- Park: ADI port 5

### Control Flow

**initialize()** (main.cpp:18)
- Initializes brain LCD
- Calibrates chassis sensors (IMU, tracking wheels)
- Spawns background task for telemetry display

**autonomous()** (main.cpp:76)
- Calls the selected autonomous routine
- Currently configured to run `TopGoalQUALS()`
- To change routines: Edit line 81 in [main.cpp:81](src/main.cpp#L81)
- All autonomous functions are defined in [autos.cpp](src/autos.cpp)

**opcontrol()** (main.cpp:105)
- Arcade drive: Left Y + Right X
- **L1**: Run intake forward (all rollers)
- **L2**: Outtake (reverse intake)
- **R1**: Top roller reverse
- **B**: Toggle wing pneumatic
- **A**: Toggle loader pneumatic
- **X**: Toggle park pneumatic
- **UP/DOWN**: Fine control for bottom intake

### Switching Autonomous Routines

To change which autonomous routine runs in competition:
1. Open [main.cpp](src/main.cpp)
2. Find the `autonomous()` function (around line 76)
3. Change the function call to your desired routine:
   ```cpp
   void autonomous() {
     TopGoalQUALS();  // <-- Change this line
   }
   ```
4. Rebuild and upload: `make && pros upload`

### Understanding the Code

**Learning-Friendly Design:**
- All autonomous code lives in [autos.cpp](src/autos.cpp) - one file to learn!
- Numbers like `127` mean full motor speed (range: -127 to 127)
- Delays like `pros::delay(300)` are in milliseconds (300ms = 0.3 seconds)
- Coordinates are in inches from starting position
- Direct, visible values make it easy to understand and tune

**Alliance Color** (main.cpp:100):
- `AllianceColor teamColor = AllianceColor::RED;` - Change to `BLUE` if needed
- This enum exists for future color sorting implementation

### Autonomous Routines

All autonomous routines are defined in [autos.cpp](src/autos.cpp). Available routines:

**TopGoalQUALS()** (autos.cpp:9) - Top/High side qualification match
- Picks up center field blocks
- Scores in match loader and alliance goal
- ~15 second routine

**BottomGoalQUALS()** (autos.cpp:88) - Bottom/Low side qualification match
- Mirror of TopGoalQUALS for the other side of the field
- Match loader → Long goal → Center goal
- ~15 second routine

**ElimsHigh()** (autos.cpp:147) - High side elimination match
- Focuses on center field blocks
- Scores in alliance goal
- Wing deployment for blocking
- ~15 second routine

**ElimsLow()** (autos.cpp:184) - Low side elimination match
- Mirror of ElimsHigh for the other side
- Center blocks + alliance goal scoring
- ~15 second routine

**Skills()** (autos.cpp:224) - Programming skills run
- Full 60-second autonomous routine
- Scores in 3 long goals using match loaders
- Navigates entire field

**noAuto()** (autos.cpp:140) - Minimal autonomous
- Moves forward slightly (5 inches)
- Use if other team has solo AWP

**antiJam()** (autos.cpp:216) - Helper function
- Reverses intake to clear jams
- Called during scoring sequences

### LemLib Chassis API Usage
Common patterns in autonomous code:
```cpp
chassis.setPose(x, y, heading);                    // Set odometry position
chassis.moveToPoint(x, y, timeout);               // Drive to field coordinates
chassis.turnToHeading(angle, timeout);            // Turn to absolute heading
chassis.swingToHeading(angle, side, timeout);     // Swing turn (one side stationary)
chassis.waitUntilDone();                          // Block until motion complete
```

**Motion Options:**
- `{.forwards = false}` - Drive backwards
- `{.maxSpeed = 80}` - Limit speed (0-127)
- `{.minSpeed = 30}` - Minimum speed for motion


## Critical Implementation Notes

### When Adding New Autonomous Routines
1. Declare function in [include/autons.hpp](include/autos.hpp)
   ```cpp
   void MyNewAuton();
   ```
2. Implement in [src/autos.cpp](src/autos.cpp)
   ```cpp
   void MyNewAuton() {
     chassis.setPose(0, 0, 0);  // Always set initial pose
     // Your autonomous code here...
   }
   ```
3. Call your new function in [main.cpp autonomous()](src/main.cpp#L76):
   ```cpp
   void autonomous() {
     MyNewAuton();  // <-- Call your new routine
   }
   ```
4. Test odometry calibration before competition

**To modify existing routines:** Just edit the function in [autos.cpp](src/autos.cpp) and rebuild!

### When Modifying Hardware Configuration
1. Motor/sensor definitions live in `src/subsystems.cpp`
2. Declarations in `include/subsystems.hpp`
3. Update port numbers carefully (wrong ports = motor smoke)
4. Reversed motors use negative port numbers (e.g., `-2`)
5. After changes, recalibrate tracking wheels and IMU

### PID Tuning Guidelines
- Linear PID (linearController in subsystems.cpp:51): Affects straight-line driving smoothness
- Angular PID (angularController in subsystems.cpp:63): Affects turning precision
- Increase kP for faster response, increase kD for less oscillation
- Test on actual field with fully charged battery

### Common Pitfalls
- **Odometry Drift**: Ensure tracking wheel encoders are securely mounted and not slipping
- **Timeout Too Short**: Motions that timeout early leave chassis in unpredictable state
- **Battery Voltage**: Low battery significantly affects autonomous performance
- **Motor Temperature**: Overheated motors trigger thermal protection (reduce load/speed)

## Field Coordinate System
LemLib uses field-centric coordinates:
- Origin (0, 0): Starting position (set by setPose)
- Positive X: Right
- Positive Y: Forward
- Heading: Degrees (0° = forward, 90° = right, 180° = back, 270° = left)
- Field dimensions: 144" x 144" (VEX V5 standard)

## Debugging Tools

**Brain LCD Display** (initialize() task):
- Line 0: X position
- Line 1: Y position
- Line 2: Theta (heading)

**PROS Terminal** (via USB):
```bash
pros terminal
```
- View printf/cout output
- Monitor task states
- Check for errors/warnings

**Motor Temperature Check:**
```cpp
motor.get_temperature();  // Returns °C
```

## Library-Specific Notes

### LemLib Motion Control
- Documentation: https://lemlib.readthedocs.io/
- Chassis must be calibrated before use (`chassis.calibrate()`)
- Blocking motions use timeouts (milliseconds)
- Non-blocking: add `{.async = true}` to motion options

### PROS API Patterns
- Task creation: `pros::Task taskName([]() { ... });`
- Delays: `pros::delay(ms)` (yields to RTOS scheduler)
- Controller: `pros::Controller controller(pros::E_CONTROLLER_MASTER);`
- Motor control: `.move(voltage)` [-127 to 127] or `.move_velocity(rpm)`

## Competition Checklist
Before matches, verify:
1. Correct autonomous routine called in [main.cpp:81](src/main.cpp#L81)
   - Edit the function call to match your strategy (TopGoalQUALS, ElimsHigh, Skills, etc.)
2. `teamColor` set correctly in [main.cpp:100](src/main.cpp#L100) (RED or BLUE)
3. All pneumatics start retracted (false/0)
4. Tracking wheels spin freely without binding
5. IMU calibrated (don't move robot during calibration)
6. Battery fully charged (>7.2V under load)
7. Code rebuilt and uploaded: `make && pros upload`
