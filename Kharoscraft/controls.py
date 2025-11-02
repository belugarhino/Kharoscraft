# controls.py
"""Utility helpers for handling player controls and pause mechanics."""

from __future__ import annotations

from ursina import Entity, Text, camera, color, mouse


def setup_controls(menu, player):
    """Configure key bindings and an on-screen HUD for the player."""

    help_text = Text(
        text="WASD to move • Space to jump • ESC to pause",
        position=(-0.5 * camera.aspect_ratio, 0.45),
        origin=(-0.5, 0.5),
        background=True,
        color=color.rgb(120, 180, 255),
        alpha=0.7,
        enabled=False,
    )

    crosshair = Text(text="+", scale=2, origin=(0, 0), color=color.rgba(255, 255, 255, 220), enabled=False)

    def set_paused(paused: bool) -> None:
        mouse.locked = not paused
        menu.enabled = paused
        player.enabled = not paused
        help_text.enabled = not paused
        crosshair.enabled = not paused

    def toggle_pause():
        """Toggle the pause menu and cursor lock state."""

        set_paused(not menu.enabled)

    def input(key):
        """Handle key presses."""

        if key == "escape":
            toggle_pause()

    controller = Entity(ignore_paused=True)
    controller.input = input

    # Expose the helpers so other modules can control the pause state.
    controller.toggle_pause = toggle_pause  # type: ignore[attr-defined]
    controller.set_paused = set_paused  # type: ignore[attr-defined]
    controller.help_text = help_text  # type: ignore[attr-defined]
    controller.crosshair = crosshair  # type: ignore[attr-defined]

    return controller
