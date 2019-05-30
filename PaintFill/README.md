# PaintFill
## 📢 개요

 인접한 배열들을 검사하여 `0`을 `*`로 바꾸기
  
  >![capture](https://github.com/kbm0996/-Algorithm-Pathfind/blob/master/PaintFill/GIF.gif)
  >*figure 1. PaintFill(animated gif)*

## 📌 주요 소스
1. Recursive Function Version

        void PaintChar(int x, int y, char chData)
        {
          //-----------------------------------
          // Ver. Recursive Function
          //-----------------------------------
          if (g_Buffer[y][x] == '0')
          {
            g_Buffer[y][x] = chData;
            DrawBuffer();
            PaintChar(x - 1, y);
            PaintChar(x + 1, y);
            PaintChar(x, y - 1);
            PaintChar(x, y + 1);
          }
          else
            return;*/
        }

2. Loop Version

        void PaintChar(int x, int y, char chData)
        {
        //-----------------------------------
        // Ver. Stack 
        //-----------------------------------
          while (1)
          {
            if (g_Buffer[y][x] == '0')
            {
              if (Push(x, y + 1))
                g_Buffer[y][x] = chData;
              if (Push(x, y - 1))
                g_Buffer[y][x] = chData;
              if (Push(x + 1, y))
                g_Buffer[y][x] = chData;
              if (Push(x - 1, y))
                g_Buffer[y][x] = chData;
            }
            else
            {
              if (Pop(&x, &y))
                continue;
              else
                break;
            }
            system("cls");
            Draw();
          }
        }
