#include "Rectangle.h"

au::Rectangle::Rectangle(int x, int y, int width, int height, int band) {
  _rect.setPosition(sf::Vector2f(x, y));
  _rect.setFillColor(sf::Color::Red);
  _rect.setSize(sf::Vector2f(width, height));
  _band = band;
  _maxHeight = height;
}

au::Rectangle::~Rectangle() {}

void au::Rectangle::update(std::vector<float> volume) {
  if (volume[_band] == 0) {
    _rect.setSize(sf::Vector2f(_rect.getSize().x, _rect.getSize().y * .98f));
  } else {
    _rect.setSize(
        sf::Vector2f(_rect.getSize().x,
                     (.2 * _maxHeight) + (volume[_band] * (1.2 * _maxHeight))));
  }

  _rect.setOrigin(sf::Vector2f(0, _rect.getSize().y));
}

void au::Rectangle::draw(sf::RenderWindow &window) { window.draw(_rect); }