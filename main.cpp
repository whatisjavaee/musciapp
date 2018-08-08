#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "filelineplayer.h"
int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<FileLinePlayer>("FileLinePlayer", 1, 0, "FileLinePlayer");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
