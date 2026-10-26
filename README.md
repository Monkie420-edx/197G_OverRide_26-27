# 197G Push Back - VEX V5 Robotics

**Team 197G's competition code for the 2025-2026 VEX V5 "Push Back" season**

This repository contains autonomous routines and driver control for our Push Back robot. Built with PROS OS and LemLib motion control.

## 📁 Branch Guide

This repository contains two versions of the robot code:

- **`main` branch** (you are here): **Student-friendly simple code**
  - Beginner-friendly structure
  - All autonomous code in one file ([autos.cpp](src/autos.cpp))
  - Direct inline values (easy to understand and tune)
  - Good for learning VEX programming

- **`bot2` branch**: **Advanced refactored code** for 2nd robot
  - Modular file structure with design patterns
  - Touchscreen autonomous selector UI
  - Namespace-based constants
  - See bot2 branch README for details

**To switch branches:** `git checkout bot2` or `git checkout main`

---

## 🎯 Quick Start

### Prerequisites
- [PROS CLI](https://pros.cs.purdue.edu/v5/getting-started/) installed
- VEX V5 Brain connected via USB
- Fully charged battery

### Build and Upload
```bash
# Build the project
make

# Upload to robot brain
pros upload

# Build + upload in one command
pros make upload

# View terminal output (for debugging)
pros terminal
```

---

## 🤖 Robot Overview

**Key Features:**
- 6-motor drivetrain with odometry tracking
- Dual-intake system
- Pneumatic wings, loader, and parking mechanism
- 7 autonomous routines: TopGoalQUALS, BottomGoalQUALS, ElimsHigh, ElimsLow, Skills, noAuto, antiJam

**Control Scheme:**
- **Left Stick Y** + **Right Stick X**: Arcade drive
- **L1**: Intake forward (all rollers)
- **L2**: Outtake (reverse intake)
- **R1**: Top roller reverse
- **B**: Toggle wing
- **A**: Toggle loader
- **X**: Toggle parking mechanism
- **UP/DOWN**: Fine control for bottom intake

---

## 🔄 Changing Autonomous Routines

### Before Competition

To switch which autonomous routine runs:

1. **Open** [src/main.cpp](src/main.cpp) in your code editor
2. **Find** the `autonomous()` function (line 76)
3. **Edit** the function call:
   ```cpp
   void autonomous() {
     TopGoalQUALS();  // <-- Change this line
   }
   ```
4. **Available routines:**
   - `TopGoalQUALS()` - Top/High side qualification match
   - `BottomGoalQUALS()` - Bottom/Low side qualification match
   - `ElimsHigh()` - High side elimination strategy
   - `ElimsLow()` - Low side elimination strategy
   - `Skills()` - Programming skills run (~60 seconds)
   - `noAuto()` - Minimal movement (if other team has solo AWP)

5. **Rebuild and upload:**
   ```bash
   make && pros upload
   ```

**Note:** Requires recompilation to change routines. See `bot2` branch for touchscreen selector.

---

## 🏗️ Project Structure

```
src/
├── main.cpp           - Entry points (initialize, autonomous, opcontrol) - 206 lines
├── subsystems.cpp     - Hardware config (motors, sensors, pneumatics) - 97 lines
└── autos.cpp          - ALL autonomous routines in one file - 356 lines

include/
├── main.h             - Main header declarations
├── subsystems.hpp     - Hardware declarations
├── autos.hpp          - Autonomous function declarations
├── pros/              - PROS OS library headers
└── lemlib/            - LemLib motion control headers
```

**Simple by design!** All autonomous code in one file ([autos.cpp](src/autos.cpp)) makes it easy to learn and understand.

---

## ⚙️ Hardware Configuration

**Motors:**
- Ports 11, 12, 13: Left drivetrain (450 RPM)
- Ports -18, -19, -20: Right drivetrain (450 RPM, reversed)
- Port 4: Top roller
- Port -2: Top intake (reversed)
- Port -5: Bottom intake (reversed)

**Sensors:**
- Port 1: IMU (inertial measurement unit)
- Port 6: Optical sensor (color sorting)
- Port 7: Distance sensor (parking detection)
- Port 15: Vertical tracking wheel (2.75" diameter, reversed)
- Port 20: Horizontal tracking wheel (2.75" diameter)

**Pneumatics (ADI):**
- Port A (1): Loader
- Port B (2): Wing
- Port E (5): Park

**Chassis Configuration:**
- Track width: 10 inches
- Wheel diameter: 3.25 inches (omni wheels)
- Gear ratio: 450 RPM

---

## 🎮 Autonomous Routines

All routines defined in [src/autos.cpp](src/autos.cpp):

### TopGoalQUALS() / BottomGoalQUALS()
**Strategy:** Center field blocks → Match loader → Alliance goal
**Duration:** ~15 seconds
**Field Side:** Top (high/left) or Bottom (low/right)
**Code:** Each side has its own routine with mirrored coordinates

### ElimsHigh() / ElimsLow()
**Strategy:** Center field blocks → Alliance goal with wing deployment
**Duration:** ~15 seconds
**Code:** Separate implementations for each side

### Skills()
**Strategy:** 3 long goals using match loaders + field navigation
**Duration:** ~60 seconds
**Code:** Full autonomous skills run

### noAuto()
**Use:** Minimal movement if other team has solo AWP
**Code:** Simple 5-inch forward movement

### antiJam()
**Use:** Helper function to clear intake jams
**Called by:** Other autonomous routines during scoring

---

## 🔧 Development Guide

### Adding a New Autonomous Routine

1. **Declare function** in [include/autos.hpp](include/autos.hpp):
   ```cpp
   void MyNewRoutine();
   ```

2. **Implement** in [src/autos.cpp](src/autos.cpp):
   ```cpp
   void MyNewRoutine() {
       chassis.setPose(0, 0, 0);  // Always set initial pose

       // Your autonomous movements
       RegFull.move(127);  // Start intake
       chassis.moveToPoint(10, 20, 1000);  // Drive to position
       pros::delay(500);  // Wait for blocks
       // ... more code
   }
   ```

3. **Call it** in [src/main.cpp](src/main.cpp) line 76:
   ```cpp
   void autonomous() {
       MyNewRoutine();  // <-- Call your new routine
   }
   ```

4. **Rebuild:** `make && pros upload`

### Modifying Existing Routines

Open [src/autos.cpp](src/autos.cpp) and edit the routine directly. All autonomous code is in this one file!

### Understanding the Code

**Direct values make learning easy:**
```cpp
RegFull.move(127);              // Full speed forward (127 is max)
pros::delay(300);               // Wait 300 milliseconds
chassis.moveToPoint(30, 2, 3250); // X=30", Y=2", timeout=3250ms
TopRoller.move(-100);           // Reverse at speed 100
```

**What the numbers mean:**
- Motor speeds: `-127` to `127` (negative = reverse)
- Delays: milliseconds (`300` = 0.3 seconds)
- Coordinates: inches from starting position
- Timeouts: milliseconds before motion gives up

### PID Tuning

Edit `src/subsystems.cpp`:
```cpp
// Linear PID (straight driving)
lemlib::ControllerSettings linearController(
    5,    // kP - increase for faster response
    0,    // kI - usually keep at 0
    60,   // kD - increase to reduce oscillation
    0, 0, false
);

// Angular PID (turning)
lemlib::ControllerSettings angularController(
    6.7,  // kP
    0,    // kI
    65,   // kD
    0, 0, false
);
```

Test on fully charged battery with actual field conditions!

---

## 🧪 Testing Checklist

### Before Competition:
- [ ] Test autonomous routine on practice field
- [ ] Verify correct routine is called in [main.cpp:81](src/main.cpp#L81)
- [ ] IMU calibrates successfully (don't move robot during calibration!)
- [ ] Odometry tracking stable (no wheel slippage)
- [ ] Pneumatics start retracted (Wing, Loader, Park all false)
- [ ] Driver control fully functional
- [ ] Code compiled and uploaded: `make && pros upload`
- [ ] Battery fully charged (>7.2V under load)

### Match-Day Routine:
1. **Before match:** Upload correct autonomous routine
   - Edit [main.cpp:81](src/main.cpp#L81) to call desired routine
   - `make && pros upload`
2. **At field:** Power on robot
3. **Wait** for IMU calibration (~3 seconds)
4. **Check** brain screen shows X/Y/Theta position
5. **Place** robot at starting position
6. **Wait** for field control to enable autonomous

---

## 🛠️ Troubleshooting

### Build Errors

**"arm-none-eabi-g++: command not found"**
- Install PROS toolchain: `pros c install`
- Verify: `arm-none-eabi-g++ --version`

**"No such file or directory" for headers**
- Run `pros c fetch` to download dependencies (LemLib, liblvgl)
- Check `project.pros` has correct kernel version

### Robot Behavior

**Odometry drifting during autonomous**
- Check tracking wheel encoders are secure
- Ensure wheels spin freely without binding
- Verify wheel diameter in `subsystems.cpp` matches actual wheels
- Recalibrate on competition field (different surface friction)

**IMU "Calibrating..." never completes**
- Robot must be stationary during calibration
- IMU port 1 connection secure
- Try power cycle

**Wrong autonomous routine runs**
- Check [main.cpp:81](src/main.cpp#L81) calls the correct function
- Verify code was rebuilt after editing: `make && pros upload`
- Check upload completed successfully (no errors)

**Autonomous routine stops early**
- Check for timeout values that are too short
- Verify battery is fully charged (low battery = slow movements)
- Test movements individually to find which one fails

**Motors overheating**
- Reduce maxSpeed in autonomous movements
- Add delays between intensive operations
- Check for mechanical binding/friction
- Verify motor cartridges are correct RPM (450 recommended)

---

## 📚 Additional Resources

- **PROS Documentation**: https://pros.cs.purdue.edu/
- **LemLib Docs**: https://lemlib.readthedocs.io/
- **VEX Push Back Manual**: https://www.vexrobotics.com/push-back-manual
- **VEX Forum**: https://www.vexforum.com/

---

## 🏆 Competition Tips

1. **Always have a backup brain** with working code uploaded
2. **Test autonomous on actual field** - carpet/tiles behave differently
3. **Charge batteries overnight** before competition day
4. **Bring micro-USB cable** for emergency code uploads
5. **Practice driver control** - autonomous only lasts 15 seconds!
6. **Document field-specific tuning** - every field is slightly different
7. **Use Skills routine** as safe fallback if new routines are untested

---

## 👥 Team 197G

**Season:** 2025-2026 VEX V5 "Push Back"
**Robot Name:** [Your robot name]
**Organization:** [Your school/organization]

### Contributors
- **Programmers:** [Team member names]
- **Builders:** [Team member names]
- **Drivers:** [Team member names]

---

## 📄 License

This project is for educational purposes as part of the VEX Robotics Competition.

---

## 🙏 Acknowledgments

- **PROS Team** - Real-time operating system
- **LemLib** - Motion control and odometry
- **VEX Robotics** - Competition platform
- **Claude Code** - Development assistance

---

**Last Updated:** January 2026
**Code Version:** Student-Friendly (Main Branch)

*Good luck at competition! 🎉*

> **Want advanced features?** Check out the `bot2` branch for touchscreen selector, modular structure, and more!
