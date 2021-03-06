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

#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QQuickItem>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QTime>
#include <QSize>
#include <QTimeLine>
#include <QAudioDeviceInfo>
#include "yf.h"
#include "audioinput.h"
#include<iostream>
#include <mutex>
#include <string>
class SquircleRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    SquircleRenderer() : m_t(0), m_program(0)
    {
        for (int i = 0; i < 200; i++)
        {
            YFData* obj1 = new YFData();
            //obj1->musicLevel = qrand() % 25;
            obj1->musicLevel = 9 + qrand() % 10;
            //obj1->musicLevel = 11;
            obj1->musicTime = qrand() % 4 + 1;
            yFDataS.push_back(obj1);
        }
        YFData::initTime(yFDataS, 60);
    }
    ~SquircleRenderer();

    void setT(qreal t)
    {
        m_t = t;
    }
    void setViewportSize(const QSize& size)
    {
        m_viewportSize = size;
    }
    void setWindow(QQuickWindow* window)
    {
        m_window = window;
    }
    void drawSingleColor(const void* values, GLenum mode, GLint first, GLsizei count, QVector4D color);
public slots:
    void paint();
private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram* m_program;
    QQuickWindow* m_window;
    //单色着色器
    QOpenGLShaderProgram* m_singleColorProgram = NULL;
    std::mutex mutex;

public:
    float SPEED = 20;
    std::vector<YFData*> yFDataS;
    std::vector<Peak> peaks;
};

class Squircle : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal sp READ sp WRITE setSp NOTIFY spChange)
    Q_PROPERTY(qreal rightNum READ rightNum NOTIFY rightChange)
    Q_PROPERTY(qreal errorNum READ errorNum NOTIFY errorChange)

public:
    Squircle();
    ~Squircle()
    {
        delete timeLine;
    }
    qreal t() const
    {
        return m_t;
    }
    void setT();
    qreal sp() const
    {
        return m_sp;
    }
    qreal rightNum() const
    {
        return m_right;
    }
    qreal errorNum() const
    {
        return m_error;
    }
    void setSp(qreal sp);

signals:
    void tChanged();
    void spChange();
    void rightChange();
    void errorChange();
public slots:
    void sync();
    void cleanup();
    void start();
    void stop();
    void dataInput(double* mydata,quint32 maxValue);
private slots:
    void handleWindowChanged(QQuickWindow* win);

private:
    qreal m_t;
    qreal m_sp = 60.0;
    qreal m_right =0;
    qreal m_error =0;
    SquircleRenderer* m_renderer = NULL;
    QTimeLine* timeLine = NULL;
    QAudioDeviceInfo m_device;
    AudioInfo* m_audioInfo = NULL;
    QAudioInput* m_audioInput = NULL;
    QAudioFormat m_format;
};

#endif // SQUIRCLE_H
