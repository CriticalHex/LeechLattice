#include "Lattice.h"

using namespace std;

Lattice::Lattice(int width, int height) {
  _width = width;
  _height = height;
}

Lattice::~Lattice() {}

void Lattice::computeProjectedVectors(double time) {
  // 11s cause of 11 cycle I guess
  // COULD refactor because elevenCycle[i][0] is known
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

void Lattice::computePointProj() {
  for (int i = 0; i < 100; i++) {
    int u = 0;
    int v = 0;
    for (int j = 0; j < 24; j++) {
      u += _points[i][j] * _projectedVectors[0][j];
      v += _points[i][j] * _projectedVectors[1][j];
    }
    _projectedPoints[i].x = (_width / 2.f) + (_height / 5.f) * v;
    _projectedPoints[i].y = (_height / 2.f) + (_height / 5.f) * u;
  }
}

void Lattice::draw(sf::RenderWindow &window) { drawLines(window); }

void Lattice::drawLines(sf::RenderWindow &window) {
  int i, j, n;
  for (n = 0; n < 1100; n++) {
    i = _edges[n][0];
    j = _edges[n][1];
    sf::VertexArray vx(sf::PrimitiveType::Lines, 2);
    vx[0] = sf::Vertex(_projectedPoints[i], sf::Color::Green);
    vx[1] = sf::Vertex(_projectedPoints[j], sf::Color::Green);
    window.draw(vx);
    // cout << _projectedPoints[i].x << ", " << _projectedPoints[i].y << " to "
    //      << _projectedPoints[j].x << ", " << _projectedPoints[j].y << endl;
    // cout << _edges[n][0] << ", " << _edges[n][1] << endl;
  }
}

void Lattice::drawPoints(sf::RenderWindow &window) {
  int i, j, n;
  for (n = 0; n < 100; n++) {
    i = _edges[n][0];
    j = _edges[n][1];
    sf::VertexArray vx(sf::PrimitiveType::Lines, 2);
    vx[0] = sf::Vertex(_projectedPoints[i], sf::Color::Green);
    vx[1] = sf::Vertex(_projectedPoints[j], sf::Color::Green);
    window.draw(vx);
    // cout << _projectedPoints[i].x << ", " << _projectedPoints[i].y << " to "
    //      << _projectedPoints[j].x << ", " << _projectedPoints[j].y << endl;
    // cout << _edges[n][0] << ", " << _edges[n][1] << endl;
  }
}

void Lattice::update(double time) {
  // cout << time << endl;
  computePointProj();
  computeProjectedVectors(time);
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