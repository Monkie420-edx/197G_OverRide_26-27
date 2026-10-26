# 197G OverRide Season Checklist

This document is the working checklist for the 2026-2027 OverRide season.

## Repository status

- Main repo: `Monkie420-edx/197G_OverRide_26-27`
- Previous season repo kept as `upstream` for historical reference
- Work for this season should happen in this repo unless otherwise noted

## Before first testing session

- [ ] Confirm robot is on the correct branch (`main`)
- [ ] Verify drivetrain port mapping in `src/subsystems.cpp`
- [ ] Confirm IMU and tracking wheels are calibrated
- [ ] Check intake and shooter motors match the current robot setup
- [ ] Verify pneumatics are wired to the intended ADI ports
- [ ] Confirm controller mappings in `src/main.cpp`

## Before competition

- [ ] Pick the correct autonomous routine in `src/main.cpp`
- [ ] Verify alliance color selection is correct
- [ ] Test all autonomous routines on flat field
- [ ] Confirm battery voltage is healthy before match
- [ ] Check all wiring and zip ties before transport
- [ ] Upload the current build and verify the brain recognizes it

## Recommended workflow

1. Keep a clean branch for season work
2. Test small changes individually
3. Commit stable code after field testing
4. Push to GitHub when the build is verified
5. Keep the old season repo as historical reference only

## Useful commands

```bash
# status
git status

# create a branch
git checkout -b season/<feature-name>

# commit changes
git add .
git commit -m "Describe the change"

# push
git push origin HEAD
```
