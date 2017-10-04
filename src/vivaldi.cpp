#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <ctime>
#include <string>
#include <time.h>
#include <functional>
#include "vivaldi.h"

Vivaldi::Vivaldi():Vivaldi(0.1, 0.1," ")
{
}

Vivaldi::Vivaldi(double c_e, double c_c,string id)
{
    const_c = c_c;
    const_e = c_e;
    std::hash<std::string> hasher;
    auto hashed = hasher(id);

    srand(hashed);
    _nsamples = 0;

    rand();
    _c._lat = (std::rand() % 201 + (-100));
    _c._lng = (std::rand() % 201 + (-100));
    _error = 0.02; //used for adaptive - not required here
}

Vivaldi::Vivaldi(int lat, int lng, double e)
{
    _nsamples = 0;
    _c._lat = lat;
    _c._lng = lng;
    _error = e;

    const_c = 0.01;
    const_e = 0.01;
}

Vivaldi::~Vivaldi()
{
}

void Vivaldi::sample(Coord c, double latency, double error)
{
    algorithm_simple(Sample(c, latency, error));
    
    // Other version of the algorithm can be called here
    // algorithm_adaptive(Sample(c, latency, error));
    // _nsamples += 1;
}

Vivaldi::Coord Vivaldi::net_force_simple(vector<Sample> v)
{
    Coord f;
    f._lat = 0;
    f._lng = 0;
    for(unsigned i = 0; i < v.size(); i++)
    {
        double d = dist(_c, v[i]._c);
        if(d > 0.01)
        {
          Coord direction = (v[i]._c - _c);
          direction = direction / d;
          f = f + (direction * (d - v[i]._latency));
        }
    }
    return f;
}

void Vivaldi::algorithm_simple(Sample s)
{
  _samples.push_back(s);

  Coord f = net_force_simple(_samples);

  // apply the force to our coordinates
  _c = _c + (f * 0.001);

  _samples.clear();
}

/*
Vivaldi::Coord Vivaldi::net_force_adaptive(vector<Sample> v)
{
    //cout << "Entering net force" << endl;
    Coord f;
    f._lat = 0;
    f._lng = 0;
    for(unsigned i = 0; i < v.size(); i++)
    {
        //if(_error+v[i]._error == 0) { cout<<"Divide by zero!!"<<endl; continue;}
        double weight = _error/(_error + v[i]._error);
        double d = dist(_c, v[i]._c);
        double rel_err = fabs(d - v[i]._latency) / v[i]._latency;
        _error = (rel_err * const_e * weight) + _error * (1 - (const_e * weight));
        if(d > 0.01)
        {
          Coord direction = (_c - v[i]._c);
          direction = direction / d;
          f = f + (direction * (v[i]._latency - d)) * weight;
        }
        //cout << "weight " << weight << " d: " << d << " rel_err: " << rel_err << endl;
    }
    return f;
}

void Vivaldi::algorithm_adaptive(Sample s)
{
  _samples.push_back(s);

  Coord f = net_force_adaptive(_samples);

  //cout << f._lat << "f lat f lng " << f._lng << endl;   
  // apply the force to our coordinates
  _c = _c + (f * const_c);

  _samples.clear();
}
*/