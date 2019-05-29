# 길찾기 알고리즘
## 📢 개요
 길 찾기는 애플리케이션에서 두 지점 사이의 최단 경로를 찾는 알고리즘이다.
  
## 📑 구성
  자세한 내용은 하위 디렉토리 참조
  
### 💻 PaintFill
  인접한 배열들을 검사하여 `0`을 `*`로 바꾸기
  
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/PaintFill/GIF.gif)
  
  *figure 1. PaintFill(animated gif)*
  
### 💻 A*
   A* 알고리즘은 주어진 출발점에서부터 목표점까지 가는 최단 경로를 찾아내는 경로 탐색 알고리즘이다. 각 꼭짓점 x에 대해 그 꼭짓점을 통과하는 최상의 경로를 추정하는 순위값인 휴리스틱(Heuristic) 추정값 h(x) 을 매기는 방법을 쓴다.
  
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/AStar/AGIF.gif)
  
  *figure 2. A*(animated gif)*
  
### 💻 JPS(JumpPointSearch)
  A* 알고리즘이 한 그리드에서 다음 그리드와 같이 작은 단위로 고려했던 것과 달리 그리드의 직선(수평, 수직 및 대각선)을 따라 긴 점프를 고려하여 최단거리를 찾아낸다. 따라서 JPS는 A*의 원리를 유지하면서 실행 시간을 획기적으로 줄일 수 있다.

## 📌 참고
 
https://qiao.github.io/PathFinding.js/visual/
