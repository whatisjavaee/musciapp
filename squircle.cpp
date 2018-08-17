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
}
void Squircle::stop()
{
    timeLine->stop();
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
    m_renderer->SPEED = m_sp;
    m_renderer->setWindow(window());
}

void SquircleRenderer::paint()
{
    if (!m_program)
    {
        initializeOpenGLFunctions();
        //设置视图、清理背景
        glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
        glDisable(GL_DEPTH_TEST);
    }
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    //绘制五线
    float lines[]
    {
        -1, 2.0f / 25 * 10 - 1, 1, 2.0f / 25 * 10 - 1,
            - 1, 2.0f / 25 * 11 - 1, 1, 2.0f / 25 * 11 - 1,
            - 1, 2.0f / 25 * 12 - 1, 1, 2.0f / 25 * 12 - 1,
            - 1, 2.0f / 25 * 13 - 1, 1, 2.0f / 25 * 13 - 1,
            - 1, 2.0f / 25 * 14 - 1, 1, 2.0f / 25 * 14 - 1
        };
    this->drawSingleColor(lines, GL_LINES, 0, 10, QVector4D(0, 0, 0, 1));

    //绘制条子
    double time =  m_t;
    std::list<YFData*>::iterator it;
    for (it = yFDataS.begin(); it != yFDataS.end(); it++)
    {
        YFData* yfd = *it;
        //新增
        int maxTime = yfd->orderTime + yfd->time + m_viewportSize.width() * SPEED;
        if (maxTime >= time && yfd->orderTime <= time)
        {
            yfd->result = 0;
            float timeIn = time - yfd->orderTime;
            float timeJl = timeIn / SPEED;
            float x1 = 1 - timeJl * (2.0 / m_viewportSize.width());
            float x2 = 1 - timeJl * (2.0 / m_viewportSize.width()) + yfd->time / SPEED * (2.0 / m_viewportSize.width());
            x2 = x2 > 1 ? 1 : x2;
            float y = yfd->musicLevel * (2.0f / 25) / 2 - (2.0f / 25) * 5 ;
            float t[] =
            {
                x1, y, x2, y,  x1, y - 2.0f / 25, x2, y - 2.0f / 25
            };
            if (yfd->color == NULL)
            {
                yfd->color = new QVector4D(qrand() % 11 / 10.0, qrand() % 11 / 10.0, qrand() % 11 / 10.0, 1);
            }
            this->drawSingleColor(t, GL_TRIANGLE_STRIP, 0, 4, *yfd->color);
        }
    }

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}

void SquircleRenderer::drawSingleColor(const void* values, GLenum mode, GLint first, GLsizei count, QVector4D color)
{
    if (!m_singleColorProgram)
    {
        m_singleColorProgram = new QOpenGLShaderProgram();
        m_singleColorProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl.vert");
        m_singleColorProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/simpleColor.frag");
        m_singleColorProgram->bindAttributeLocation("vertices", 0);
        m_singleColorProgram->link();
    }
    m_singleColorProgram->bind();
    m_singleColorProgram->enableAttributeArray(0);
    m_singleColorProgram->setAttributeArray(0, GL_FLOAT, values, 2);
    m_singleColorProgram->setUniformValue("color", color);
    glDrawArrays(mode, first, count);
    m_singleColorProgram->disableAttributeArray(0);
    m_singleColorProgram->release();
}
