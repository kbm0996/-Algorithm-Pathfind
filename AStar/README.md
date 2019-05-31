# A*
## 📢 개요

 A* 알고리즘은 주어진 출발지점에서부터 목적지점까지 가는 최단 경로를 찾아내는 경로 탐색 알고리즘이다. 각 꼭짓점 x에 대해 그 꼭짓점을 통과하는 최상의 경로를 추정하는 순위값인 휴리스틱(Heuristic) 추정값 h(x) 을 매기는 방법을 쓴다.
  
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/AStar/AGIF.gif)
  >*figure 1. A*(animated gif)*

 
 ## 🅿 개념
 출처 -  https://egloos.zum.com/cozycoz/v/9748811
 
 ### 1. 범위 찾기(The Search Area)
   녹색은 출발지점A, 빨간색은 도착지점B, 그리고 파란색으로 채워진 사각형은 양쪽사이에 있는 벽입니다.

  >![capture](http://pds11.egloos.com/pds/200905/25/42/a0118142_4a1a3cd65c3d0.jpg)
  >*figure 2. Step 1*

 ### 2. 노드 제거

  ![](https://s3.ap-northeast-2.amazonaws.com/opentutorials-user-file/module/1335/2975.png)

 **figure 14. Phase 7*

  삭제가 완료 되었습니다.
 
 
 ## 📌 참고
 
 'A* Pathfinding for Beginner By Patrick Lester’ 
 
 https://deliorange.tistory.com/110
