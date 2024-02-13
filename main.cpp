#include "Lattice.h"
#include "Listener.hpp"

using namespace std;

void smooth(float *x) {
  if (*x == 0)
    *x = 1;
  *x = atan(*x + .2);
}

int main() {
  Listener listener;
  float volume;

  sf::ContextSettings settings;
  settings.antialiasingLevel = 4;
  sf::RenderWindow window(sf::VideoMode(1920 * 3, 1080), "Leech Lattice",
                          sf::Style::None, settings);
  window.setVerticalSyncEnabled(true);
  sf::Event event;
  sf::Clock time;
  sf::Clock frame_clock;
  UINT frames = 0;
  UINT framerate = 0;

  sf::Vector2f middle(1920 / 2, 1080 / 2);
  sf::Vector2f p1(middle);
  sf::Vector2f p2(middle.x + 1920, middle.y);
  sf::Vector2f p3(middle.x + 1920 * 2, middle.y);

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
    listener.getAudioLevel(&volume);
    smooth(&volume);
    listener.getFrequencyData();
    for (auto lat : lattice) {
      lat->update(time.getElapsedTime().asSeconds() / 30, volume);
    }
    window.clear();
    for (auto lat : lattice) {
      lat->draw(window);
    }
    window.display();
    frames++;
    if (frame_clock.getElapsedTime().asMilliseconds() >= 1000) {
      framerate = frames;
      frames = 0;
      frame_clock.restart();
    }
  }
  return 0;
}