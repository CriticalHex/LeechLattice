#include "Lattice.h"
#include "Listener.h"
#include "Rectangle.h"

using namespace std;

void smooth(float *x) {
  if (*x == 0)
    *x = 1;
  *x = atan(*x + .2);
}

int main() {
  const UINT frequencies = 1920 * 3;
  enum renderModes { RECTANGLE, LATTICE_VOL, LATTICE_FREQ };
  int renderMode = LATTICE_VOL;
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
  sf::Vector2f winSize = sf::Vector2f(window.getSize().x, window.getSize().y);

  vector<Lattice *> lattice;
  vector<au::Rectangle *> rects;

  switch (renderMode) {
  case RECTANGLE:
    for (int i = 0; i < frequencies; i++) {
      rects.push_back(
          new au::Rectangle((winSize.x / float(frequencies)) * i, winSize.y,
                            winSize.x / float(frequencies), winSize.y, i));
    }
    break;
  case LATTICE_VOL:
    lattice.push_back(new Lattice(p1.x, p1.y, winSize.x, winSize.y));
    lattice.push_back(new Lattice(p2.x, p2.y, winSize.x, winSize.y));
    lattice.push_back(new Lattice(p3.x, p3.y, winSize.x, winSize.y));
    break;
  case LATTICE_FREQ:
    for (int i = 1; i <= frequencies; i++) {
      lattice.push_back(new Lattice((winSize.x / (frequencies + 1)) * i,
                                    middle.y, winSize.x / 5, winSize.y / 5,
                                    i - 1));
    }
    break;
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

    switch (renderMode) {
    case RECTANGLE:
      volumes = listener.getFrequencyData();
      for (auto rec : rects) {
        rec->update(volumes);
      }
    case LATTICE_VOL:
      listener.getAudioLevel(&volume);
      smooth(&volume);
      volumes = listener.getFrequencyData();
      for (auto lat : lattice) {
        lat->update(time.getElapsedTime().asSeconds() / 30, volume);
      }
      break;
    case LATTICE_FREQ:
      volumes = listener.getFrequencyData();
      for (auto lat : lattice) {
        lat->update(time.getElapsedTime().asSeconds() / 30, volumes);
      }
    }

    window.clear();

    switch (renderMode) {
    case RECTANGLE:
      for (auto rec : rects) {
        rec->draw(window);
      }
      break;
    case LATTICE_VOL:
    case LATTICE_FREQ:
      for (auto lat : lattice) {
        lat->draw(window);
      }
      break;
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