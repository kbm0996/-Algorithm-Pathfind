# JPS(JumpPointSearch) + Bresenham's Line
## 📢 개요

 A* 알고리즘이 한 그리드에서 다음 그리드와 같이 작은 단위로 고려했던 것과 달리 그리드의 직선(수평, 수직 및 대각선)을 따라 긴 점프를 고려하여 최단거리를 찾아낸다. 따라서 JPS는 A*에 비해 최단 거리를 찾는 데 필요한 노드 수를 획기적으로 줄일 수 있다.
 
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/GIF.gif)
  >*figure 1. JPS(animated gif)

 
 ## 🅿 개념
 
 원본 출처 - https://zerowidth.com/2013/05/05/jump-point-search-explained.html
 
 ### 경로 확장과 대칭 줄이기 (Path Expansion and Symmetry Reduction)
 * 녹색은 `출발지점A`, 빨간색은 `도착지점B`, 그리고 검은색은 `벽`을 의미
  
  먼저, 위와 같이 탐색 지역을 2차원 배열로 단순화한다. 배열의 각 항목은 사각형이며 사각형은 이동 가능 또는 이동 불가능으로 나뉘어진다.
  
  ![1](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/1.png)
  
  ![2](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2.png)
  
  ![2b](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2b.PNG)
  
  
  
  ![2-6](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b6.PNG)
  
  ![2-7](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b7.PNG)
  
  ![2=8](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b8.PNG)
  
  ![2-9](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b9.PNG)
  
  ![2-10](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b10.PNG)
  
  
  
  ![2-1](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c1.PNG)
  
  ![2-2](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c2.PNG)
  
  ![2-3](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c3.PNG)
  
  ![2-4](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c4.PNG)
  
  ![2-5](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c5.PNG)
  
  ![2-6](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c6.PNG)
  
  ![2-7](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c7.PNG)
  
  ![2=8](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c8.PNG)
  
  ![2-9](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c9.PNG)
  
  ![2-10](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c10.PNG)


