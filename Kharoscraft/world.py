# world.py
"""Procedural world generation helpers."""

from __future__ import annotations

import random
from dataclasses import dataclass, field
from typing import List, Tuple

from noise import pnoise2
from ursina import Entity, color, destroy


@dataclass
class World:
    """Simple container that tracks blocks spawned in the scene."""

    blocks: List[Entity] = field(default_factory=list)

    def cleanup(self) -> None:
        """Destroy every block that was previously generated."""

        for block in self.blocks:
            if not block or block.is_destroyed:
                continue
            block.disable()  # Hide immediately to avoid one more frame of flicker.
            destroy(block)
        self.blocks.clear()


def _spawn_block(position: Tuple[int, int, int], texture: str, tint: color.Color) -> Entity:
    """Create a single voxel and return the associated entity."""

    return Entity(
        model="cube",
        texture=texture,
        collider="box",
        color=tint,
        position=position,
    )


def _maybe_spawn_tree(world: World, base_position: Tuple[int, int, int], rng: random.Random) -> None:
    """Spawn a small tree made from cubes with a random chance."""

    trunk_height = rng.randint(2, 3)
    trunk_color = color.rgb(139, 96, 53)
    leaves_color = color.rgb(46, 154, 23)

    x, y, z = base_position
    for i in range(trunk_height):
        world.blocks.append(_spawn_block((x, y + i, z), "white_cube", trunk_color))

    # Create a 3x3 blob of leaves centred above the trunk.
    top_y = y + trunk_height
    for dx in range(-1, 2):
        for dy in range(2):
            for dz in range(-1, 2):
                world.blocks.append(
                    _spawn_block((x + dx, top_y + dy, z + dz), "white_cube", leaves_color)
                )


def generate_world(
    size: int = 20,
    max_height: int = 4,
    seed: int | None = None,
    tree_chance: float = 0.07,
    frequency: float = 0.12,
) -> World:
    """Generate a chunk of terrain with gentle height variation.

    Parameters
    ----------
    size:
        Width and depth of the square chunk. The generated area will be centred around
        the origin.
    max_height:
        Maximum height (in blocks) added above the base layer.
    seed:
        Optional random seed that allows players to recreate worlds.
    tree_chance:
        Probability of spawning a tree on a column.
    frequency:
        Frequency of the Perlin noise used for height variation.
    """

    rng = random.Random(seed)
    base = rng.randint(0, 10_000)

    world = World()

    half = size // 2
    for x in range(-half, half):
        for z in range(-half, half):
            height_value = pnoise2(x * frequency, z * frequency, octaves=3, base=base)
            column_height = 1 + int(((height_value + 1) / 2) * max_height)

            # Build a small column of voxels. The top block is grassy while the rest is dirt.
            for y in range(column_height):
                texture = "grass" if y == column_height - 1 else "white_cube"
                tint = color.rgb(130, 100, 70) if texture == "white_cube" else color.rgb(90, 200, 90)
                world.blocks.append(_spawn_block((x, y, z), texture, tint))

            if rng.random() < tree_chance and column_height >= 2:
                _maybe_spawn_tree(world, (x, column_height, z), rng)

    return world
