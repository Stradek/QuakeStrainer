[![Build Windows](https://github.com/Stradek/QuakeSTrainer/actions/workflows/build-windows.yml/badge.svg)](https://github.com/Stradek/QuakeSTrainer/actions/workflows/build-windows.yml)
# Quake Strainer

## Description
Quake Strainer is my reverse engineering project of classic Quake.
It is still work in progress and evolving rapidly. 

## Features
- Uses my own implementation of low level utilities library
- Uses my own implementation of job scheduler for background tasks
- Build upon my own one reusable API to interact with Quake game which features functions like:
  - Find quake process ID
  - Check if player is spawned
  - Set unlimited ammo

## Status
- [WIP] Develop my own toolkit for modding games:
  - It's in progress. I called it "PatchingUtils". It's separate from Quake game API class, but used across Quake game API.
- [WIP] Implement basic hacks like for: unlimitted ammo, god mode, all weapons unlocked etc:
  - [DONE] unlimitted ammo
  - [TO-DO] god mode
  - [TO-DO] all weapons unlocked
- [TODO] Be able to use hacks by injecting DLL
- [TBD] Create script for patching binary with my trainer:
  - Not decided yet if I want to patch code in memory or patch game executable file
