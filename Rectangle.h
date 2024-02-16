#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SFML\Graphics.hpp>
#include <vector>

namespace au {

class Rectangle {
private:
  sf::RectangleShape _rect;
  int _band;
  float _maxHeight;

public:
  void update(float time, std::vector<float> volume);
  void draw(sf::RenderWindow &window);
  Rectangle(int x, int y, int width, int height, int band);
  ~Rectangle();
};

} // namespace au

#endif