# **Zombie Apocalypse: A Survival Maze Game**

## **Overview**
This project is a 2D zombie escape and survival game developed in **C++** using the **Raylib** graphics library.  
The game generates a **random maze** at each run, where the player must avoid zombies, collect items, and reach one of the exit points.  
It demonstrates essential concepts from **Data Structures and Algorithms (DSA)** through gameplay mechanics and system design.

---

## **Objectives**
- Implement a grid-based maze environment using algorithmic generation  
- Apply classical pathfinding techniques for navigation and enemy logic  
- Integrate DSA concepts (stack, queue, deque, priority queue) in real gameplay  
- Build an interactive turn-based survival simulation  
- Practice modular programming and structured game architecture  

---

## **Key Features**
- Random maze generation using algorithmic carving  
- Player movement with collision rules  
- Zombie AI that moves intelligently and spreads infection  
- **Weapons system (Stack)**  
- **Medkits system (Deque)**  
- Hint tiles that show suggested paths  
- Survival mechanics (HP, infection, danger)  
- Scoring system with best-score tracking  
- Complete UI/HUD built with Raylib  

---

## **Data Structures Used**
- **Vector:** Grid, zombies, paths, exits  
- **Queue:** Breadth-first exploration  
- **Stack:** Weapon (grenade) inventory  
- **Deque:** Medkit inventory (FIFO)  
- **Priority Queue:** Safe-path calculation  
- **mt19937 Engine:** Randomized maze and zombie placement  

---

## **Algorithms Implemented**
- **Prim’s Algorithm** – procedural maze generation  
- **BFS** – shortest path and enemy navigation  
- **Cost-based Pathfinding** – safer path selection  
- **Infection Algorithm** – zombie replication logic  

---

## **Game Architecture**
- Maze Generator  
- Player System  
- Zombie System  
- Pathfinding Engine  
- Rendering/UI System  
- Score System  
- Main Game Loop  

---

## **Conclusion**
This project successfully combines game development with core **Data Structures and Algorithmic techniques**.  
It demonstrates procedural generation, pathfinding logic, inventory management, and survival mechanics in a functional C++ application using Raylib.  
It highlights how theoretical DSA concepts can be applied directly in interactive simulations and real-time problem solving.
