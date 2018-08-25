#include "fftalgorithm.h"
//#define M_PI       3.14159265358979323846   // pi
/// <summary>
/// Gets number of significat bytes.
/// </summary>
/// <param name="n">Number</param>
/// <returns>Amount of minimal bits to store the number.</returns>
int Log2(int n)
{
    int i = 0;
    while (n > 0)
    {
        ++i;
        n >>= 1;
    }
    return i;
}

/// <summary>
/// Checks if number is power of 2.
/// </summary>
/// <param name="n">number</param>
/// <returns>true if n=2^k and k is positive integer</returns>
bool IsPowerOfTwo(int n)
{
    return n > 1 && (n & (n - 1)) == 0;
}

/// <summary>
/// Reverses bits in the number.
/// </summary>
/// <param name="n">Number</param>
/// <param name="bitsCount">Significant bits in the number.</param>
/// <returns>Reversed binary number.</returns>
int ReverseBits(int n, int bitsCount)
{
    int reversed = 0;
    for (int i = 0; i < bitsCount; i++)
    {
        int nextBit = n & 1;
        n >>= 1;

        reversed <<= 1;
        reversed |= nextBit;
    }
    return reversed;
}

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

double* Calculate(double* x, int len)
{
    int length;
    int bitsInLength;
    if (IsPowerOfTwo(len))
    {
        length = len;
        bitsInLength = Log2(length) - 1;
    }
    else
    {
        bitsInLength = Log2(len);
        length = 1 << bitsInLength;
        // the items will be pad with zeros
    }

    // bit reversal
    ComplexNumber* data = new ComplexNumber[length];
    for (int i = 0; i < len; i++)
    {
        int j = ReverseBits(i, bitsInLength);
        data[j] = ComplexNumber(x[i]);
    }

    // Cooley-Tukey
    for (int i = 0; i < bitsInLength; i++)
    {
        int m = 1 << i;
        int n = m * 2;
        double alpha = -(2 * M_PI / n);

        for (int k = 0; k < m; k++)
        {
            // e^(-2*pi/N*k)
            ComplexNumber oddPartMultiplier =
                ComplexNumber(0, alpha * k).PoweredE();

            for (int j = k; j < length; j += n)
            {
                ComplexNumber evenPart = data[j];
                ComplexNumber oddPart = oddPartMultiplier * data[j + m];
                data[j] = evenPart + oddPart;
                data[j + m] = evenPart - oddPart;
            }
        }
    }

    // calculate spectrogram
    double* spectrogram = new double[length];
    for (int i = 0; i < length; i++)
    {
        spectrogram[i] = data[i].AbsPower2();
    }
    delete[] data;
    return spectrogram;
}

int* FindPeaks(double* values, int index, int length,
               int peaksCount)
{
    double* peakValues = new double[peaksCount];
    int* peakIndices = new int[peaksCount];

    for (int i = 0; i < peaksCount; i++)
    {
        peakValues[i] = values[peakIndices[i] = i + index];
    }

    // find min peaked value
    double minStoredPeak = peakValues[0];
    int minIndex = 0;
    for (int i = 1; i < peaksCount; i++)
    {
        if (minStoredPeak > peakValues[i])
            minStoredPeak = peakValues[minIndex = i];
    }

    for (int i = peaksCount; i < length; i++)
    {
        if (minStoredPeak < values[i + index])
        {
            // replace the min peaked value with bigger one
            peakValues[minIndex] = values[peakIndices[minIndex] = i + index];

            // and find min peaked value again
            minStoredPeak = peakValues[minIndex = 0];
            for (int j = 1; j < peaksCount; j++)
            {
                if (minStoredPeak > peakValues[j])
                    minStoredPeak = peakValues[minIndex = j];
            }
        }
    }
    delete[] peakValues;
    return peakIndices;
}
void ScanSignalIntervals(double* x, int index, int length, int intervalMin,
                         int intervalMax, int& optimalInterval, double& optimalValue,
                         int* peakIndices)
{
    optimalValue = DBL_MAX;
    optimalInterval = 0;

    // distance between min and max range value can be big
    // limiting it to the fixed value
    const int MaxAmountOfSteps = 30;
    int steps = intervalMax - intervalMin;
    if (steps > MaxAmountOfSteps)
    {
        steps = MaxAmountOfSteps;
    }
    else if (steps <= 0)
    {
        steps = 1;
    }

    // trying all intervals in the range to find one with
    // smaller difference in signal waves
    for (int i = 0; i < steps; i++)
    {
        int interval = intervalMin + (intervalMax - intervalMin) * i / steps;

        double sum = 0;
        for (int j = 0; j < length; j++)
        {
            double diff = x[index + j] - x[index + j + interval];
            sum += diff * diff;
        }
        if (optimalValue > sum)
        {
            optimalValue = sum;
            optimalInterval = interval;
        }
    }
}

