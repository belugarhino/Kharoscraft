# game.py
from ursina import *
from ursina.prefabs.first_person_controller import FirstPersonController
from controls import setup_controls  # Import controls
from world import generate_world  # Import world generation

app = Ursina()

def start_game():
    print("Starting the game...")
    menu.disable()
    mouse.locked = True  # Lock the mouse to the window
    
    # Generate world
    generate_world()
    
    # Add player and link to controls
    player = FirstPersonController(y=2, speed=5)
    setup_controls(menu, player)  # Pass menu and player to controls

def exit_game():
    application.quit()

# Menu UI
menu = Entity(enabled=True)
title = Text(text="KHAROSCRAFT", y=0.3, scale=3, origin=(0, 0), parent=menu)
start_button = Button(text="Start", color=color.green, y=0.1, scale=(0.2, 0.1), parent=menu, on_click=start_game)
exit_button = Button(text="Exit", color=color.red, y=-0.3, scale=(0.2, 0.1), parent=menu, on_click=exit_game)
start_button.x = 0
exit_button.x = 0
menu_background = Entity(parent=menu, model='quad', texture='shore', scale=(2, 2), z=10)

app.run()