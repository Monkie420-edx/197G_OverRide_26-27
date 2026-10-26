# 197G OverRide - VEX V5 Robotics

**Team 197G's competition code for the 2026-2027 VEX V5 "OverRide" season**

This repository is the active season repo for Team 197G. It was migrated from the previous PushBack codebase and kept as a reference in the `upstream` remote.

## Repository status

- New GitHub repo: `Monkie420-edx/197G_OverRide_26-27`
- Previous season repo kept as `upstream` for reference and comparison
- Main branch is the current working codebase for this season

## Current focus

This repo is intended to be the working season codebase for the 2026-2027 OverRide game. Team members can use the old PushBack code as a reference, but new work should be built around the OverRide robot strategy and current hardware setup.

## Quick start

### Prerequisites
- PROS CLI installed
- VEX V5 Brain connected via USB
- Fully charged battery

### Build and upload
```bash
make
pros upload
# or
make upload
```

### Debugging
```bash
pros terminal
```

## Robot overview

This project includes:
- 6-motor drivetrain with odometry
- intake and scoring system
- pneumatic mechanisms
- autonomous routines for qualification, elimination, and skills matches
- LemLib-based chassis motion control

## Project structure

```text
src/
├── main.cpp        - initialize, autonomous, opcontrol
├── subsystems.cpp  - motor and sensor configuration
├── autos.cpp       - autonomous routines
├── relocalization.cpp

include/
├── autos.hpp
├── main.h
├── relocalization.hpp
├── subsystems.hpp
├── pros/
├── lemlib/

README.md
Makefile
project.pros
```

## Autonomous workflow

To change which autonomous routine runs, edit the function call in `src/main.cpp` inside `autonomous()`.

Example:
```cpp
void autonomous() {
    FourPush();
}
```

Then rebuild and upload:
```bash
make && pros upload
```

## Notes

- This repo keeps the simplified student-friendly code style.
- Use the old PushBack repo as a reference if needed via `upstream`.
- For future advanced refactors, work can be split into separate branches or feature-focused work.

## Team information

- Team: 197G
- Game: OverRide
- Season: 2026-2027
