#include "squircle.h"
#include <QQuickWindow>

Squircle::Squircle()
    : m_t(0)
    , m_renderer(0)
{
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
    timeLine = new QTimeLine(1000 * 60 * 30.0);
    timeLine->setUpdateInterval(17);
    this->connect(timeLine, &QTimeLine::valueChanged, this, &Squircle::setT);

    m_device = QAudioDeviceInfo::defaultInputDevice();
    m_format.setSampleRate(AudioInfo::sampleRate);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");
    if (!m_device.isFormatSupported(m_format))
    {
        qWarning() << "Default format not supported, trying to use the nearest.";
        m_format = m_device.nearestFormat(m_format);
    }
    m_audioInfo = new AudioInfo(m_format, this);
    connect(m_audioInfo, SIGNAL(update(double*,quint32)), SLOT(dataInput(double*,quint32)));
    m_audioInput = new QAudioInput(m_device, m_format, this);
}

void Squircle::setT()
{
    m_t = timeLine->currentTime();
    emit tChanged();
    if (window())
        window()->update();
}
void Squircle::setSp(qreal sp)
{
    if (sp == m_sp)
        return;
    m_sp = sp;
    if (m_renderer != NULL)
    {
        YFData::initTime(m_renderer->yFDataS, m_sp);
        qDebug() << m_sp;
    }
    emit spChange();
}
void Squircle::handleWindowChanged(QQuickWindow* win)
{
    if (win)
    {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &Squircle::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &Squircle::cleanup, Qt::DirectConnection);

        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void Squircle::cleanup()
{
    if (m_renderer)
    {
        delete m_renderer;
        m_renderer = 0;
    }
}
void Squircle::start()
{
    timeLine->start();
    m_audioInfo->start();
    m_audioInput->start(m_audioInfo);
}
void Squircle::stop()
{
    timeLine->stop();
    m_audioInfo->stop();
    m_audioInput->stop();
}
void Squircle::dataInput(double* mydata,quint32 maxValue)
{
    int time  = timeLine->currentTime();
    std::vector<YFData*> currentYfs;
    unsigned long yfSize = m_renderer->yFDataS.size();
    for (unsigned long i=0;i<yfSize;i++)
    {
        YFData* yfd = m_renderer->yFDataS[i];
        //新增
        int startTime = yfd->orderTime + (2 / 8.0 * 7)  * m_renderer->SPEED;
        int maxTime = startTime + yfd->time ;
        if (maxTime >= time && startTime <= time)
        {
            //正确继续
            if(yfd->result == 2){
                continue;
            }
            //状态改成线上
            if(yfd->result == 0){
                yfd->result = 1;
            }
            currentYfs.push_back(yfd);
        }
    }
    cacIsRight(mydata,currentYfs,maxValue);
    int sumError =0;
    int sumRight= 0;
    for(unsigned i =0;i<yfSize;i++){
         YFData* yfd = m_renderer->yFDataS[i];
         if(yfd->result == 3){
             sumError++;
         }else if(yfd->result == 2){
             sumRight++;
         }
    }
    if(sumError != m_error){
        qDebug()<<"errorChange";
        m_error = sumError;
        emit errorChange();
    }
    if(sumRight != m_right){
        qDebug()<<"errorChange";
        m_right = sumRight;
        emit rightChange();
    }
}


SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
    delete m_singleColorProgram;
}

void Squircle::sync()
{
    if (!m_renderer)
    {
        m_renderer = new SquircleRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &SquircleRenderer::paint, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    //每个像素需要的时间=每拍时间/每拍像素长度
    m_renderer->SPEED = (1000 * 60 / m_sp) / (2 / 60.0 * 4) ;
    m_renderer->setWindow(window());
}

void SquircleRenderer::paint()
{
    if (m_singleColorProgram == NULL)
    {
        mutex.lock();
        if(m_singleColorProgram == NULL){
            initializeOpenGLFunctions();
            m_singleColorProgram = new QOpenGLShaderProgram();
            m_singleColorProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl.vert");
            m_singleColorProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/simpleColor.frag");
            m_singleColorProgram->bindAttributeLocation("vertices", 0);
            m_singleColorProgram->link();
        }
        mutex.unlock();
    }
    //设置视图、清理背景
    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glDisable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //绘制五线
    float lines[]
    {
        -1, 2.0f / 25 * 10 - 1, 1, 2.0f / 25 * 10 - 1,
        - 1, 2.0f / 25 * 11 - 1, 1, 2.0f / 25 * 11 - 1,
        - 1, 2.0f / 25 * 12 - 1, 1, 2.0f / 25 * 12 - 1,
        - 1, 2.0f / 25 * 13 - 1, 1, 2.0f / 25 * 13 - 1,
        - 1, 2.0f / 25 * 14 - 1, 1, 2.0f / 25 * 14 - 1,
        1 / 4.0 - 1, 2.0f / 25 * 5 - 1, 1 / 4.0 - 1, 2.0f / 25 * 19 - 1
    };
    this->drawSingleColor(lines, GL_LINES, 0, 12, QVector4D(0, 0, 0, 1));

    //绘制条子
    double time =  m_t;
    for (unsigned long i = 0; i<yFDataS.size(); i++)
    {
        YFData* yfd = yFDataS[i];
        //新增
        int maxTime = yfd->orderTime + yfd->time + 2 * SPEED;
        if (maxTime >= time && yfd->orderTime <= time)
        {
            if (yfd->result == -1)
            {
                yfd->color = QVector4D(qrand() % 11 / 10.0, qrand() % 11 / 10.0, qrand() % 11 / 10.0, 1);
                yfd->result = 0;
            }
            float timeIn = time - yfd->orderTime;
            float timeJl = timeIn / SPEED;
            float x1 = 1 - timeJl;
            float x2 = 1 - timeJl  + yfd->time / SPEED;
            x2 = x2 > 1 ? 1 : x2;
            float y = yfd->musicLevel * (2.0f / 25) / 2 - (2.0f / 25) * 5 ;
            float t[] =
            {
                x1, y, x2, y,  x1, y - 2.0f / 25, x2, y - 2.0f / 25
            };
            this->drawSingleColor(t, GL_TRIANGLE_STRIP, 0, 4, yfd->color);
        }
    }

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}

void SquircleRenderer::drawSingleColor(const void* values, GLenum mode, GLint first, GLsizei count, QVector4D color)
{
    m_singleColorProgram->bind();
    m_singleColorProgram->enableAttributeArray(0);
    m_singleColorProgram->setAttributeArray(0, GL_FLOAT, values, 2);
    m_singleColorProgram->setUniformValue("color", color);
    glDrawArrays(mode, first, count);
    m_singleColorProgram->disableAttributeArray(0);
    m_singleColorProgram->release();
}
