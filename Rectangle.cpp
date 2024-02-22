#include "Rectangle.h"
#include <cmath>

static sf::Color lerpColor(const sf::Color &color1, const sf::Color &color2,
                           float t) {
  float r = color1.r + t * (color2.r - color1.r);
  float g = color1.g + t * (color2.g - color1.g);
  float b = color1.b + t * (color2.b - color1.b);
  return sf::Color(r, g, b);
}

static sf::Color getColor(float t) {
  const float hue_range = 360.0f;
  float hue = std::fmod(t / 5.0f, 1.0f) * hue_range;
  int i = int(hue / 60) % 6;
  float f = hue / 60 - i;

  sf::Color colors[] = {sf::Color::Red,  sf::Color::Yellow, sf::Color::Green,
                        sf::Color::Cyan, sf::Color::Blue,   sf::Color::Magenta};

  sf::Color color1 = colors[i];
  sf::Color color2 = colors[(i + 1) % 6];
  return lerpColor(color1, color2, f);
}

au::Rectangle::Rectangle(int x, int y, int width, int height, int band) {
  _rect.setPosition(sf::Vector2f(x, y));
  _rect.setFillColor(sf::Color::Red);
  _rect.setSize(sf::Vector2f(width, height));
  _band = band;
  _maxHeight = height;
}

au::Rectangle::~Rectangle() {}

void au::Rectangle::update(float time, std::vector<float> volume) {
  _rect.setFillColor(getColor(time));
  if (volume[_band] == 0) {
    _rect.setSize(sf::Vector2f(_rect.getSize().x, _rect.getSize().y * .98f));
  } else {
    float rate = .2f;
    float newHeight = (0 * _maxHeight) + (rate * (volume[_band] * _maxHeight) +
                                          (1 - rate) * _rect.getSize().y);
    _rect.setSize(sf::Vector2f(_rect.getSize().x, newHeight));
  }

  _rect.setOrigin(sf::Vector2f(0, _rect.getSize().y));
}

void au::Rectangle::draw(sf::RenderWindow &window) { window.draw(_rect); }