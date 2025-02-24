# world.py
from ursina import *

def generate_world():
    """Generates the game world."""
    for x in range(8):
        for z in range(8):
            Entity(
                model='cube',
                color=color.green,
                position=(x, 0, z),
                collider='box',
                texture='grass'
            )