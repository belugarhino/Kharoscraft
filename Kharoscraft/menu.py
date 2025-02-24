# menu.py
from ursina import *

def create_menu(start_game, exit_game):
    """Creates the main menu."""
    menu = Entity(enabled=True)

    # Background to fill the window
    menu_background = Entity(
        parent=menu,
        model='quad',
        texture='shore',  # Replace with your image (e.g., 'menu_bg.png')
        scale=(camera.aspect_ratio * 2, 2),  # Fill the window
        z=10,
        color=color.white  # Ensure the texture is visible
    )

    # Title
    title = Text(text="KHAROSCRAFT", y=0.2, scale=3, origin=(0, 0), parent=menu)

    # Buttons
    start_button = Button(text="Start", color=color.green, y=0, scale=(0.3, 0.1), parent=menu, on_click=start_game)
    exit_button = Button(text="Exit", color=color.red, y=-0.2, scale=(0.3, 0.1), parent=menu, on_click=exit_game)

    # Center buttons
    start_button.x = 0
    exit_button.x = 0

    return menu