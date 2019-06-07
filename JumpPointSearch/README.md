# JPS(JumpPointSearch) + Bresenham's Line
## 📢 개요

 A* 알고리즘이 한 그리드에서 다음 그리드와 같이 작은 단위로 고려했던 것과 달리 그리드의 직선(수평, 수직 및 대각선)을 따라 긴 점프를 고려하여 최단거리를 찾아낸다. 따라서 JPS는 A*에 비해 최단 거리를 찾는 데 필요한 노드 수를 획기적으로 줄일 수 있다.
 
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/GIF.gif)
  >*figure 1. JPS(animated gif)

 
 ## 🅿 개념
 * A* 리뷰 그림, JPS 그림 출처 - https://zerowidth.com/2013/05/05/jump-point-search-explained.html
 * 강제 이웃 설명 및 그림 출처 - https://joonleestudio.tistory.com/28
 
 ### A* 리뷰
 
 A* 알고리즘은 단순하게 열린 목록(Openlist)에 도착지점의 그리드가 들어올 때까지 아래와 같은 단계를 밟으면서 경로를 찾는다.
 
  >1. **열린 목록(Openlist)** 에서 가장 작은 F 비용을 가지고 있는 그리드를 빼고 **닫힌 목록(Closelist)** 에 넣는다.
  >
  >2. 선택한 그리드에 인접한 그리드들을 확인한다(**닫힌 목록(Closelist)** 에 있거나 벽은 무시). 인접한 그리드가 **열린 목록(Openlist)** 에 없다면 추가한다. 그리고 현재 그리드를 새로운 그리드의 부모로 지정한다. 
  >
  >3. 인접한 그리드가 이미 **열린 목록(Openlist)** 에 있다면 해당 그리드의 비용이 더 좋은지 확인한다. 즉, 선택한 그리드와 비교하여 어느 그리드의 G 비용이 더 낮은지 확인한다. 그리고 인접한 그리드들의 부모를 G 비용이 낮게 검증된 그리드로 바꾼다. 
 
  * 녹색은 `출발지점A`, 빨간색은 `도착지점B`, 그리고 검은색은 `벽`을 의미
  
  >![1](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/1.png)
  >
  >*figure 2. A** 수직, 수평 탐색
  
  >![1b](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/1b.PNG)
  >
  >*figure 3. A** 수직, 수평 탐색 + 장애물
  
  >![2](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2.png)
  >
  >*figure 4. A** 대각선 탐색
  
  >![2b](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2b.PNG)
  >
  >*figure 5. A** 대각선 탐색 + 장애물
  
  
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


