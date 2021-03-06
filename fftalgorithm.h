#ifndef FFTALGORITHM_H
#define FFTALGORITHM_H

#include <QtGlobal>
#include <math.h>
#include <float.h>
#include "complex.h"
#include <QDebug>
#include "audioinput.h"
#include "yf.h"
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

struct Peak
{
    int index;
    double value;
    bool isDouble = false;
    Peak(int _index,double _value){
        index = _index;
        value = _value;
    }
};

/// <summary>
/// Gets number of significat bytes.
/// </summary>
/// <param name="n">Number</param>
/// <returns>Amount of minimal bits to store the number.</returns>
int Log2(int n);
/// <summary>
/// Checks if number is power of 2.
/// </summary>
/// <param name="n">number</param>
/// <returns>true if n=2^k and k is positive integer</returns>
bool IsPowerOfTwo(int n);
/// <summary>
/// Reverses bits in the number.
/// </summary>
/// <param name="n">Number</param>
/// <param name="bitsCount">Significant bits in the number.</param>
/// <returns>Reversed binary number.</returns>
int ReverseBits(int n, int bitsCount);

/// <summary>
/// Cooley-Tukey FFT algorithm.
/// </summary>
/// <summary>
/// Calculates FFT using Cooley-Tukey FFT algorithm.
/// </summary>
/// <param name="x">input data</param>
/// <returns>spectrogram of the data</returns>
/// <remarks>
/// If amount of data items not equal a power of 2, then algorithm
/// automatically pad with 0s to the lowest amount of power of 2.
/// </remarks>

double* Calculate(double* x, int len);

int findMaxPeak(double* values, int values_length, int* peak, int peak_length);

void cutNotPeak(double* values, int index, int length);

void zxg(double* values, int start, int end, int length);
bool greaterSort(Peak a, Peak b);
void cacIsRight(double* mydata,std::vector<YFData*> yfdata,quint32 maxValue);
std::vector<Peak> findPeaks(double* values, int start, int end);
std::vector<Peak> sortPeaks(std::vector<Peak> peaks);
std::vector<Peak>  getUsefullPeaks(double* mydata);
void initIsDouble(std::vector<Peak> &peaks,unsigned long size);
#endif // FFTALGORITHM