int findMaxPeak(double* values, int values_length, int* peak, int peak_length)
{
    int max_index = 0;
    double max_value = 0;
    for (int i = 0; i < peak_length; i++)
    {
        double sum = 0;
        int peak_index = peak[i];
        for (int j = 1; j * peak_index < values_length; j++)
        {
            sum = sum + values[j * peak_index];
        }
        for (int j = 1; j * peak_index * 2 < values_length; j++)
        {
            sum = sum + values[j * peak_index * 2];
        }
        for (int j = 1; j * peak_index * 3 < values_length; j++)
        {
            sum = sum + values[j * peak_index * 3];
        }
        if (sum > max_value)
        {
            max_value = sum;
            max_index = peak_index;
        }
    }
    return max_index;
}

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
                                double minFreq, double maxFreq, double& result)
{
    double* spectr = Calculate(x, length);
    int usefullMinSpectr = qMax(0, (int)(minFreq * length / sampleRate));
    int usefullMaxSpectr = qMin(length,
                                (int)(maxFreq * length / sampleRate) + 1);

    // find peaks in the FFT frequency bins
    const int PeaksCount = 5;
    int* peakIndices = FindPeaks(spectr, length, usefullMinSpectr, PeaksCount);
    for (int i = 0; i < PeaksCount; i++)
    {
        //   std::cout<<peakIndices[i]<<" ";
    }
    // std::cout<<std::endl;
    for (int i = 0; i < PeaksCount; i++)
    {
        if (peakIndices[i] == usefullMinSpectr)
            return 0;
    }
    result = findMaxPeak(spectr, length, peakIndices, PeaksCount);
    // select fragment to check peak values: data offset
    const int verifyFragmentOffset = 0;
    // ... and half length of data
    int verifyFragmentLength = (int)(sampleRate / minFreq);

    // trying all peaks to find one with smaller difference value
    double minPeakValue = DBL_MAX;
    int minPeakIndex = 0;
    int minOptimalInterval = 0;
    for (int i = 0; i < PeaksCount; i++)
    {
        int index = peakIndices[i];
        int binIntervalStart = length / (index + 1), binIntervalEnd = length
                               / index;
        int interval;
        double peakValue;
        // scan bins frequencies/intervals
        ScanSignalIntervals(x, verifyFragmentOffset, verifyFragmentLength,
                            binIntervalStart, binIntervalEnd, interval, peakValue,
                            peakIndices);
        //  std::cout<<"";
        if (peakValue < minPeakValue)
        {
            minPeakValue = peakValue;
            minPeakIndex = index;
            minOptimalInterval = interval;
        }
    }
    delete[] spectr;
    delete[] peakIndices;
    // std::cout<<endl<<"minOptimalInterval:"<<minOptimalInterval<<"";
    return (double) sampleRate / minOptimalInterval;
}
void cutNotPeak(double* values, int index, int length)
{
    int* t = new int[length] {0};
    for (int i = index; i < length - 1; i++)
    {
        if (values[i] > values[i + 1] && values[i] > values[i - 1])
        {
            t[i] = 1;
        }
        else
        {
            t[i] = 0;
        }
    }
    for (int i = index; i < length - 1; i++)
    {
        values[i] = t[i] * values[i];
    }
    if (values[length - 1] < values[length - 2])
    {
        values[length - 1] = 0;
    }
    delete[] t;
}
void zxg(double* values, int start, int end, int length)
{
    //double* result =new double[length]{0};
    for (int i = start; i <= end; i++)
    {
        float sum = 0;
        for (int k = start; k < length; k++)
        {
            sum +=  values[k] * values[k - start];
        }
        // result[i] = sum;
        qDebug() << sum / (length - start) << " " << i;
    }
    // delete[] result;
}
