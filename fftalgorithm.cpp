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



void cutNotPeak(double* values, int index, int length)
{
    int* t = new int[length];
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
std::vector<Peak> findPeaks(double* values, int start, int end)
{
    std::vector<Peak> v;
    for (int i = start; i <= end; i++)
    {
        if (values[i] > values[i - 1] && values[i] > values[i + 1])
        {
            v.push_back(Peak(i, values[i]));
        }
    }
    return v;
}
bool greaterSort(Peak a, Peak b)
{
    return (a.value > b.value);
}
std::vector<Peak> sortPeaks(std::vector<Peak> peaks)
{
    std::sort(peaks.begin(), peaks.end(), greaterSort);
    return peaks;
}
std::vector<Peak>  getUsefullPeaks(double* mydata)
{
    double* result = Calculate(mydata, AudioInfo::N);
    int maxLength = 5000 / (AudioInfo::sampleRate / (AudioInfo::N + 0.0));
    int minLength = 200 / (AudioInfo::sampleRate / (AudioInfo::N + 0.0));
    return sortPeaks(findPeaks(result, minLength, maxLength));
}
void cacIsRight(double* mydata, std::vector<YFData*> currentYf, quint32 maxValue)
{
    if (currentYf.empty())
    {
        return;
    }
    if (maxValue < 1000)
    {
        for (unsigned long j = 0; j < currentYf.size(); j++)
        {
            YFData*  yfd = currentYf[j];
            //错误
            if (yfd->result != 2 && yfd->result != 3)
            {
                yfd->color =  QVector4D(1, 0, 0, 1);
                yfd->result = 3;
            }
            return;
        }
    }
    std::vector<Peak> peaks = getUsefullPeaks(mydata);
    unsigned long size = peaks.size() > 5 ? 5 : peaks.size();
    initIsDouble(peaks, size * 2);
    qDebug() << "+++++++++++++++++++++++++++++++++";
    for (unsigned long j = 0; j < size; j++)
    {
        qDebug() << peaks[j].index << " " << peaks[j].value << " " << AudioInfo::sampleRate* peaks[j].index / (AudioInfo::N) << " " << sqrt(peaks[j].value) << " " << peaks[j].isDouble;
    }
    for (unsigned long j = 0; j < currentYf.size(); j++)
    {
        YFData*  yfd = currentYf[j];
        double key = levelCData[yfd->musicLevel];
        for (unsigned long i = 0; i < size; i++)
        {
            if (!peaks[i].isDouble)
            {
                continue;
            }
            //计算频率
            double f = AudioInfo::sampleRate * peaks[i].index / (AudioInfo::N);
            //偏差在0.5%以内
            if (abs(f - key) / key < 0.01)
            {
                qDebug() << key << " " << "正确" << f << " " << " " << peaks[i].index << " " << peaks[i].value;
                if (yfd->result != 2)
                {
                    yfd->result = 2;
                    yfd->color = QVector4D(0, 1, 0, 1);
                }
                continue;
            }
        }
        //错误
        if (yfd->result != 2)
        {
            yfd->color =  QVector4D(1, 0, 0, 1);
            yfd->result = 3;
            qDebug() << "错误" << key;
        }
    }

}
void zxg(double* values, int start, int end, int length)
{
    for (int i = start; i <= end; i++)
    {
        float sum = 0;
        for (int k = start; k < length; k++)
        {
            sum +=  values[k] * values[k - start];
        }
        qDebug() << sum / (length - start) << " " << i;
    }
}
void initIsDouble(std::vector<Peak>& peaks, unsigned long size)
{
    for (unsigned long i = 0; i < size; i++)
    {
        for (unsigned long k = i; k < size; k++)
        {
            if (abs((peaks)[i].index - (peaks)[k].index / 2) < 2 || abs((peaks)[i].index / 2 - (peaks)[k].index) < 2 || abs((peaks)[i].index - (peaks)[k].index / 3) < 2 || abs((peaks)[i].index / 3 - (peaks)[k].index) < 2 || abs((peaks)[i].index - (peaks)[k].index / 4) < 2 || abs((peaks)[i].index / 4 - (peaks)[k].index) < 2)
            {
                (peaks)[i].isDouble = true;
                (peaks)[k].isDouble = true;
            }
        }
    }
}
