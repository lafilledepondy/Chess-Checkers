// ==============================================================================
// main.cpp
// ==============================================================================

#include <iostream>

#include "checkerboard.hpp"


/*
TODO

  A B C D E F G H
8 r h b q k b h r
7 p p p p p p p p
6 . . . . . . . .
5 . . . . . . . .
4 . . . . . . . .
3 . . . . . . . .
2 P P P P P P P P
1 R H B Q K B H R

White move:
> D3
*/

int main() {
  std::cout << "+==============================================================+" << std::endl;
  std::cout << "|                       CHESS & CHECKERS                       |" << std::endl;
  std::cout << "+==============================================================+" << std::endl;

  Checkerboard cb;

  cb.initialConditions();

  std::cout << cb.toString() << std::endl;


  return 0;
}
