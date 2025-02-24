# controls.py
from ursina import *

def setup_controls(menu, player):
    """Handles game controls and pause functionality."""
    
    def toggle_pause():
        """Toggles mouse lock and pause menu."""
        mouse.locked = not mouse.locked
        menu.enabled = not menu.enabled
        player.enabled = not menu.enabled  # Disable player movement when paused
    
    def input(key):
        """Handles key presses."""
        if key == 'escape':
            toggle_pause()
    
    # Create an invisible entity to handle inputs globally
    controller = Entity(ignore_paused=True)
    controller.input = input  # Attach the input function