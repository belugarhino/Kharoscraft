# Kharoscraft

Kharoscraft is a Linux-focused 3D voxel sandbox inspired by classic block-building games.  
The engine is written in modern C++20, uses SDL2 for platform abstraction, and drives a fully programmable OpenGL 4 core renderer tailored for dense voxel scenes.

## Features
- Chunked voxel world with configurable dimensions and deterministic noise-based terrain generation.
- Mesh builder with face culling to keep GPU workloads efficient.
- Free-flying spectator camera with configurable mouse sensitivity and smooth damping.
- Extensible renderer architecture with shader hot-loading and centralized resource lifetime management.

## Getting started (Ubuntu 22.04+)
```bash
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libgl1-mesa-dev libglu1-mesa-dev

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/kharoscraft
```

## Controls
- **W / A / S / D** – Move relative to the camera.
- **Space / Left Ctrl** – Ascend / descend.
- **Shift** – Boost speed.
- **Mouse** – Look around (relative mode).
- **F5** – Reload shaders from disk.
- **Esc** – Release the mouse and quit.

## Project layout
```
assets/            Runtime shader files.
include/           Public headers grouped by subsystem.
src/               Engine implementation files.
```

## License
This repository is distributed under the MIT License. See [`LICENSE`](LICENSE) for details.
