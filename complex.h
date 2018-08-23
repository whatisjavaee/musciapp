#ifndef COMPLEX_H
#define COMPLEX_H
#include <math.h>
class ComplexNumber
{
public:
    ComplexNumber();
    ComplexNumber(double);
    ComplexNumber(double, double);
    void write() const;
    ComplexNumber operator +(const ComplexNumber&) const;
    ComplexNumber operator -(const ComplexNumber&) const;
    ComplexNumber operator *(const ComplexNumber&) const;
    ComplexNumber operator /(const ComplexNumber&) const;
    double AbsPower2()
    {
        return real * real + imag * imag;
    }
    ComplexNumber PoweredE()
    {
        double e = exp(real);
        return ComplexNumber(e * cos(imag), e * sin(imag));
    }
private:
    double real;
    double imag;
};
#endif // COMPLEX
