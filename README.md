[![Build Windows](https://github.com/Stradek/QuakeSTrainer/actions/workflows/build-windows.yml/badge.svg)](https://github.com/Stradek/QuakeSTrainer/actions/workflows/build-windows.yml)
# Quake Strainer

## Description
Quake Strainer is my reverse engineering project of classic Quake.
It is still work in progress and evolving rapidly. 

## Features
- [x] Uses my own implementation of low level utilities library
- [x] Uses my own implementation of job scheduler for background tasks
- [x] Build upon my own one reusable API to interact with Quake game which features functions like:
  - [x] Find quake process ID
  - [x] Check if player is spawned
  - [x] Set unlimited ammo

## Goals
- Develop my own toolkit for modding games:
- Implement basic hacks like for: unlimitted ammo, god mode, all weapons unlocked etc
- Be able to use hacks by injecting DLL
- Create script for patching binary with my trainer:
  - Not decided yet if I want to patch code in memory or patch game executable file
