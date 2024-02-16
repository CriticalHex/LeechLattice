#include "Lattice.h"

using namespace std;

double dist(sf::Vector2f p1, sf::Vector2f p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

static sf::Color lerpColor(const sf::Color &color1, const sf::Color &color2,
                           float t) {
  float r = color1.r + t * (color2.r - color1.r);
  float g = color1.g + t * (color2.g - color1.g);
  float b = color1.b + t * (color2.b - color1.b);
  return sf::Color(r, g, b, 100);
}

static sf::Color getColor(float t) {
  const float hue_range = 360.0f;
  float hue = fmod(t / 5.0f, 1.0f) * hue_range;
  int i = int(hue / 60) % 6;
  float f = hue / 60 - i;

  sf::Color colors[] = {sf::Color::Red,  sf::Color::Yellow, sf::Color::Green,
                        sf::Color::Cyan, sf::Color::Blue,   sf::Color::Magenta};

  sf::Color color1 = colors[i];
  sf::Color color2 = colors[(i + 1) % 6];
  return lerpColor(color1, color2, f);
}

Lattice::Lattice(int x, int y, int width, int height, int frequencyBand) {
  _position = sf::Vector2f(x, y);
  _width = width;
  _height = height;
  _frequencyBand = frequencyBand;
}

Lattice::~Lattice() {}

void Lattice::computeProjectedVectors(double time) {
  for (int i = 0; i < 24; i++) {
    if (_elevenCycle[i][0] == 0) {
      _projectedVectors[0][i] = 0;
      _projectedVectors[1][i] = 0;
    } else if (_elevenCycle[i][0] == 1) {
      _projectedVectors[0][i] =
          cos(2 * M_PI * (_elevenCycle[i][1] / 11 + time)) / _normalizer;
      _projectedVectors[1][i] =
          -sin(2 * M_PI * (_elevenCycle[i][1] / 11 + time)) / _normalizer;
    } else if (_elevenCycle[i][0] == 2) {
      _projectedVectors[0][i] =
          cos(2 * M_PI * (_elevenCycle[i][1] / 11 - time)) / _normalizer;
      _projectedVectors[1][i] =
          -sin(2 * M_PI * (_elevenCycle[i][1] / 11 - time)) / _normalizer;
    }
  }
}

void Lattice::computeProjectedPoints(double volume) {
  for (int i = 0; i < 100; i++) {
    double u = 0;
    double v = 0;
    for (int j = 0; j < 24; j++) {
      u += _points[i][j] * _projectedVectors[0][j];
      v += _points[i][j] * _projectedVectors[1][j];
    }
    _projectedPoints[i].x = _position.x + (volume * _height / 5.f) * v;
    _projectedPoints[i].y = _position.y + (volume * _height / 5.f) * u;
  }
}

void Lattice::draw(sf::RenderWindow &window) {
  drawLines(window);
  // drawPoints(window);
  // window.draw(_sprite);
}

void Lattice::drawLines(sf::RenderWindow &window) {
  int i, j, n;
  for (n = 0; n < 1100; n++) {
    i = _edges[n][0];
    j = _edges[n][1];
    sf::VertexArray vx(sf::PrimitiveType::Lines, 2);
    vx[0] = sf::Vertex(_projectedPoints[i], _color);
    vx[1] = sf::Vertex(_projectedPoints[j], _color);
    window.draw(vx);
  }
}

void Lattice::drawPoints(sf::RenderWindow &window) {
  int i, j, n;
  for (n = 0; n < 100; n++) {
    sf::CircleShape circle(2);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setPosition(_projectedPoints[n]);
    window.draw(circle);
  }
}

void Lattice::update(double time, double volume) {
  computeProjectedVectors(time);
  computeProjectedPoints(volume);
  _color = getColor(time);
}

float smooth(float x) { return atan(x + .2); }

void Lattice::update(double time, vector<float> volume) {
  computeProjectedVectors(time);
  computeProjectedPoints(smooth(volume[_frequencyBand]));
  _color = getColor(time);
}

// void Lattice::printPoints() {
//   for (int i = 0; i < _points.size(); i++) {
//     cout << "{";
//     for (int j = 0; j < _points[i].size(); j++) {
//       cout << _points[i][j] << ", ";
//     }
//     cout << "\b\b}," << endl;
//   }
//   cout << endl << endl << endl;
// }

// void Lattice::printEdges() {
//   sf::Vector2f v;
//   for (int i = 0; i < 1100; i++) {
//     v = _edges[i];
//     cout << "{" << v.x << ", " << v.y << "}, ";
//   }
//   cout << endl << endl << endl;
// }

// void Lattice::createPoints() {
//   vector<int> t(24);
//   for (int i = 0; i < 24; i++) {
//     t[i] = (i == 0 || i == 4) ? 4 : 0;
//   }
//   _points.push_back(t);
//   for (int j = 0; j < 24; j++) {
//     if (!(j == 0 || j == 4)) {
//       t = vector<int>(24);
//       for (int i = 0; i < 24; i++)
//         t[i] = (i == j) ? -3 : 1;
//       _points.push_back(t);
//     }
//   }
//   for (int j = 0; j < 77; j++) {
//     t = vector<int>(24);
//     for (int i = 0; i < 24; i++)
//       t[i] = ((_octads[j] >> i) & 1) ? 2 : 0;
//     _points.push_back(t);
//   }
//   printPoints();
// }

// void Lattice::fillEdges() {
//   int count = 0;
//   for (int i = 0; i < 100; i++) {
//     for (int j = 0; j < i; j++) {
//       int v = 0;
//       for (int k = 0; k < 24; k++) {
//         v += (_points[i][k] - _points[j][k]) * (_points[i][k] -
//         _points[j][k]);
//       }
//       if (v == 48) {
//         _edges[count] = sf::Vector2f(j, i);
//         count++;
//       }
//     }
//   }
// }