#include "Lattice.h"

using namespace std;

int main() {
  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Leech Lattice",
                          sf::Style::None);
  Lattice lattice(window.getSize().x, window.getSize().y);
  sf::Event event;
  sf::Clock time;
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
    lattice.update(time.getElapsedTime().asSeconds() / 25);
    window.clear();
    lattice.draw(window);
    window.display();
  }
  return 0;
}