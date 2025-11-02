# custom_player.py
"""Extended player controller used by the main game."""

from __future__ import annotations

from ursina import BoxCollider, Vec3, color, held_keys
from ursina.prefabs.first_person_controller import FirstPersonController


class CustomPlayer(FirstPersonController):
    """Adds a sprint toggle and improved collision box."""

    def __init__(self, **kwargs):
        kwargs.setdefault("model", "cube")
        kwargs.setdefault("color", color.rgb(220, 210, 200))
        super().__init__(**kwargs)

        self.base_speed = self.speed
        self.sprint_multiplier = 1.6
        self.collider = BoxCollider(self, center=Vec3(0, 0.9, 0), size=Vec3(0.6, 1.8, 0.6))

    def update(self):  # noqa: D401 - Ursina lifecycle method
        self.speed = self.base_speed * (self.sprint_multiplier if held_keys.get("shift") else 1)
        super().update()
