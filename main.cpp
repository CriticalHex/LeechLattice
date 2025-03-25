#include "Lattice.h"
#include "iostream"

using namespace std;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;
  const int screens = 3;
  sf::RenderWindow window(sf::VideoMode(1920 * screens, 1080), "Leech Lattice",
                          sf::Style::None, settings);
  window.setVerticalSyncEnabled(true);
  sf::Event event;
  sf::Clock time;

  sf::Vector2f winSize = sf::Vector2f(window.getSize().x, window.getSize().y);
  sf::Vector2f middle(winSize.x / screens / 2, winSize.y / 2);

  vector<Lattice *> lattice;
  for (int i = 0; i < screens; i++) {
    cout << i << endl;
    cout << middle.x + (winSize.x * i / screens) << endl;
    lattice.push_back(new Lattice(middle.x + (winSize.x / screens * i),
                                  middle.y, winSize.x, winSize.y));
  }

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

  for (int i = 0; i < lattice.size(); i++) {
    delete lattice[i];
  }
  lattice.clear();

  return 0;
}