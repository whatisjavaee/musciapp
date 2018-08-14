#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "squircle.h"
int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    qDebug() << "exe";
    return app.exec();
}
