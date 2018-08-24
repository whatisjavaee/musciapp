#ifndef FFTALGORITHM_H
#define FFTALGORITHM_H

#include <QtGlobal>
#include <math.h>
#include <float.h>
#include "complex.h"
#include<QDebug>
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

int* FindPeaks(double* values, int index, int length,
               int peaksCount);
void ScanSignalIntervals(double* x, int index, int length, int intervalMin,
                         int intervalMax, int& optimalInterval, double& optimalValue,
                         int* peakIndices);

int findMaxPeak(double* values, int values_length, int* peak, int peak_length);

// <summary>
// Finds fundamental frequency: calculates spectrogram, finds peaks, analyzes
// and refines frequency by diff sample values.
// </summary>
// <param name="x">The sounds samples data</param>
// <param name="sampleRate">The sound sample rate</param>
// <param name="minFreq">The min useful frequency</param>
// <param name="maxFreq">The max useful frequency</param>
// <returns>Found frequency, 0 - otherwise</returns>
double FindFundamentalFrequency(double* x, int length, int sampleRate,
                                double minFreq, double maxFreq, double& result);

void cutNotPeak(double* values, int index, int length);

void zxg(double* values,int start,int end,int length);
#endif // FFTALGORITHM
