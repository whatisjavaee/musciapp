#include "complex.h"
ComplexNumber::ComplexNumber()
{
    real = imag = 0.0;
}

ComplexNumber::ComplexNumber(double re)
{
    real = re;
    imag = 0.0;
}

ComplexNumber::ComplexNumber(double re, double im)
{
    real = re;
    imag = im;
}
void ComplexNumber::write() const
{
    //cout << real << " + " << imag << 'i';
}

ComplexNumber ComplexNumber::operator +(const ComplexNumber &u) const
{
    ComplexNumber v(real + u.real, imag + u.imag);
    return v;
}

ComplexNumber ComplexNumber::operator -(const ComplexNumber &u) const
{
    ComplexNumber v(real - u.real, imag - u.imag);
    return v;
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber &u) const
{
    ComplexNumber v(real * u.real - imag * u.imag,
                    real * u.imag + imag * u.real);
    return v;
}

ComplexNumber ComplexNumber::operator /(const ComplexNumber &u) const
{
    double temp = u.real * u.real + u.imag * u.imag;
    ComplexNumber v((real * u.real + imag * u.imag) / temp,
                    (imag * u.real - real * u.imag) / temp);
    return v;
}
