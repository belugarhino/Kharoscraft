# custom_player.py
from ursina import *
from ursina.prefabs.first_person_controller import FirstPersonController

class CustomPlayer(FirstPersonController):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.jump_speed = 15  # Initial upward speed
        self.gravity = 30  # Gravity strength
        self.max_jump_height = 2  # Maximum jump height
        self.is_jumping = False
        self.vertical_velocity = 0  # Tracks upward/downward speed

        # Adjust the player's collision box
        self.collider = BoxCollider(self, center=Vec3(0, 0.5, 0), size=Vec3(0.5, 1, 0.5))

    def update(self):
        super().update()  # Call the parent class's update method

        # Handle jumping
        if self.is_jumping:
            self.y += self.vertical_velocity * time.dt  # Move vertically
            self.vertical_velocity -= self.gravity * time.dt  # Apply gravity

            # Check for collisions with blocks below
            hit_info = self.intersects()
            if hit_info.hit:
                # Adjust the player's position to sit on top of the block
                self.y = hit_info.entity.y + hit_info.entity.scale_y / 2 + self.scale_y / 2
                self.is_jumping = False
                self.vertical_velocity = 0

            # Stop jumping if the player falls below the ground
            if self.y <= 0:
                self.y = 0
                self.is_jumping = False
                self.vertical_velocity = 0

    def input(self, key):
        super().input(key)  # Call the parent class's input method

        # Start jumping when spacebar is pressed and not already jumping
        if key == "space" and not self.is_jumping:
            # Check if the player is on the ground or a block
            hit_info = self.intersects()
            if hit_info.hit or self.y <= 0:
                self.is_jumping = True
                self.vertical_velocity = self.jump_speed