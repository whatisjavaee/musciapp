#ifndef YF_H
#define YF_H
#include <math.h>
#include <QVector4D>
class YFData
{
public:
    //key
    int musicLevel = 0;
    //paishu
    int musicTime = 0;
    //time
    int time = 0;
    //init
    int orderTime = 0;
    //初始化-1，绘制0，线上 1 正确 2 错误 3
    int result = -1;
    int rightCount = 0;
    QVector4D color;
    static void initTime(std::vector<YFData*> yFDataS, int paishu)
    {
        int orderTime = 0;
        for (unsigned long i = 0; i<yFDataS.size(); i++)
        {
            YFData* yfd = yFDataS[i];
            yfd->time = yfd->musicTime * (1000 * 60 / paishu);
            yfd->orderTime = orderTime;
            orderTime += yfd->time;
        }
    }

};
const double levelCData[50] =
{
    0, 0,
    220.0,
    220 * pow(2, 2.0 / 12),
    220 * pow(2, 3.0 / 12),
    220 * pow(2, 5.0 / 12),
    220 * pow(2, 7.0 / 12),
    220 * pow(2, 8.0 / 12),
    220 * pow(2, 10.0 / 12),
    440.0,
    440 * pow(2, 2.0 / 12),
    440 * pow(2, 3.0 / 12),
    440 * pow(2, 5.0 / 12),
    440 * pow(2, 7.0 / 12),
    440 * pow(2, 8.0 / 12),
    440 * pow(2, 10.0 / 12),
    880,
    880 * pow(2, 2.0 / 12),
    880 * pow(2, 3.0 / 12),
    880 * pow(2, 5.0 / 12),
    880 * pow(2, 7.0 / 12),
    880 * pow(2, 8.0 / 12),
    880 * pow(2, 10.0 / 12),
    1760,
    1760 * pow(2, 2.0 / 12),
    1760 * pow(2, 3.0 / 12),
    1760 * pow(2, 5.0 / 12),
    1760 * pow(2, 7.0 / 12),
    1760 * pow(2, 8.0 / 12),
    1760 * pow(2, 10.0 / 12),
    3520,
    3520 * pow(2, 2.0 / 12),
    3520 * pow(2, 3.0 / 12),
    3520 * pow(2, 5.0 / 12),
    3520 * pow(2, 7.0 / 12),
    3520 * pow(2, 8.0 / 12),
    3520 * pow(2, 10.0 / 12)
};

#endif // YF_H
