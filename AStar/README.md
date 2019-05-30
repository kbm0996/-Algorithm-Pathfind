# A*
## 📢 개요

 A* 알고리즘은 주어진 출발지점에서부터 목적지점까지 가는 최단 경로를 찾아내는 경로 탐색 알고리즘이다. 각 꼭짓점 x에 대해 그 꼭짓점을 통과하는 최상의 경로를 추정하는 순위값인 휴리스틱(Heuristic) 추정값 h(x) 을 매기는 방법을 쓴다.
  
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/AStar/AGIF.gif)
  >*figure 1. A*(animated gif)*

 
 ## 🅿 원리
 ### 1. 노드 추가
   단순 연결 리스트와 거의 비슷합니다. 중요한 차이점은 양방향으로 연결을 해야 한다는 점입니다. 새로운 노드(25)를 기존의 노드(20, 30)와 연결하는 방법만 살펴보겠습니다.

  ![](https://s3.ap-northeast-2.amazonaws.com/opentutorials-user-file/module/1335/2958.png)

 **figure 2. Phase 1*

 ### 2. 노드 제거

  ![](https://s3.ap-northeast-2.amazonaws.com/opentutorials-user-file/module/1335/2975.png)

 **figure 14. Phase 7*

  삭제가 완료 되었습니다.
 
 
 ## 📌 참고
 
 https://qiao.github.io/PathFinding.js/visual/
 
 https://egloos.zum.com/cozycoz/v/9748811
 
 https://itmining.tistory.com/66

 https://deliorange.tistory.com/110
