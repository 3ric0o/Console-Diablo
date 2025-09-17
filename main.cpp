#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

// Game constants
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;
const char PLAYER_CHAR = '@';
const char EMPTY_CHAR = '.';
const char ENEMY_CHAR = 'M';

// Game states
enum GameState {
    OVERWORLD,
    COMBAT
};

// Combat menu options
enum CombatOption {
    ATTACK = 0,
    ITEM = 1,
    RUN = 2
};

class Player {
public:
    int x, y;
    int health;
    int maxHealth;
    int attack;
    
    Player() : x(MAP_WIDTH/2), y(MAP_HEIGHT/2), health(100), maxHealth(100), attack(20) {}
    
    void move(char direction) {
        int newX = x, newY = y;
        
        switch(direction) {
            case 'w': case 'W': newY--; break;  // Up
            case 's': case 'S': newY++; break;  // Down
            case 'a': case 'A': newX--; break;  // Left
            case 'd': case 'D': newX++; break;  // Right
        }
        
        // Check boundaries
        if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT) {
            x = newX;
            y = newY;
        }
    }
};

class Enemy {
public:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    
    Enemy(const std::string& n) : name(n), health(50), maxHealth(50), attack(15) {}
};

class Game {
private:
    Player player;
    GameState currentState;
    std::vector<std::vector<char>> map;
    std::mt19937 rng;
    
    // Cross-platform getch function
    char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
            perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
        return (buf);
    }
    
    void initializeMap() {
        map.resize(MAP_HEIGHT, std::vector<char>(MAP_WIDTH, EMPTY_CHAR));
        
        // Place some enemies randomly
        std::uniform_int_distribution<int> xDist(0, MAP_WIDTH - 1);
        std::uniform_int_distribution<int> yDist(0, MAP_HEIGHT - 1);
        
        for (int i = 0; i < 5; i++) {
            int x, y;
            do {
                x = xDist(rng);
                y = yDist(rng);
            } while ((x == player.x && y == player.y) || map[y][x] != EMPTY_CHAR);
            
            map[y][x] = ENEMY_CHAR;
        }
    }
    
    void clearScreen() {
        system("clear");
    }
    
    void flashScreen() {
        // Pokemon-style screen flash effect
        for (int i = 0; i < 3; i++) {
            // Flash effect using ANSI escape codes
            std::cout << "\033[41m"; // Red background
            std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top
            std::cout << "*** ENCOUNTER! ***";
            std::cout << "\033[0m"; // Reset colors
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            
            std::cout << "\033[2J\033[H"; // Clear screen
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
    
    void drawOverworld() {
        clearScreen();
        
        std::cout << "=== CONSOLE DIABLO ===\n";
        std::cout << "Health: " << player.health << "/" << player.maxHealth << "\n\n";
        
        // Draw the map
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (x == player.x && y == player.y) {
                    std::cout << PLAYER_CHAR;
                } else {
                    std::cout << map[y][x];
                }
            }
            std::cout << "\n";
        }
        
        std::cout << "\nControls: WASD to move, Q to quit\n";
        std::cout << "Find monsters (M) to battle!\n";
    }
    
    int showCombatMenu() {
        std::cout << "\n=== COMBAT MENU ===\n";
        std::cout << "1. Attack\n";
        std::cout << "2. Item\n";
        std::cout << "3. Run\n";
        std::cout << "Choose your action (1-3): ";
        
        char choice;
        while (true) {
            choice = getch();
            if (choice >= '1' && choice <= '3') {
                std::cout << choice << "\n";
                return choice - '1'; // Convert to 0-based index
            }
        }
    }
    
    void combatEncounter() {
        Enemy enemy("Demon");
        
        // Flash screen when entering combat
        flashScreen();
        
        clearScreen();
        std::cout << "A wild " << enemy.name << " appears!\n\n";
        
        while (enemy.health > 0 && player.health > 0) {
            // Display status
            std::cout << "Player Health: " << player.health << "/" << player.maxHealth << "\n";
            std::cout << enemy.name << " Health: " << enemy.health << "/" << enemy.maxHealth << "\n";
            
            // Show combat menu
            int choice = showCombatMenu();
            
            switch (choice) {
                case ATTACK: {
                    std::cout << "You attack the " << enemy.name << "!\n";
                    enemy.health -= player.attack;
                    
                    if (enemy.health <= 0) {
                        std::cout << "You defeated the " << enemy.name << "!\n";
                        std::cout << "Press any key to continue...\n";
                        getch();
                        return;
                    }
                    
                    // Enemy attacks back
                    std::cout << enemy.name << " attacks you!\n";
                    player.health -= enemy.attack;
                    
                    if (player.health <= 0) {
                        std::cout << "You have been defeated!\n";
                        std::cout << "Game Over! Press any key to exit...\n";
                        getch();
                        exit(0);
                    }
                    break;
                }
                case ITEM:
                    std::cout << "You don't have any items!\n";
                    break;
                case RUN:
                    std::cout << "You ran away safely!\n";
                    std::cout << "Press any key to continue...\n";
                    getch();
                    return;
            }
            
            std::cout << "Press any key to continue...\n";
            getch();
            clearScreen();
        }
    }
    
    bool checkForEncounter() {
        // Check if player stepped on an enemy
        if (map[player.y][player.x] == ENEMY_CHAR) {
            map[player.y][player.x] = EMPTY_CHAR; // Remove enemy from map
            return true;
        }
        
        // Random encounter chance (5%)
        std::uniform_int_distribution<int> encounterChance(1, 100);
        return encounterChance(rng) <= 5;
    }
    
public:
    Game() : currentState(OVERWORLD), rng(std::random_device{}()) {
        initializeMap();
    }
    
    void run() {
        while (true) {
            if (currentState == OVERWORLD) {
                drawOverworld();
                
                char input = getch();
                
                if (input == 'q' || input == 'Q') {
                    break;
                }
                
                player.move(input);
                
                // Check for combat encounter
                if (checkForEncounter()) {
                    currentState = COMBAT;
                    combatEncounter();
                    currentState = OVERWORLD;
                }
            }
        }
        
        std::cout << "Thanks for playing Console Diablo!\n";
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}