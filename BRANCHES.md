# Branch Structure Guide

## Overview

This repository maintains two parallel codebases for different use cases. The branches are **intentionally different** and should **never be merged**.

---

## Branches

### `main` (Student Code)

**Purpose:** Learning platform for students new to VEX programming

**Structure:** Simplified file organization
```
src/
├── main.cpp           - Entry points
├── subsystems.cpp     - Hardware config
└── autos.cpp          - All autonomous code in ONE file (356 lines)

include/
├── main.h, subsystems.hpp, autos.hpp
└── Library headers (pros/, lemlib/, liblvgl/, fmt/)
```

**Characteristics:**
- Direct inline values (`RegFull.move(127)` instead of `Motors::FULL_FORWARD`)
- All autonomous routines in single file for easy navigation
- Manual autonomous selection (edit `main.cpp` line 81)
- Simple `antiJam()` helper function
- No abstraction layers - see exactly what's happening

**Best for:**
- First-time VEX programmers
- Students learning C++ and robotics
- Quick prototyping and testing
- Understanding robot behavior step-by-step

**Robot:** Primary competition robot (student-driven development)

---

### `bot2` (Advanced Code)

**Purpose:** Production code for 2nd competition robot

**Structure:** Modular architecture with design patterns
```
src/
├── main.cpp
├── subsystems.cpp
├── autonomous/
│   ├── auton_helpers.cpp      - Reusable utilities
│   ├── auton_selector.cpp     - Touchscreen UI (singleton)
│   ├── match_autons.cpp       - Match routines
│   └── skills_auton.cpp       - Skills routine
└── utils/
    └── constants.cpp          - Namespace constants

include/
├── [corresponding headers]
├── autonomous/ & utils/ directories
└── Library headers
```

**Characteristics:**
- Namespace-based constants (`Motors::`, `Timing::`, `ColorSensor::`)
- Singleton pattern for autonomous selector
- Touchscreen UI for autonomous selection (no recompilation!)
- Coordinate mirroring utilities for field symmetry
- Helper functions namespace (`AutonHelpers`)
- Color sorting logic with optical sensor voting
- Professional code organization

**Best for:**
- Experienced programmers
- Scalable team projects
- Production-ready competition code
- Advanced algorithm implementation

**Robot:** Secondary competition robot

---

## Switching Branches

```bash
# View all branches
git branch -a

# Switch to student code
git checkout main

# Switch to bot2 code
git checkout bot2

# Check current branch
git branch
```

---

## Making Changes

### Development Workflow

- **Changes to student code:** Make on `main` branch
- **Changes to bot2 code:** Make on `bot2` branch
- **DO NOT** merge between branches - they are intentionally different

### Synchronizing Autonomous Logic

If both robots need the same autonomous routine:
1. Test the routine on `main` first (simpler to debug)
2. Manually copy/adapt the code to `bot2`
3. Update constants and abstractions as needed
4. Test separately on each branch

**Never merge:** The structures are too different for automatic merging

---

## When to Use Each Branch

| Scenario | Branch |
|----------|--------|
| Teaching students VEX basics | `main` |
| Student-driven development | `main` |
| Learning C++ and robotics concepts | `main` |
| Quick prototyping | `main` |
| Building 2nd competition robot | `bot2` |
| Testing advanced algorithms | `bot2` |
| Production-ready team code | `bot2` |
| Touchscreen autonomous selection | `bot2` |
| Color sorting implementation | `bot2` |

---

## Code Comparison Examples

### Autonomous Selection

**Student Code (main):**
```cpp
// In main.cpp autonomous() function
void autonomous() {
  TopGoalQUALS();  // <-- Manually edit this line
}
```

**Bot2 Code:**
```cpp
// Touchscreen UI - no code changes needed
void autonomous() {
  AutonSelector::getInstance().executeSelected();
}
```

### Motor Control

**Student Code (main):**
```cpp
RegFull.move(127);          // Full speed forward
pros::delay(300);           // Wait 300ms
TopRoller.move(-100);       // Reverse at speed 100
```

**Bot2 Code:**
```cpp
RegFull.move(Motors::FULL_FORWARD);
pros::delay(Timing::ANTI_JAM_REVERSE_MS);
TopRoller.move(Motors::REJECT_SPEED);
```

### Anti-Jam Logic

**Student Code (main):**
```cpp
void antiJam() {
  RegFull.move(-127);
  TopRoller.move(127);
  pros::delay(300);
  RegFull.move(127);
  pros::delay(2750);
}
```

**Bot2 Code:**
```cpp
// In AutonHelpers namespace
antiJam(RegFull, Timing::ANTI_JAM_REVERSE_MS);
// Overloaded for different motors and behaviors
```

---

## Future Development Patterns

### Student Learning Path
1. Students learn on `main` (simple, direct code)
2. Understand robot behavior and VEX concepts
3. Graduate to contributing on `bot2` (professional patterns)

### Feature Development
1. New autonomous strategies tested on `main` first
   - Simpler to debug
   - Faster iteration
2. Proven strategies ported to `bot2` with proper abstraction
   - Add constants
   - Update autonomous selector UI
   - Implement coordinate mirroring if needed

---

## Repository Guidelines

### Branch Isolation
- `main` and `bot2` evolve independently
- No merge commits should exist between them
- They represent different philosophies, not different versions

### Documentation
- `README.md`: Quick start and branch guide
- `CLAUDE.md`: Detailed technical documentation (differs by branch)
- `BRANCHES.md`: This file (same on both branches)

### Commit Messages
- Clearly indicate which robot/branch the changes affect
- Use descriptive commit messages for autonomous routine updates

---

## Questions?

See:
- [README.md](README.md) for quick start guide
- [CLAUDE.md](CLAUDE.md) for detailed technical documentation (branch-specific)
- GitHub Issues for bug reports or feature requests
