# Console-Diablo

A console-based Diablo-style game with Pokemon-inspired movement and combat mechanics.

## Features

- **Top-down view**: Navigate using WASD keys like in classic Pokemon games
- **Pokemon-style combat**: Screen flashes when entering battle encounters
- **Combat menu**: Choose from 3 options during battle:
  - Attack: Deal damage to enemies
  - Item: Use items (currently not implemented)
  - Run: Escape from battle

## Controls

### Overworld Movement
- `W` followed by Enter - Move up
- `S` followed by Enter - Move down
- `A` followed by Enter - Move left
- `D` followed by Enter - Move right
- `Q` followed by Enter - Quit game

### Combat
- `1` followed by Enter - Attack
- `2` followed by Enter - Item
- `3` followed by Enter - Run

## How to Play

1. Move around the map using W/A/S/D keys followed by Enter
2. Find monsters (M) to battle
3. When you encounter a monster, the screen will flash (Pokemon-style)
4. Choose your action from the combat menu
5. Defeat enemies or run away to continue exploring

## Building and Running

```bash
# Build the game
make

# Run the game
make run

# Or run directly
./console_diablo

# Clean build files
make clean
```

## Game Mechanics

- **Player**: Starts with 100 HP and 20 attack power
- **Enemies**: Demons with 50 HP and 15 attack power
- **Random encounters**: 5% chance of encountering an enemy while moving
- **Fixed enemies**: Some enemies are placed on the map and can be fought by walking into them

## Requirements

- C++11 compatible compiler (g++)
- Standard C++ library only (no external dependencies)
- Compatible with any system that supports standard C++