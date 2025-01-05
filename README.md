# README.md

## Prerequisites

Before using this template, please ensure that the following requirements are met:

- macOS operating system.
- Xcode or a compatible C++ compiler installed.
- Basic knowledge of C++ programming language.
- Raylib installed

## Getting Started

To get started with this template, follow these steps:

1. Clone or download this repository to your local machine.
2. Open the project in your preferred C++ development environment
3. Build the project using the provided build script `make build` or by configuring the project settings in your development environment.
4. Run the compiled application with `make run`, and you should see a basic window with a sample scene.

Feel free to modify the sample code and project structure to suit your needs. You can start building your own application by adding new source files, assets, and implementing your desired functionality.

## Ideas

- [ ] **Basic User Interaction**: Add basic user input handling so that the player can move the character around. For instance, when the user presses the up arrow, the character could jump, while pressing the left or right arrow could make the character move in that direction.

- [ ] **Adding Enemies or Obstacles**: You could add enemies or obstacles that the character must avoid. This will require adding new Texture2D objects, similar to how you've added the character.

- [ ] **Collision Detection**: Implement a system for collision detection. For example, if your character comes into contact with an enemy or obstacle, some action should occur (like losing a life or ending the game).

- [ ] **Scoring System**: Add a way for players to earn points. This could be by collecting objects, defeating enemies, or simply surviving for a certain amount of time.

- [ ] **Sound Effects**: Your code already has some commented-out audio code. You could add sound effects for actions like jumping, collecting items, or losing a life.

- [ ] **Game States**: Implement different game states like Start Screen, Playing, Game Over, etc. This will give your game a more complete feel.

- [X] **Animations**: Improve the visual feedback of the game by adding animations to your character. This could be different animations for running, jumping, and any other actions your character can perform.

- [X] **Add Parallax Effect**: You're already doing some background scrolling, but you can experiment with different speeds for different layers to create a parallax effect. This can help create an illusion of depth in a 2D game.

- [ ] **Level Design**: Design different levels for your game, which could involve different backgrounds, obstacles, and enemies.
