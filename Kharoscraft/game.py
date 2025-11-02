# game.py
"""Entry point that wires together the menu, player and world generation."""

from __future__ import annotations

import math
from dataclasses import dataclass
from typing import Optional

from ursina import (
    AmbientLight,
    DirectionalLight,
    Entity,
    Sky,
    Ursina,
    Vec3,
    application,
    color,
    mouse,
    time,
)

from controls import setup_controls
from custom_player import CustomPlayer
from menu import create_menu
from world import World, generate_world


app = Ursina()


@dataclass
class GameState:
    world: Optional[World] = None
    player: Optional[CustomPlayer] = None
    controls = None
    cycle = None


state = GameState()


class DayNightCycle(Entity):
    """Animate the lighting for a simple day/night effect."""

    def __init__(self) -> None:
        super().__init__()
        self.time_of_day = 0.0
        self.speed = 0.03
        self.sky = Sky(texture="sky_sunset")
        self.ambient = AmbientLight(color=color.rgb(100, 100, 130))
        self.sun = DirectionalLight(color=color.rgb(255, 245, 220), shadows=True)
        self.sun.look_at(Vec3(1, -1, -0.5))

    def update(self) -> None:  # noqa: D401 - standard ursina update hook
        phase = (self.time_of_day + time.dt * self.speed) % 1
        self.time_of_day = phase

        light_strength = max(0.15, math.sin(phase * math.tau) * 0.5 + 0.5)
        night_factor = 1 - light_strength

        self.sky.color = color.rgb(40 + 160 * light_strength, 60 + 120 * light_strength, 120 + 80 * light_strength)
        self.ambient.color = color.rgb(60 + 120 * light_strength, 60 + 120 * light_strength, 90 + 120 * light_strength)

        sun_y = math.sin(phase * math.tau)
        sun_x = math.cos(phase * math.tau)
        self.sun.look_at(Vec3(-sun_x, -sun_y, -0.3))
        self.sun.color = color.rgb(255, 240 - 80 * night_factor, 200 - 110 * night_factor)


def _parse_seed(seed_text: str) -> Optional[int]:
    seed_text = seed_text.strip()
    if not seed_text:
        return None

    try:
        return int(seed_text)
    except ValueError:
        return abs(hash(seed_text)) % (2**31)


def start_game(seed_text: str = "", size: int = 20) -> None:
    """Spawn the player, world and HUD."""

    if state.world:
        state.world.cleanup()

    menu.enabled = False
    mouse.locked = True

    seed = _parse_seed(seed_text)
    state.world = generate_world(size=size, seed=seed)

    if not state.player:
        state.player = CustomPlayer(y=6, speed=6)
    else:
        state.player.position = Vec3(0, 6, 0)
        state.player.enable()

    if not state.controls:
        state.controls = setup_controls(menu, state.player)

    state.controls.set_paused(False)

    if not state.cycle:
        state.cycle = DayNightCycle()
    else:
        state.cycle.enable()


def exit_game() -> None:
    if state.world:
        state.world.cleanup()
    application.quit()


menu = create_menu(start_game, exit_game)


app.run()
