#ifndef LATTICE_H
#define LATTICE_H

#include <SFML\Graphics.hpp>
#include <cmath>
#include <vector>

class Lattice {
private:
  const int _octads[77] = {
      255,      13107,    21845,    26265,    39321,    43605,    52275,
      197525,   208985,   329017,   348307,   394835,   417845,   592211,
      626741,   657977,   696467,   789653,   835673,   1121809,  1171729,
      1198289,  1213469,  1320987,  1344177,  1582193,  1589783,  1970449,
      2181149,  2196689,  2239761,  2282001,  2365553,  2392343,  2626587,
      2638257,  2957841,  3146585,  3158165,  3342387,  4270257,  4293147,
      4331543,  4358513,  4475665,  4502545,  4723409,  4727069,  4932625,
      5244307,  5263417,  5570645,  6293045,  6316115,  6684825,  7899153,
      8463383,  8471153,  8524977,  8536347,  8655389,  8659409,  8882193,
      8943633,  8947473,  9439541,  9474131,  10027161, 10488467, 10526777,
      11141205, 11813905, 12586073, 12632213, 13369395, 13771281, 14749969};

  const float _elevenCycle[24][2] = {
      {0},    {1, 0},  {1, 1}, {1, 8}, {0},    {2, 0}, {2, 3}, {2, 10},
      {2, 8}, {1, 2},  {1, 9}, {1, 6}, {1, 3}, {2, 9}, {2, 5}, {2, 2},
      {2, 7}, {1, 10}, {1, 5}, {1, 7}, {1, 4}, {2, 1}, {2, 4}, {2, 6}};

