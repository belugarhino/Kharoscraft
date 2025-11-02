# Kharoscraft (C++ Edition)

Kharoscraft has been rebuilt from the ground up as a lightweight, performant native
application targeting Linux (and other SDL2-compatible platforms). The new
implementation focuses on responsive controls, deterministic world generation,
and a tight C++ gameplay loop suitable for future expansion.

## Features

- **Deterministic terrain** powered by a compact value-noise generator that
  synthesises grasslands, forests, coasts, and mountain ridges on the fly.
- **Smooth camera scrolling** with tile-based culling so large worlds remain
  responsive even on modest hardware.
- **Day/night lighting cycle** rendered entirely on the GPU via SDL2, with a
  HUD panel that visualises the current time of day.
- **Sprint-enabled movement** that respects terrain walkability, keeping the
  player constrained to traversable biomes.

## Getting started (Ubuntu 22.04+)

1. Install the build prerequisites and the SDL2 development headers:
   ```bash
   sudo apt update
   sudo apt install build-essential cmake libsdl2-dev
   ```
2. Configure the project with CMake:
   ```bash
   cmake -S . -B build
   ```
3. Compile the executable:
   ```bash
   cmake --build build
   ```
4. Launch the game:
   ```bash
   ./build/kharoscraft
   ```

The executable opens a resizable 1280×720 window. Use **WASD** to move, hold
**Shift** to sprint, and press **Esc** to exit.

## Project structure

```
CMakeLists.txt   # Build script for the kharoscraft executable
src/
  Game.cpp/.hpp  # SDL2 bootstrap, main loop, and orchestrator
  Renderer.*     # Tile culling, lighting overlay, and HUD drawing
  World.*        # Value-noise terrain generation and walkability checks
  Player.*       # Input-driven movement with sprinting and collision limits
  main.cpp       # Entry point that spins up the Game class
```

## License

This project remains available under the MIT License.
