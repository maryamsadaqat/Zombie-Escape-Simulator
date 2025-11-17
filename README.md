# **Zombie Apocalypse: A Pathfinding And Survival Simulation**

### **PROJECT OVERVIEW**

### **1. Project Description**
Our project is a 2D survival strategy game built using C++ and Raylib.  
The player spawns in a randomly generated maze filled with zombies, exits, and helpful items.  
The goal is to safely reach an exit while avoiding zombies, using weapons, medkits, and hint tiles.  
The project demonstrates the use of several important data structures, pathfinding algorithms, and game development concepts.



### **2. Project Functionalities**

#### **1. Maze Generation**
- A new maze is generated each game.  
- Prim’s algorithm forms random paths.  
- Walls and empty tiles are placed automatically.  

#### **2. Player Movement**
- Player moves using arrow keys.  
- Player walks on empty tiles only.  
- Player cannot pass through walls.  

#### **3. Zombies**
- Zombies move toward the player.  
- They follow shortest-path logic.  
- Zombies multiply through infection.  

#### **4. Weapons**
- Player can pick up weapons.  
- Weapons are stored in a stack.  
- Weapons kill zombies in a blast area.  

#### **5. Medkits**
- Player can pick up medkits.  
- Medkits increase player health.  
- Medkits use a deque for storage.  

#### **6. Hints**
- Hint tiles guide the player.  
- Shortest hint shows shortest path.  
- Safest hint shows safest path.  

#### **7. Pathfinding**
- BFS finds shortest routes.  
- Dijkstra finds safest routes.  
- Paths are drawn on the grid.  

#### **8. Scoring**
- Score increases with each move.  
- Best score is saved to a file.  
- Game shows current and best scores.  

#### **9. Win Condition**
- Player wins by reaching an exit.  

#### **10. Lose Condition**
- Player loses if HP becomes zero.  
- Player loses if a zombie catches them.  

#### **11. User Interface**
- Grid is displayed on screen.  
- HUD shows player info.  
- Inventory and health are always visible.
  


### **3. System Architecture**

#### **Main Components**

| Component | Description |
|----------|-------------|
| **Game State** | Stores entire maze, player, zombies, items |
| **Maze Generator** | Creates random maze using Prim’s algorithm |
| **Pathfinding Engine** | BFS and Dijkstra for hints and zombie movement |
| **Zombie System** | Controls chasing and infection |
| **Player System** | Movement, pickups, actions |
| **Rendering System** | Draws grid, HUD, effects |
| **Score System** | Saves and loads high score |



### **4. Data Structures Used**
- **Vector (vector):** Stores grid, zombies, exits, FX, and path preview.  
- **Queue (queue):** Used for BFS shortest-path search.  
- **Stack (stack):** Used for weapon inventory (LIFO grenades).  
- **Deque (deque):** Used for medkit inventory (FIFO usage).  
- **Priority Queue (priority_queue):** Used in Dijkstra’s safest path algorithm.  
- **Random Engine (mt19937):** Used for maze and item randomness.
- 


### **5. Algorithms Used**

#### **5.1 Maze Generation by Prim’s Algorithm**
- Creates a connected maze with randomized carving.  

#### **5.2 BFS for Shortest Path**
- Used for player hint (shortest path) and zombie chasing.  

#### **5.3 Dijkstra’s Algorithm for Safest Path**
- Finds a path that avoids zombies using danger weights.  

#### **5.4 Infection Spread**
- Zombies generate new zombies around them every few turns.  

#### **5.5 Player Inventory Logic**
- Weapons use LIFO (stack)  
- Medkits use FIFO (deque)
- 


### **6. Function Descriptions**
- **GenerateMaze():** Creates a maze using Prim’s algorithm.  
- **PlaceStuff():** Places zombies, exits, items, and the player.  
- **BFS_PlayerToExit():** Finds shortest path to nearest exit.  
- **Dijkstra_SafestPath():** Finds safest path to exit based on danger cost.  
- **ZombieNextStepTowardPlayer():** Calculates zombie movement using BFS.  
- **UseWeapon():** Uses grenade weapon to kill zombies in area.  
- **UseMedkit():** Restores HP by removing medkit from deque.  
- **ResolveTurn():** Processes one full turn (zombie movement, infection, hints, score, win/loss).  
- **DrawGrid() / DrawHUD():** Displays maze, player, zombies, inventory, and UI.  
- **FinalizeScore():** Saves best score to file.
- 


### **7. Conclusion**
This project demonstrates a complete and functional 2D survival game built using C++ and Raylib.  
It integrates essential concepts from Data Structures, Algorithms, and Game Development, including:  
- Procedural maze generation  
- Pathfinding (BFS, Dijkstra)  
- Stack and deque inventory systems  
- Intelligent zombie AI  
- Real-time simulation  
- Persistent scoring  
