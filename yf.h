#ifndef YF_H
#define YF_H

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
#endif // YF_H
