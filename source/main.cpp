#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "item.h"
#include "game.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<GraphicItem>();
    qmlRegisterType<AnimatedItem>();
    qmlRegisterType<PartedItem>();

    Game* game = new Game(&app);


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gameBoard", game);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

