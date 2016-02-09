#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <math.h>

class Complex
{
    public:
        Complex() : real(0.0), imag(0.0) {}
        Complex(float r, float i) : real(r), imag(i) {}
        float getMagnitude(){return sqrt(real*real+imag*imag);}
        float getPhase(){return atan2(imag, real);}
        float real;
        float imag;
};

#endif