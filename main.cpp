#include "Lattice.h"

using namespace std;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Leech Lattice",
                          sf::Style::None, settings);
  window.setVerticalSyncEnabled(true);
  sf::Event event;
  sf::Clock time;

  sf::Vector2f p1(710, 680);
  sf::Vector2f p2(1210, 680);
  sf::Vector2f p3(960, 280);

  vector<Lattice *> lattice;
  lattice.push_back(
      new Lattice(p1.x, p1.y, window.getSize().x, window.getSize().y));
  lattice.push_back(
      new Lattice(p2.x, p2.y, window.getSize().x, window.getSize().y));
  lattice.push_back(
      new Lattice(p3.x, p3.y, window.getSize().x, window.getSize().y));

  while (window.isOpen()) {
    while (window.pollEvent(event)) {
      if (event.type == event.Closed) {
        window.close();
      }
      if (event.type == event.KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
          window.close();
        }
      }
    }
    for (auto lat : lattice) {
      lat->update(time.getElapsedTime().asSeconds() / 30);
    }
    window.clear();
    for (auto lat : lattice) {
      lat->draw(window);
    }
    window.display();
  }
  return 0;
}