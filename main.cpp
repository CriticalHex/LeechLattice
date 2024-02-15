#include "Lattice.h"
#include "Listener.h"

using namespace std;

void smooth(float *x) {
  if (*x == 0)
    *x = 1;
  *x = atan(*x + .2);
}

int main() {
  const UINT frequencies = 25;
  Listener listener(frequencies);
  float volume;
  vector<float> volumes;

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
  // lattice.push_back(
  //     new Lattice(p1.x, p1.y, window.getSize().x, window.getSize().y));
  // lattice.push_back(
  //     new Lattice(p2.x, p2.y, window.getSize().x, window.getSize().y));
  // lattice.push_back(
  //     new Lattice(p3.x, p3.y, window.getSize().x, window.getSize().y));
  for (int i = 1; i <= frequencies; i++) {
    lattice.push_back(new Lattice((window.getSize().x / (frequencies + 1)) * i,
                                  middle.y, window.getSize().x / 5,
                                  window.getSize().y / 5, i - 1));
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
    // listener.getAudioLevel(&volume);
    // smooth(&volume);

    volumes = listener.getFrequencyData();
    for (auto lat : lattice) {
      lat->update(time.getElapsedTime().asSeconds() / 30, volumes);
    }
    window.clear();
    for (auto lat : lattice) {
      lat->draw(window);
    }
    window.display();
    frames++;
    if (frame_clock.getElapsedTime().asMilliseconds() >= 1000) {
      framerate = frames;
      // cout << framerate << endl;
      frames = 0;
      frame_clock.restart();
    }
  }
  for (int i = 0; i < frequencies; i++) {
    delete lattice[0];
  }
  lattice.clear();
  return 0;
}