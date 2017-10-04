#ifndef __VIVALDI_H
#define __VIVALDI_H

#include <vector>
#include <iostream>
using namespace std;

class Vivaldi 
{
  public:
    Vivaldi();
    Vivaldi(double c_e, double c_c, string id);
    Vivaldi(int lat, int lng, double e);
    virtual ~Vivaldi();
    int nsamples() { return _nsamples; }

    struct Coord 
    {
        double _lat;
        double _lng;
    };

    void sample(Coord c, double latency, double error);
    void print_my_location() { std::cout << "My location:" << _c._lat << "\t" <<_c._lng << std::endl;  }
    double print_my_lat(){ return _c._lat;}
    double print_my_lng(){ return _c._lng;}

    double const_c;
    double const_e;
    int _nsamples; // how many times sample() has been called
    Coord _c; // current estimated coordinates
    double _error;

    struct Sample 
    {
        Coord _c;
        double _latency;
        double _error;
        Sample(Coord c, double l, double e) {
        _c = c; _latency = l; _error = e;}
    };

    vector<Sample> _samples;

    Coord net_force_simple(vector<Sample> v);
    void algorithm_simple(Sample);
    Coord net_force_adaptive(vector<Sample> v);
    void algorithm_adaptive(Sample);
};

inline double dist(Vivaldi::Coord a, Vivaldi::Coord b)
{
  double dlat = a._lat - b._lat;
  double dlng = a._lng - b._lng;
  return sqrt(dlat*dlat + dlng*dlng);
}

inline Vivaldi::Coord operator-(Vivaldi::Coord a, Vivaldi::Coord b)
{
  Vivaldi::Coord c;
  c._lat = a._lat - b._lat;
  c._lng = a._lng - b._lng;
  return c;
}

inline Vivaldi::Coord operator+(Vivaldi::Coord a, Vivaldi::Coord b)
{
  Vivaldi::Coord c;
  c._lat = a._lat + b._lat;
  c._lng = a._lng + b._lng;
  return c;
}

inline Vivaldi::Coord operator/(Vivaldi::Coord c, double x)
{
  c._lat /= x;
  c._lng /= x;
  return c;
}

inline Vivaldi::Coord operator*(Vivaldi::Coord c, double x)
{
  c._lat *= x;
  c._lng *= x;
  return c;
}

inline double length(Vivaldi::Coord c)
{
  return sqrt(c._lat*c._lat + c._lng*c._lng);
}

#endif
