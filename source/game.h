#ifndef GAME
#define GAME
#include <QObject>
#include <QQmlListProperty>
#include <QTimer>
#include <QSound>
#include <QDebug>
#include <map>
#include "item.h"


enum DIRECTION
{
    RIGHT = 0,
    DOWN = 1,
    LEFT = 2,
    UP = 3,
    NONE = 4,
};

class Game : public QObject
{
    Q_OBJECT

public:
    Game(QObject *parent = 0);

    Q_PROPERTY(int width READ getWidth CONSTANT)
    virtual int getWidth() const {return m_width;}

    Q_PROPERTY(int height READ getHeight CONSTANT)
    virtual int getHeight() const {return m_height;}

    typedef QQmlListProperty<AnimatedItem> QPlayerList;
    typedef QList<AnimatedItem*> ItemList;
    typedef QList<GraphicItem*> GItemList;
    typedef QList<PartedItem*> PItemList;

    Q_PROPERTY(QQmlListProperty<AnimatedItem> player READ getPlayer NOTIFY playerChanged)
    QQmlListProperty<AnimatedItem> getPlayer() {return QQmlListProperty<AnimatedItem>(this, m_player);}

    Q_PROPERTY(QQmlListProperty<AnimatedItem> enemy READ getEnemy NOTIFY enemyChanged)
    QQmlListProperty<AnimatedItem> getEnemy() {return QQmlListProperty<AnimatedItem>(this, m_enemy);}

    Q_PROPERTY(QQmlListProperty<AnimatedItem> explosion READ getExplosion NOTIFY explosionChanged)
    QQmlListProperty<AnimatedItem> getExplosion() {return QQmlListProperty<AnimatedItem>(this, m_explosion);}

    Q_PROPERTY(QQmlListProperty<GraphicItem> bullet READ getBullet NOTIFY bulletChanged)
    QQmlListProperty<GraphicItem> getBullet() {return QQmlListProperty<GraphicItem>(this, m_bullet);}

    Q_PROPERTY(QQmlListProperty<GraphicItem> base READ getBase NOTIFY baseChanged)
    QQmlListProperty<GraphicItem> getBase() {return QQmlListProperty<GraphicItem>(this, m_base);}

    Q_PROPERTY(QQmlListProperty<GraphicItem> frags READ getFrags NOTIFY fragsChanged)
    QQmlListProperty<GraphicItem> getFrags() {return QQmlListProperty<GraphicItem>(this, m_frags);}

    Q_PROPERTY(QQmlListProperty<PartedItem> brick READ getBrick NOTIFY brickChanged)
    QQmlListProperty<PartedItem> getBrick() {return QQmlListProperty<PartedItem>(this, m_brick);}

    Q_PROPERTY(QQmlListProperty<PartedItem> concrete READ getConcrete NOTIFY concreteChanged)
    QQmlListProperty<PartedItem> getConcrete() {return QQmlListProperty<PartedItem>(this, m_concrete);}

    Q_PROPERTY(int direction READ getDirection WRITE setDirection NOTIFY directionChanged)
    int getDirection(){return m_directionMap[0];}
    void setDirection(int direction)
    {
        if(direction < 0 || direction > 4)
            m_directionMap[0] = NONE;
        else
            m_directionMap[0] = static_cast<DIRECTION>(direction);
    }
    Q_PROPERTY(int controlKey READ getControlKey WRITE setControlKey NOTIFY controlKeyChanged)
    int getControlKey(){return m_controlKey;}
    void setControlKey(int key){m_controlKey = key;}

    Q_PROPERTY(int lifesCount READ getLifesCount NOTIFY lifesCountChanged)
    int getLifesCount(){return m_lifesCounter;}

    Q_PROPERTY(int fragsCount READ getFragsCount NOTIFY fragsCountChanged)
    int getFragsCount(){return m_fragsCounter;}

    Q_PROPERTY(int maxFragsCount READ getMaxFragsCount NOTIFY maxFragsCountChanged)
    int getMaxFragsCount(){return m_maxFragsCount;}

    Q_PROPERTY(QString message READ getMessage WRITE setMessage NOTIFY messageChanged)
    QString getMessage(){return m_message;}
    void setMessage(const QString & msg) {m_message = msg; emit messageChanged();}
public slots:
    void move();

signals:
    void playerChanged();
    void directionChanged();
    void enemyChanged();
    void bulletChanged();
    void controlKeyChanged();
    void explosionChanged();
    void brickChanged();
    void concreteChanged();
    void baseChanged();
    void lifesCountChanged();
    void fragsCountChanged();
    void maxFragsCountChanged();
    void fragsChanged();
    void messageChanged();

private:
    void moveItem(AnimatedItem * item, int speed);
    void moveBullet(GraphicItem * item, int speed);
    bool checkBorders(GraphicItem * item);
    void initGame();

    void think();

    void createBullet(GraphicItem * item);
    void createExplosion(GraphicItem * item);
    void createBrick(int x,int y);
    void createConcrete(int x,int y);
    void createBase(int x, int y);
    void createPlayer(int x, int y);
    void createEnemy(int x, int y);
    void createOrUpdateFrags(int count);
    void destroyBullet(GraphicItem * bullet);


    int checkWay(AnimatedItem * item);

    int checkX(int x);
    int checkY(int y);
    int getDistance(GraphicItem * current, GraphicItem * other);
    int getPartedDistance(GraphicItem *current, PartedItem *brick);

    std::map<int,DIRECTION> m_directionMap;
    std::map<int,int> m_explosionMap;
    QObject * m_parent;
    ItemList m_player;
    ItemList m_enemy;
    ItemList m_explosion;
    GItemList m_bullet;
    GItemList m_base;
    GItemList m_frags;
    PItemList m_brick;
    PItemList m_concrete;
    int m_objectCounter;
    int m_controlKey;
    int m_loopCounter;
    int m_lifesCounter;
    int m_fragsCounter;
    int m_maxFragsCount;
    int m_width;
    int m_height;
    bool m_canMove;
    int m_lastPlayerFire;
    QSound m_move_snd;
    QString m_message;
    std::vector<std::pair<int,int>> m_enemyRespawnList;
    std::pair<int,int> m_playerRespawn;
};


#endif // GAME

