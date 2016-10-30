#ifndef ITEM
#define ITEM
#include <QObject>
#include <QQmlListProperty>

class GraphicItem : public QObject
{
    Q_OBJECT
    const int defaultWidth = 32;
    const int defaultHeight = 32;
public:
    explicit GraphicItem(QObject *parent = 0):QObject(parent){}

    Q_PROPERTY(int objectId READ getObjectId CONSTANT)
    virtual int getObjectId() const {return m_objectId;}
    virtual void setObjectId(int objectId){ m_objectId = objectId;}

    Q_PROPERTY(QString objectImagePath READ getObjectImagePath NOTIFY imagePathChanged)
    virtual QString getObjectImagePath() const{return m_objectImagePath;}
    virtual void setObjectImagePath(const QString& path) {m_objectImagePath = path; emit imagePathChanged();}

    Q_PROPERTY(int positionX READ getPositionX WRITE setPositionX NOTIFY positionXChanged)
    virtual int getPositionX() const {return m_positionX;}
    virtual void setPositionX(int posX) {m_positionX = posX;}

    Q_PROPERTY(int positionY READ getPositionY WRITE setPositionY NOTIFY positionYChanged)
    virtual int getPositionY() const {return m_positionY;}
    virtual void setPositionY(int posY) {m_positionY = posY;}

    Q_PROPERTY(int rotation READ getRotation WRITE setRotation NOTIFY rotationChanged)
    virtual int getRotation() const {return m_rotation;}
    virtual void setRotation(int rotation) {m_rotation = rotation;}

    void updatePositionAndRotation(int positionX, int positionY, int rotation)
    {
        m_positionX = positionX;
        m_positionY = positionY;
        m_rotation = rotation;
        emit positionXChanged(m_positionX);
        emit positionYChanged(m_positionY);
        emit rotationChanged(m_rotation);
    }

    Q_PROPERTY(int width READ getWidth CONSTANT)
    virtual int getWidth() const {return defaultWidth;}

    Q_PROPERTY(int height READ getHeight CONSTANT)
    virtual int getHeight() const {return defaultHeight;}

signals:
    // Signals are sent when corresponding changes should be propagated to QML
    void positionXChanged(int);
    void positionYChanged(int);
    void rotationChanged(int);
    void visibleChanged(bool);
    void imagePathChanged();

private:
    QString m_objectImagePath;
    int m_objectId;
    int m_rotation;
    int m_positionX;
    int m_positionY;
};

class AnimatedItem : public GraphicItem
{
    Q_OBJECT
public:
    explicit AnimatedItem(QObject *parent = 0) : GraphicItem(parent), m_isPaused(false){}
    Q_PROPERTY(bool animationPaused READ getAnimationPaused WRITE setAnimationPaused NOTIFY animationPausedChanged)
    virtual bool getAnimationPaused() {return m_isPaused;}
    virtual void setAnimationPaused(bool isPaused) {m_isPaused = isPaused;}
signals:
    void animationPausedChanged(bool);
private:
    bool m_isPaused;
};

class PartedItem : public GraphicItem
{
    Q_OBJECT
public:
    explicit PartedItem(int rows, int cols, QObject *parent = 0) : GraphicItem(parent), m_rows(rows), m_cols(cols)
    {
        m_partWidth = getWidth()/m_cols;
        m_partHeight = getHeight()/m_rows;
        emit partWidthChanged();
        emit partHeigthChanged();
    }
    Q_PROPERTY(QVariantList part READ getPart NOTIFY partChanged)
    QVariantList getPart() {
        QVariantList list;
        foreach (int var, m_part) {
            list.push_back(var);
        }
        return list;
    }
    QList<int> m_part;
    Q_PROPERTY(int rows READ getRows CONSTANT)
    virtual int getRows() const {return m_rows;}

    Q_PROPERTY(int cols READ getCols CONSTANT)
    virtual int getCols() const {return m_cols;}

    Q_PROPERTY(int partWidth READ getPartWidth CONSTANT)
    virtual int getPartWidth() const {return m_partWidth;}

    Q_PROPERTY(int partHeight READ getPartHeight CONSTANT)
    virtual int getPartHeight() const {return m_partHeight;}



signals:
    void partChanged();
    void partWidthChanged();
    void partHeigthChanged();

private:
    int m_rows;
    int m_cols;
    int m_partWidth;
    int m_partHeight;
};

class IconItem : public GraphicItem
{
public:
    explicit IconItem(QObject *parent = 0) : GraphicItem(parent){}
    virtual int getWidth() const {return 16;}
    virtual int getHeight() const {return 16;}
};
#endif // ITEM