  const int _points[100][24] = {
      {4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -3},
      {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 0, 0, 2, 2, 0, 0, 2, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
      {2, 0, 2, 0, 2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
      {2, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
      {2, 0, 0, 2, 2, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0},
      {2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0},
      {2, 2, 0, 0, 2, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0},
      {2, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0},
      {2, 2, 0, 0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0},
      {2, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0},
      {2, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0},
      {2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0},
      {2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0},
      {2, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 0, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0},
      {2, 0, 0, 0, 2, 0, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0},
      {2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0},
      {2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0},
      {2, 0, 0, 0, 2, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 0},
      {2, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0},
      {2, 2, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0},
      {2, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0},
      {2, 0, 0, 0, 2, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 0, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0},
      {2, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0},
      {2, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0},
      {2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0},
      {2, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0},
      {2, 0, 0, 2, 2, 0, 2, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0},
      {2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0},
      {2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0},
      {2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0},
      {2, 2, 0, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 0},
      {2, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0},
      {2, 0, 0, 0, 2, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 2, 0, 0, 0, 2, 0},
      {2, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0},
      {2, 0, 2, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0, 2, 0},
      {2, 2, 0, 0, 2, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0},
      {2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2, 0},
      {2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2, 0},
      {2, 0, 2, 0, 2, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0},
      {2, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0},
      {2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 0},
      {2, 2, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2},
      {2, 0, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 2},
      {2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
      {2, 2, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2},
      {2, 0, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2},
      {2, 0, 0, 0, 2, 0, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2},
      {2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2},
      {2, 0, 2, 0, 2, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2},
      {2, 2, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2},
      {2, 0, 0, 2, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 2},
      {2, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2},
      {2, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 2, 0, 2},
      {2, 0, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 2, 0, 2},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 2, 2, 0, 2},
      {2, 0, 0, 2, 2, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
      {2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2},
      {2, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 2, 2},
      {2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2, 2},
      {2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2, 2}};

  const int _edges[1100][2] = {
      {0, 1},   {0, 2},   {0, 3},   {0, 4},   {0, 5},   {0, 6},   {0, 7},
      {0, 8},   {0, 9},   {0, 10},  {0, 11},  {0, 12},  {0, 13},  {0, 14},
      {0, 15},  {0, 16},  {0, 17},  {0, 18},  {0, 19},  {0, 20},  {0, 21},
      {0, 22},  {1, 23},  {2, 23},  {3, 23},  {4, 23},  {5, 23},  {6, 23},
      {1, 24},  {4, 24},  {7, 24},  {8, 24},  {11, 24}, {12, 24}, {2, 25},
      {5, 25},  {7, 25},  {9, 25},  {11, 25}, {13, 25}, {3, 26},  {6, 26},
      {8, 26},  {9, 26},  {12, 26}, {13, 26}, {3, 27},  {6, 27},  {7, 27},
      {10, 27}, {11, 27}, {14, 27}, {2, 28},  {5, 28},  {8, 28},  {10, 28},
      {12, 28}, {14, 28}, {1, 29},  {4, 29},  {9, 29},  {10, 29}, {13, 29},
      {14, 29}, {2, 30},  {6, 30},  {7, 30},  {8, 30},  {15, 30}, {16, 30},
      {29, 30}, {3, 31},  {5, 31},  {11, 31}, {12, 31}, {15, 31}, {16, 31},
      {29, 31}, {3, 32},  {4, 32},  {7, 32},  {9, 32},  {15, 32}, {17, 32},
      {28, 32}, {1, 33},  {6, 33},  {11, 33}, {13, 33}, {15, 33}, {17, 33},
      {28, 33}, {1, 34},  {5, 34},  {8, 34},  {9, 34},  {16, 34}, {17, 34},
      {27, 34}, {2, 35},  {4, 35},  {12, 35}, {13, 35}, {16, 35}, {17, 35},
      {27, 35}, {1, 36},  {5, 36},  {7, 36},  {10, 36}, {15, 36}, {18, 36},
      {26, 36}, {35, 36}, {2, 37},  {4, 37},  {11, 37}, {14, 37}, {15, 37},
      {18, 37}, {26, 37}, {34, 37}, {3, 38},  {4, 38},  {8, 38},  {10, 38},
      {16, 38}, {18, 38}, {25, 38}, {33, 38}, {1, 39},  {6, 39},  {12, 39},
      {14, 39}, {16, 39}, {18, 39}, {25, 39}, {32, 39}, {2, 40},  {6, 40},
      {9, 40},  {10, 40}, {17, 40}, {18, 40}, {24, 40}, {31, 40}, {3, 41},
      {5, 41},  {13, 41}, {14, 41}, {17, 41}, {18, 41}, {24, 41}, {30, 41},
      {8, 42},  {9, 42},  {10, 42}, {11, 42}, {15, 42}, {19, 42}, {23, 42},
      {35, 42}, {39, 42}, {41, 42}, {7, 43},  {12, 43}, {13, 43}, {14, 43},
      {15, 43}, {19, 43}, {23, 43}, {34, 43}, {38, 43}, {40, 43}, {5, 44},
      {6, 44},  {10, 44}, {13, 44}, {16, 44}, {19, 44}, {24, 44}, {32, 44},
      {37, 44}, {2, 45},  {3, 45},  {9, 45},  {14, 45}, {16, 45}, {19, 45},
      {24, 45}, {33, 45}, {36, 45}, {1, 46},  {3, 46},  {10, 46}, {12, 46},
      {17, 46}, {19, 46}, {25, 46}, {30, 46}, {37, 46}, {4, 47},  {6, 47},
      {8, 47},  {14, 47}, {17, 47}, {19, 47}, {25, 47}, {31, 47}, {36, 47},
      {4, 48},  {5, 48},  {9, 48},  {12, 48}, {18, 48}, {19, 48}, {27, 48},
      {30, 48}, {33, 48}, {1, 49},  {2, 49},  {8, 49},  {13, 49}, {18, 49},
      {19, 49}, {27, 49}, {31, 49}, {32, 49}, {7, 50},  {11, 50}, {16, 50},
      {17, 50}, {18, 50}, {19, 50}, {23, 50}, {26, 50}, {28, 50}, {29, 50},
      {2, 51},  {3, 51},  {10, 51}, {13, 51}, {15, 51}, {20, 51}, {24, 51},
      {34, 51}, {39, 51}, {47, 51}, {48, 51}, {50, 51}, {5, 52},  {6, 52},
      {9, 52},  {14, 52}, {15, 52}, {20, 52}, {24, 52}, {35, 52}, {38, 52},
      {46, 52}, {49, 52}, {50, 52}, {7, 53},  {9, 53},  {10, 53}, {12, 53},
      {16, 53}, {20, 53}, {23, 53}, {33, 53}, {37, 53}, {41, 53}, {47, 53},
      {49, 53}, {8, 54},  {11, 54}, {13, 54}, {14, 54}, {16, 54}, {20, 54},
      {23, 54}, {32, 54}, {36, 54}, {40, 54}, {46, 54}, {48, 54}, {4, 55},
      {5, 55},  {10, 55}, {11, 55}, {17, 55}, {20, 55}, {26, 55}, {30, 55},
      {39, 55}, {43, 55}, {45, 55}, {49, 55}, {1, 56},  {2, 56},  {7, 56},
      {14, 56}, {17, 56}, {20, 56}, {26, 56}, {31, 56}, {38, 56}, {42, 56},
      {44, 56}, {48, 56}, {1, 57},  {3, 57},  {9, 57},  {11, 57}, {18, 57},
      {20, 57}, {28, 57}, {30, 57}, {35, 57}, {43, 57}, {44, 57}, {47, 57},
      {4, 58},  {6, 58},  {7, 58},  {13, 58}, {18, 58}, {20, 58}, {28, 58},
      {31, 58}, {34, 58}, {42, 58}, {45, 58}, {46, 58}, {8, 59},  {12, 59},
      {15, 59}, {17, 59}, {18, 59}, {20, 59}, {23, 59}, {25, 59}, {27, 59},
      {29, 59}, {44, 59}, {45, 59}, {3, 60},  {5, 60},  {7, 60},  {8, 60},
      {19, 60}, {20, 60}, {29, 60}, {33, 60}, {35, 60}, {37, 60}, {39, 60},
      {40, 60}, {2, 61},  {6, 61},  {11, 61}, {12, 61}, {19, 61}, {20, 61},
      {29, 61}, {32, 61}, {34, 61}, {36, 61}, {38, 61}, {41, 61}, {1, 62},
      {4, 62},  {15, 62}, {16, 62}, {19, 62}, {20, 62}, {25, 62}, {26, 62},
      {27, 62}, {28, 62}, {40, 62}, {41, 62}, {4, 63},  {6, 63},  {10, 63},
      {12, 63}, {15, 63}, {21, 63}, {25, 63}, {34, 63}, {41, 63}, {45, 63},
      {49, 63}, {50, 63}, {54, 63}, {56, 63}, {57, 63}, {60, 63}, {1, 64},
      {3, 64},  {8, 64},  {14, 64}, {15, 64}, {21, 64}, {25, 64}, {35, 64},
      {40, 64}, {44, 64}, {48, 64}, {50, 64}, {53, 64}, {55, 64}, {58, 64},
      {61, 64}, {1, 65},  {2, 65},  {10, 65}, {11, 65}, {16, 65}, {21, 65},
      {26, 65}, {32, 65}, {41, 65}, {43, 65}, {47, 65}, {48, 65}, {52, 65},
      {58, 65}, {59, 65}, {60, 65}, {4, 66},  {5, 66},  {7, 66},  {14, 66},
      {16, 66}, {21, 66}, {26, 66}, {33, 66}, {40, 66}, {42, 66}, {46, 66},
      {49, 66}, {51, 66}, {57, 66}, {59, 66}, {61, 66}, {7, 67},  {8, 67},
      {10, 67}, {13, 67}, {17, 67}, {21, 67}, {23, 67}, {31, 67}, {37, 67},
      {39, 67}, {45, 67}, {48, 67}, {52, 67}, {57, 67}, {61, 67}, {62, 67},
      {9, 68},  {11, 68}, {12, 68}, {14, 68}, {17, 68}, {21, 68}, {23, 68},
      {30, 68}, {36, 68}, {38, 68}, {44, 68}, {49, 68}, {51, 68}, {58, 68},
      {60, 68}, {62, 68}, {5, 69},  {6, 69},  {8, 69},  {11, 69}, {18, 69},
      {21, 69}, {29, 69}, {32, 69}, {35, 69}, {43, 69}, {45, 69}, {46, 69},
      {51, 69}, {53, 69}, {56, 69}, {62, 69}, {2, 70},  {3, 70},  {7, 70},
      {12, 70}, {18, 70}, {21, 70}, {29, 70}, {33, 70}, {34, 70}, {42, 70},
      {44, 70}, {47, 70}, {52, 70}, {54, 70}, {55, 70}, {62, 70}, {9, 71},
      {13, 71}, {15, 71}, {16, 71}, {18, 71}, {21, 71}, {23, 71}, {24, 71},
      {27, 71}, {28, 71}, {46, 71}, {47, 71}, {55, 71}, {56, 71}, {60, 71},
      {61, 71}, {1, 72},  {6, 72},  {7, 72},  {9, 72},  {19, 72}, {21, 72},
      {28, 72}, {31, 72}, {35, 72}, {37, 72}, {38, 72}, {41, 72}, {51, 72},
      {54, 72}, {55, 72}, {59, 72}, {3, 73},  {4, 73},  {11, 73}, {13, 73},
      {19, 73}, {21, 73}, {28, 73}, {30, 73}, {34, 73}, {36, 73}, {39, 73},
      {40, 73}, {52, 73}, {53, 73}, {56, 73}, {59, 73}, {2, 74},  {5, 74},
      {15, 74}, {17, 74}, {19, 74}, {21, 74}, {24, 74}, {26, 74}, {27, 74},
      {29, 74}, {38, 74}, {39, 74}, {53, 74}, {54, 74}, {57, 74}, {58, 74},
      {2, 75},  {4, 75},  {8, 75},  {9, 75},  {20, 75}, {21, 75}, {27, 75},
      {31, 75}, {33, 75}, {36, 75}, {39, 75}, {41, 75}, {43, 75}, {44, 75},
      {46, 75}, {50, 75}, {1, 76},  {5, 76},  {12, 76}, {13, 76}, {20, 76},
      {21, 76}, {27, 76}, {30, 76}, {32, 76}, {37, 76}, {38, 76}, {40, 76},
      {42, 76}, {45, 76}, {47, 76}, {50, 76}, {3, 77},  {6, 77},  {16, 77},
      {17, 77}, {20, 77}, {21, 77}, {24, 77}, {25, 77}, {28, 77}, {29, 77},
      {36, 77}, {37, 77}, {42, 77}, {43, 77}, {48, 77}, {49, 77}, {10, 78},
      {14, 78}, {18, 78}, {19, 78}, {20, 78}, {21, 78}, {23, 78}, {24, 78},
      {25, 78}, {26, 78}, {30, 78}, {31, 78}, {32, 78}, {33, 78}, {34, 78},
      {35, 78}, {1, 79},  {2, 79},  {9, 79},  {12, 79}, {15, 79}, {22, 79},
      {27, 79}, {38, 79}, {41, 79}, {44, 79}, {47, 79}, {50, 79}, {54, 79},
      {55, 79}, {58, 79}, {60, 79}, {66, 79}, {67, 79}, {69, 79}, {73, 79},
      {77, 79}, {78, 79}, {4, 80},  {5, 80},  {8, 80},  {13, 80}, {15, 80},
      {22, 80}, {27, 80}, {39, 80}, {40, 80}, {45, 80}, {46, 80}, {50, 80},
      {53, 80}, {56, 80}, {57, 80}, {61, 80}, {65, 80}, {68, 80}, {70, 80},
      {72, 80}, {77, 80}, {78, 80}, {4, 81},  {6, 81},  {9, 81},  {11, 81},
      {16, 81}, {22, 81}, {28, 81}, {36, 81}, {41, 81}, {43, 81}, {46, 81},
      {49, 81}, {51, 81}, {56, 81}, {59, 81}, {60, 81}, {64, 81}, {67, 81},
      {70, 81}, {74, 81}, {76, 81}, {78, 81}, {1, 82},  {3, 82},  {7, 82},
      {13, 82}, {16, 82}, {22, 82}, {28, 82}, {37, 82}, {40, 82}, {42, 82},
      {47, 82}, {48, 82}, {52, 82}, {55, 82}, {59, 82}, {61, 82}, {63, 82},
      {68, 82}, {69, 82}, {74, 82}, {75, 82}, {78, 82}, {2, 83},  {3, 83},
      {8, 83},  {11, 83}, {17, 83}, {22, 83}, {29, 83}, {36, 83}, {39, 83},
      {43, 83}, {44, 83}, {48, 83}, {52, 83}, {53, 83}, {58, 83}, {62, 83},
      {63, 83}, {66, 83}, {71, 83}, {72, 83}, {76, 83}, {78, 83}, {5, 84},
      {6, 84},  {7, 84},  {12, 84}, {17, 84}, {22, 84}, {29, 84}, {37, 84},
      {38, 84}, {42, 84}, {45, 84}, {49, 84}, {51, 84}, {54, 84}, {57, 84},
      {62, 84}, {64, 84}, {65, 84}, {71, 84}, {73, 84}, {75, 84}, {78, 84},
      {10, 85}, {14, 85}, {15, 85}, {16, 85}, {17, 85}, {22, 85}, {23, 85},
      {24, 85}, {25, 85}, {26, 85}, {48, 85}, {49, 85}, {57, 85}, {58, 85},
      {60, 85}, {61, 85}, {69, 85}, {70, 85}, {72, 85}, {73, 85}, {75, 85},
      {76, 85}, {10, 86}, {11, 86}, {12, 86}, {13, 86}, {18, 86}, {22, 86},
      {23, 86}, {30, 86}, {32, 86}, {34, 86}, {45, 86}, {47, 86}, {52, 86},
      {56, 86}, {60, 86}, {62, 86}, {64, 86}, {66, 86}, {72, 86}, {74, 86},
      {75, 86}, {77, 86}, {7, 87},  {8, 87},  {9, 87},  {14, 87}, {18, 87},
      {22, 87}, {23, 87}, {31, 87}, {33, 87}, {35, 87}, {44, 87}, {46, 87},
      {51, 87}, {55, 87}, {61, 87}, {62, 87}, {63, 87}, {65, 87}, {73, 87},
      {74, 87}, {76, 87}, {77, 87}, {2, 88},  {4, 88},  {7, 88},  {10, 88},
      {19, 88}, {22, 88}, {26, 88}, {31, 88}, {33, 88}, {34, 88}, {39, 88},
      {41, 88}, {52, 88}, {54, 88}, {57, 88}, {59, 88}, {64, 88}, {68, 88},
      {69, 88}, {71, 88}, {76, 88}, {77, 88}, {1, 89},  {5, 89},  {11, 89},
      {14, 89}, {19, 89}, {22, 89}, {26, 89}, {30, 89}, {32, 89}, {35, 89},
      {38, 89}, {40, 89}, {51, 89}, {53, 89}, {58, 89}, {59, 89}, {63, 89},
      {67, 89}, {70, 89}, {71, 89}, {75, 89}, {77, 89}, {3, 90},  {6, 90},
      {15, 90}, {18, 90}, {19, 90}, {22, 90}, {24, 90}, {25, 90}, {28, 90},
      {29, 90}, {34, 90}, {35, 90}, {53, 90}, {54, 90}, {55, 90}, {56, 90},
      {65, 90}, {66, 90}, {67, 90}, {68, 90}, {75, 90}, {76, 90}, {1, 91},
      {6, 91},  {8, 91},  {10, 91}, {20, 91}, {22, 91}, {25, 91}, {31, 91},
      {32, 91}, {35, 91}, {37, 91}, {41, 91}, {43, 91}, {45, 91}, {48, 91},
      {50, 91}, {66, 91}, {68, 91}, {70, 91}, {71, 91}, {73, 91}, {74, 91},
      {3, 92},  {4, 92},  {12, 92}, {14, 92}, {20, 92}, {22, 92}, {25, 92},
      {30, 92}, {33, 92}, {34, 92}, {36, 92}, {40, 92}, {42, 92}, {44, 92},
      {49, 92}, {50, 92}, {65, 92}, {67, 92}, {69, 92}, {71, 92}, {72, 92},
      {74, 92}, {2, 93},  {5, 93},  {16, 93}, {18, 93}, {20, 93}, {22, 93},
      {24, 93}, {26, 93}, {27, 93}, {29, 93}, {32, 93}, {33, 93}, {42, 93},
      {43, 93}, {46, 93}, {47, 93}, {63, 93}, {64, 93}, {67, 93}, {68, 93},
      {72, 93}, {73, 93}, {9, 94},  {13, 94}, {17, 94}, {19, 94}, {20, 94},
      {22, 94}, {23, 94}, {24, 94}, {27, 94}, {28, 94}, {30, 94}, {31, 94},
      {36, 94}, {37, 94}, {38, 94}, {39, 94}, {63, 94}, {64, 94}, {65, 94},
      {66, 94}, {69, 94}, {70, 94}, {3, 95},  {5, 95},  {9, 95},  {10, 95},
      {21, 95}, {22, 95}, {24, 95}, {30, 95}, {33, 95}, {35, 95}, {37, 95},
      {39, 95}, {43, 95}, {47, 95}, {49, 95}, {50, 95}, {54, 95}, {56, 95},
      {58, 95}, {59, 95}, {61, 95}, {62, 95}, {2, 96},  {6, 96},  {13, 96},
      {14, 96}, {21, 96}, {22, 96}, {24, 96}, {31, 96}, {32, 96}, {34, 96},
      {36, 96}, {38, 96}, {42, 96}, {46, 96}, {48, 96}, {50, 96}, {53, 96},
      {55, 96}, {57, 96}, {59, 96}, {60, 96}, {62, 96}, {1, 97},  {4, 97},
      {17, 97}, {18, 97}, {21, 97}, {22, 97}, {25, 97}, {26, 97}, {27, 97},
      {28, 97}, {30, 97}, {31, 97}, {42, 97}, {43, 97}, {44, 97}, {45, 97},
      {51, 97}, {52, 97}, {53, 97}, {54, 97}, {60, 97}, {61, 97}, {8, 98},
      {12, 98}, {16, 98}, {19, 98}, {21, 98}, {22, 98}, {23, 98}, {25, 98},
      {27, 98}, {29, 98}, {32, 98}, {33, 98}, {36, 98}, {37, 98}, {40, 98},
      {41, 98}, {51, 98}, {52, 98}, {55, 98}, {56, 98}, {57, 98}, {58, 98},
      {7, 99},  {11, 99}, {15, 99}, {20, 99}, {21, 99}, {22, 99}, {23, 99},
      {26, 99}, {28, 99}, {29, 99}, {34, 99}, {35, 99}, {38, 99}, {39, 99},
      {40, 99}, {41, 99}, {44, 99}, {45, 99}, {46, 99}, {47, 99}, {48, 99},
      {49, 99}};

  const double _normalizer = sqrt(11);

  double _projectedVectors[2][24];
  sf::Vector2f _projectedPoints[100];
  int _width, _height;
  sf::Vector2f _position;
  sf::Color _color = sf::Color(220, 208, 255, 100);
  int _frequencyBand;

  void computeProjectedPoints();
  void computeProjectedVectors(double time);
  void drawLines(sf::RenderWindow &window);
  void drawPoints(sf::RenderWindow &window);

  // points and edges were calculated then printed since they're constant
  // void createPoints();
  // void printPoints();
  // void printEdges();
  // void fillEdges();

public:
  Lattice(int x, int y, int width, int height, int frequencyBand = 0);
  void update(double time);
  void draw(sf::RenderWindow &window);
  ~Lattice();
};

#endif