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

    int retCode = 0;

    try {
        QScopedPointer<Game> game(new Game(&app));


        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("gameBoard", game.data());
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        retCode = app.exec();
    }
    catch(const std::bad_alloc&) {
        // There is no references to external resources yet, so show message and exit
        qDebug("Not enough memory!");
    }
    return retCode;
}

