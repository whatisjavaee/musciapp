/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "squircle.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>


Squircle::Squircle()
    : m_t(0)
    , m_renderer(0)
{
    connect(this, &QQuickItem::windowChanged, this, &Squircle::handleWindowChanged);
}

void Squircle::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
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

SquircleRenderer::~SquircleRenderer()
{
    delete m_program;
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
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    float values[] =
    {
        -0.5, -0.5,
            -0.5, 0.5,
            0.5, 0.5,
            0.5, -0.5,
            1, 1,
            0.5, 1,
            0.5, 0.5,
            1, 0.5
        };

    drawSingleColor(values, GL_QUADS, 0, 8, QVector4D(m_t, 0, 0, m_t));

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    m_window->resetOpenGLState();
}

void SquircleRenderer::drawSingleColor(const void* values, GLenum mode, GLint first, GLsizei count, QVector4D color)
{
    if (!m_singleColorProgram)
    {
        m_singleColorProgram = new QOpenGLShaderProgram();
        m_singleColorProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "C:\\Users\\tangxuegui\\Documents\\musciapp\\gl.vert");
        m_singleColorProgram->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "C:\\Users\\tangxuegui\\Documents\\musciapp\\simpleColor.frag");
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
