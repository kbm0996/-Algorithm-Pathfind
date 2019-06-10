# JPS(JumpPointSearch) + Bresenham's Line
## 📢 개요

 A* 알고리즘이 한 그리드에서 다음 그리드와 같이 작은 단위로 고려했던 것과 달리 그리드의 직선(수평, 수직 및 대각선)을 따라 긴 점프를 고려하여 최단거리를 찾아낸다. 따라서 JPS는 A*에 비해 최단 거리를 찾는 데 필요한 노드 수를 획기적으로 줄일 수 있다.
 
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/GIF.gif)
  >figure 1. JPS(animated gif)

 
 ## 🅿 개념
 * A* 리뷰 그림, JPS 그림 출처 - https://zerowidth.com/2013/05/05/jump-point-search-explained.html
 
 ___
 
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
  
  >![2](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2.png)
  >
  >*figure 3. A** 대각선 탐색

 ___
  
 ### 강제 이웃(Forced Neighbor) 
 
  >![1b](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/1b.PNG)
  >
  >figure 4. A* 수직, 수평 탐색 중 장애물로 인해 이웃이 생긴 경우
  
  >![2b](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2b.PNG)
  >
  >figure 5. A* 대각선 탐색 중 장애물로 인해 이웃이 생긴 경우
  
 위 두 그림은 탐색 경로 상에 장애물이 있는 경우이다.
 
 길을 찾는 도중에 장애물에 막히면 당연히 막힌 노드에 멈춰서 다시 길찾기를 해야 한다. 즉, 원래 탐색해야 할 경로(4번의 경우 오른쪽 방향) 뿐만 아니라 새로 생긴 탐색 경로(4번의 경우 우측 대각선 위쪽 방향)로도 탐색을 진행해야한다. 이 과정에서 새로 생긴 탐색 경로의 탐색 주체가 되는 노드를 JPS 논문에서는 `강제 이웃(Forced Neighbor)`이라고 한다.
 
 JPS에서 탐색을 할 때 `강제 이웃`을 이용하는 것이 핵심 요소이다. 수평 및 수직 경로는 대각선 경로를 이동하기 전에 먼저 탐색한다. 수평 및 수직 경로 탐색 중 맵의 가장자리나 장애물을 만날 경우 장애물 끝으로 점프하면 `강제 이웃`을 감지하게 된다.
  
  ![2-6](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b6.PNG)
  ![2-7](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b7.PNG)
  ![2=8](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b8.PNG)
  
   수직과 수평 탐색을 먼저 한다. 두 탐색은 장애물(또는 지도의 가장자리)에 닿으면 끝난다. 그 다음 대각선 방향으로 탐색을 진행한다.
  
  ![2-9](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b9.PNG)
  
   수직 방향으로 탐색 범위를 확장해서 가장자리에 도달하는 동안 수평으로 점프하면 `강제 이웃`이 드러난다.
  
  ![2-10](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-b10.PNG)
  
   이 시점에서 **현재 노드**와 **탐색 경로 상의 `강제 이웃`과 인접한 노드**를 **열린 목록(Openlist)** 에 넣고 수평 탐색을 종료한다. 그리고 현재 노드의 탐색을 계속한다.
   
   
   이를 A* 알고리즘에서 열린 목록의 노드를 검사할 때마다 적용한다. 그러면 주변의 모든 노드를 검사했던 기존의 중간 단계를 모두 생략하고 새로운 탐색 노드들이 열린 목록에 들어가게 된다.
  
 ___
 
 ### JPS 예제
  
  ![2-1](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c1.PNG)
  
   위의 강제 노드 예시와 같은 지도이다.
  
  ![2-2](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c2.PNG)
  
   이전에 식별된 노드(열린 목록의 유일한 노드)에서 시작. 먼저, 수직 방향으로 탐색했으나 지도의 가장자리에 닿을 때까지 아무 경로도 찾지 못한다.
  
  ![2-3](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c3.PNG)
  
   수평 방향으로 탐색하여 `강제 노드`(보라색 노드)를 찾고 **현재 노드**와 **탐색 경로 상에 있던 `강제 노드`와 인접한 노드**를 **열린 목록**에 넣는다.
  
  ![2-4](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c4.PNG)
  
   대각선 방향으로 탐색했으나 지도의 가장자리에 부딪혀 아무 경로도 찾지 못한다.
  
  ![2-5](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c5.PNG)
  
   탐색이 끝난 노드는 열린 목록에서 빼고 **닫힌 목록(Closelist)** 에 넣는다. 그리고 열린 목록에 남아있는 다른 노드로 탐색을 시작한다. 먼저, 이 노드에 도달했을 때 수평 탐색으로 도달했으므로 수평 탐색을 먼저 한다. 하지만 지도의 가장자리에 부딪혀 아무 경로도 찾지 못한다.
  
  ![2-6](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c6.PNG)
  
   `강제 이웃`이 있던 방향으로 탐색을 확장하고, 대각선 점프 규칙에 따라 여태까지 그래왔던 것처럼 대각선으로 이동한 다음 수직 및 수평으로 탐색한다.
  
  ![2-7](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c7.PNG)
  
   아무것도 찾지 못했으므로, 대각선으로 다시 탐색한다.
  
  ![2=8](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c8.PNG)
  
   이제 수평(이제 갈 곳 없음) 및 수직으로 확장하면 목표 노드가 보인다. 강제 이웃을 발견한 것과 마찬가지로 현재 노드를 **열린 목록**에 넣는다.
  
  ![2-9](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c9.PNG)
  
   마지막으로 열린 목록에 목표를 넣는다.
  
  ![2-10](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/JumpPointSearch/jpg/2-c10.PNG)

   이제 각 노드의 부모를 따라가면 최적화된 길을 알아낼 수 있다.
