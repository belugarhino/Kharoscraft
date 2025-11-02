# menu.py
"""UI elements for the game's main menu."""

from __future__ import annotations

from ursina import Button, Entity, InputField, Slider, Text, camera, color


def create_menu(start_game, exit_game):
    """Create the title screen with options for configuring the world."""

    menu = Entity(enabled=True)

    Entity(
        parent=menu,
        model="quad",
        texture="shore",
        scale=(camera.aspect_ratio * 2, 2),
        z=10,
        color=color.white,
    )

    Text(text="KHAROSCRAFT", y=0.3, scale=3, origin=(0, 0), parent=menu)

    Text(text="Seed (optional):", y=0.08, parent=menu)
    seed_field = InputField(parent=menu, default_value="", y=0.0, limit_content_to="0-9A-Za-z")

    Text(text="World size", y=-0.12, parent=menu)
    size_slider = Slider(min=12, max=36, default=20, dynamic=True, step=2, y=-0.18, parent=menu)
    size_value = Text(text=f"{int(size_slider.value)} x {int(size_slider.value)}", y=-0.25, parent=menu)

    def _update_size_value():
        value = int(size_slider.value)
        size_value.text = f"{value} x {value}"

    size_slider.on_value_changed = lambda: _update_size_value()

    def _start():
        start_game(seed_field.text, int(size_slider.value))

    Button(text="Start", color=color.green, y=-0.38, scale=(0.3, 0.1), parent=menu, on_click=_start)
    Button(text="Exit", color=color.red, y=-0.52, scale=(0.3, 0.1), parent=menu, on_click=exit_game)

    menu.seed_field = seed_field  # type: ignore[attr-defined]
    menu.size_slider = size_slider  # type: ignore[attr-defined]

    return menu
