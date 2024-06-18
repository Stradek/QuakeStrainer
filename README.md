[![license](https://badgen.net/github/license/stradek/QuakeStrainer?color=blue)](https://github.com/Stradek/QuakeStrainer/blob/main/LICENSE)
[![commits](https://badgen.net/github/commits/stradek/QuakeStrainer?color=238636&icon=git)](https://github.com/Stradek/QuakeStrainer/commits/main)
[![last commit](https://badgen.net/github/last-commit/stradek/QuakeStrainer?color=238636&icon=github)](https://github.com/Stradek/QuakeStrainer/commits/main)
[![Build Status](https://badgen.net/github/checks/stradek/QuakeStrainer/main)](https://github.com/Stradek/QuakeSTrainer/actions/workflows/build-windows.yml)

# Quake Strainer

## Description
Quake Strainer is my reverse engineering project of classic Quake.
It is still work in progress and evolving rapidly. 
Right now it supports only unlimited ammo hack.

## Prerequisites:
- OS: Windows
- MSBuild
- CMake(for exact version look into CMakeLists.txt)

## Usage
1. Run `git clone --recursive https://github.com/Stradek/QuakeStrainer.git`
2. Run GenerateProjectVS2022.bat
3. Open QuakeStrainer.sln in Visual Studio
4. Build & Run QuakeStrainer project
5. Enjoy Quake!

## Features
- [x] Uses my implementation of low level utilities library
- [x] Uses my implementation of job scheduler for background tasks
- [x] Build upon my reusable API to interact with Quake game. QuakePatchingAPI features functions like:
  - [x] Find Quake process ID and attach to it with memory r/w access
  - [x] Ensure player is spawned
  - [x] Set unlimited ammo

## Goals
- Develop my own toolkit for modding games:
- Implement basic hacks like for: unlimitted ammo, god mode, all weapons unlocked etc
- Be able to use hacks by injecting DLL
- TBD: Create script for patching binary with my trainer:
  - Not decided yet if I want to patch code in memory or create game executable file patcher.
