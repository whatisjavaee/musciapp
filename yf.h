#ifndef YF_H
#define YF_H
#include <math.h>

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
    //初始化-1，绘制0，线上 1 正确 2 错误 2
    int result = -1;
    QVector4D* color;
    static void initTime(std::list<YFData*> yFDataS, int paishu)
    {
        int orderTime = 0;
        std::list<YFData*>::iterator it;
        for (it = yFDataS.begin(); it != yFDataS.end(); it++)
        {
            YFData* yfd = *it;
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
    220 * pow(2, 4.0 / 12),
    220 * pow(2, 5.0 / 12),
    220 * pow(2, 7.0 / 12),
    220 * pow(2, 9.0 / 12),
    220 * pow(2, 11.0 / 12),
    440.0,
    440 * pow(2, 2.0 / 12),
    440 * pow(2, 4.0 / 12),
    440 * pow(2, 5.0 / 12),
    440 * pow(2, 7.0 / 12),
    440 * pow(2, 9.0 / 12),
    440 * pow(2, 11.0 / 12),
    880,
    880 * pow(2, 2.0 / 12),
    880 * pow(2, 4.0 / 12),
    880 * pow(2, 5.0 / 12),
    880 * pow(2, 7.0 / 12),
    880 * pow(2, 9.0 / 12),
    880 * pow(2, 11.0 / 12),
    1760,
    1760 * pow(2, 2.0 / 12),
    1760 * pow(2, 4.0 / 12),
    1760 * pow(2, 5.0 / 12),
    1760 * pow(2, 7.0 / 12),
    1760 * pow(2, 9.0 / 12),
    1760 * pow(2, 11.0 / 12),
    3520,
    3520 * pow(2, 2.0 / 12),
    3520 * pow(2, 4.0 / 12),
    3520 * pow(2, 5.0 / 12),
    3520 * pow(2, 7.0 / 12),
    3520 * pow(2, 9.0 / 12),
    3520 * pow(2, 11.0 / 12)
};
#endif // YF_H
