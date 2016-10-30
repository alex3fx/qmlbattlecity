#include <Qt>
#include <QFile>
#include <QSound>
#include "game.h"


Game::Game(QObject *parent)
    : m_parent(parent)
    , m_objectCounter(10000000)
    , m_loopCounter(0)
    , m_lifesCounter(3)
    , m_fragsCounter(0)
    , m_maxFragsCount(20)
    , m_canMove(true)
    , m_lastPlayerFire(0)
    , m_move_snd(":/sounds/userMoving.wav")
{
    initGame();
    m_move_snd.setLoops(QSound::Infinite);

    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this, SLOT(move()));
    timer->start(50);
}

void Game::move(){
    think();
    if(m_directionMap[m_player.at(0)->getObjectId()] == NONE)
        m_move_snd.stop();
    else if(m_move_snd.isFinished())
        m_move_snd.play();
    if(m_canMove)
        moveItem(m_player.at(0), 5);
    for (auto enemy : m_enemy)
    {
        moveItem(enemy,5);
    }
    for (auto bullet : m_bullet)
    {
        moveBullet(bullet, 10);
    }
}

int Game::checkX(int x)
{
    if(x<0) return 0;
    if(x>(m_width - 32)) return (m_height - 32);
    return x;
}

int Game::checkY(int y)
{
    if(y<0) return 0;
    if(y>(m_height - 32)) return (m_height - 32);
    return y;
}

void Game::moveItem(AnimatedItem *item, int speed)
{
    if(item)
    {
        item->setAnimationPaused(false);
        speed = std::min(speed, checkWay(item));
        switch (m_directionMap[item->getObjectId()]) {
        case UP:
            item->updatePositionAndRotation(checkX(item->getPositionX()),checkY(item->getPositionY()-speed), 0);
            break;
        case DOWN:
            item->updatePositionAndRotation(checkX(item->getPositionX()),checkY(item->getPositionY()+speed), 180);
            break;
        case LEFT:
            item->updatePositionAndRotation(checkX(item->getPositionX()-speed),checkY(item->getPositionY()), 270);
            break;
        case RIGHT:
            item->updatePositionAndRotation(checkX(item->getPositionX()+speed),checkY(item->getPositionY()), 90);
            break;
        default:
            item->setAnimationPaused(true);
            break;
        }
    }
}

void Game::moveBullet(GraphicItem *item, int speed)
{
    if(item)
    {

        if(!checkBorders(item))
        {
            createExplosion(item);
            destroyBullet(item);
            return;
        }
        switch (m_directionMap[item->getObjectId()]) {
        case UP:
            item->updatePositionAndRotation(item->getPositionX(),item->getPositionY()-speed, 0);
            break;
        case DOWN:
            item->updatePositionAndRotation(item->getPositionX(),item->getPositionY()+speed, 180);
            break;
        case LEFT:
            item->updatePositionAndRotation(item->getPositionX()-speed,item->getPositionY(), 270);
            break;
        case RIGHT:
            item->updatePositionAndRotation(item->getPositionX()+speed,item->getPositionY(), 90);
            break;
        default:
            break;
        }
    }
}

bool Game::checkBorders(GraphicItem *item)
{
    DIRECTION direction = m_directionMap[item->getObjectId()];
    switch(direction)
    {
    case UP:
        if(item->getPositionY()<=0) return false;
        break;
    case DOWN:
        if(item->getPositionY()>=(m_height - item->getHeight())) return false;
        break;
    case LEFT:
        if(item->getPositionX()<=0) return false;
        break;
    case RIGHT:
        if(item->getPositionX()>=(m_width - item->getWidth())) return false;
        break;
    default:
        break;
    }
    return true;
}

void Game::initGame()
{
    createOrUpdateFrags(m_maxFragsCount);
    std::vector<std::string> map;
    //e - empty
    //w - enemy respawn
    //b - brick
    //1 - right half brick
    //2 - left half brick
    //3 - up half brick
    //4 - down half brick
    //9 - left down quarter
    //0 - right down quarter
    //c - concrete
    //5 - right half concrete
    //6 - left half concrete
    //7 - up half concrete
    //8 - down half concrete
    //p - player
    //f - base
    map.push_back("weeeeeweeeeew");
    map.push_back("ebebebebebebe");
    map.push_back("ebebebebebebe");
    map.push_back("ebebebcbebebe");
    map.push_back("ebebe3e3ebebe");
    map.push_back("e3e3e4e4e3e3e");
    map.push_back("4e44e3e3e44e4");
    map.push_back("7e33e4e4e33e7");
    map.push_back("e4e4ebbbe4e4e");
    map.push_back("ebebebebebebe");
    map.push_back("ebebe3e3ebebe");
    map.push_back("ebebe940ebebe");
    map.push_back("eeeep2f1eeeee");
    int x = 0;
    int y = 0;
    for(auto str : map)
    {
        x=0;
        for(auto ch: str)
        {
            switch(ch)
            {
            case'w':
                m_enemyRespawnList.push_back(std::make_pair(x,y));
                break;
            case'b':
                createBrick(x,y);
                break;
            case '1':
                createBrick(x,y);
            {
                PartedItem * item = m_brick.last();
                for(int i = 0; i<4;++i)
                {
                    item->m_part[4*i+2] = 1;
                    item->m_part[4*i+3] = 1;
                }
            }
                break;
            case '2':
                createBrick(x,y);
            {
                PartedItem * item = m_brick.last();
                for(int i = 0; i<4;++i)
                {
                    item->m_part[4*i] = 1;
                    item->m_part[4*i+1] = 1;
                }
            }
                break;
            case '3':
                createBrick(x,y);
            {
                PartedItem * item = m_brick.last();
                for(int i = 0; i<8;++i)
                {
                    item->m_part[8+i] = 1;
                }
            }
                break;
            case '4':
                createBrick(x,y);
            {
                PartedItem * item = m_brick.last();
                for(int i = 0; i<8;++i)
                {
                    item->m_part[i] = 1;
                }
            }
                break;
            case '9':
                createBrick(x,y);
            {
                PartedItem * brick = m_brick.last();
                for (int i = 0; i < brick->m_part.size(); ++i)
                {
                    if(i<8 || i%4<2)
                        brick->m_part[i] = 1;
                }
            }
                break;
            case '0':
                createBrick(x,y);
            {
                PartedItem * brick = m_brick.last();
                for (int i = 0; i < brick->m_part.size(); ++i)
                {
                    if(i<8 || i%4>1)
                        brick->m_part[i] = 1;
                }
            }
                break;
            case 'c':
                createConcrete(x,y);
                break;
            case '5':
                createConcrete(x,y);
            {
                PartedItem * item = m_concrete.last();
                item->m_part[1] = 1;
                item->m_part[3] = 1;
            }

                break;
            case '6':
                createConcrete(x,y);
            {
                PartedItem * item = m_concrete.last();
                item->m_part[0] = 1;
                item->m_part[2] = 1;
            }
                break;
            case '7':
                createConcrete(x,y);
            {
                PartedItem * item = m_concrete.last();
                item->m_part[2] = 1;
                item->m_part[3] = 1;
            }
                break;
            case '8':
                createConcrete(x,y);
            {
                PartedItem * item = m_concrete.last();
                item->m_part[0] = 1;
                item->m_part[1] = 1;
            }
                break;
            case 'p':
                m_playerRespawn = std::make_pair(x,y);
                createPlayer(x,y);
                break;
            case 'f':
                createBase(x,y);
                break;
            default:
                break;
            }
            x+=32;
        }
        y+=32;
    }
    m_width = x;
    m_height = y;

    for(auto pos : m_enemyRespawnList)
    {
        createEnemy(pos.first,pos.second);
    }
}

void hitBrick(PartedItem * brick, DIRECTION direction)
{
    switch (direction) {
    case UP:
        for(int i = 0; i< brick->getCols(); ++i)
        {
            for(int j = 0; j < brick->getRows(); ++j)
            {
                int k = (brick->getCols()*brick->getRows()) - brick->getCols()*j - i - 1;
                if(brick->m_part[k] == 0)
                {
                    brick->m_part[k] = 1;
                    break;
                }
            }
        }
        break;
    case DOWN:
        for(int i = 0; i< brick->getCols(); ++i)
        {
            for(int j = 0; j < brick->getRows(); ++j)
            {
                int k = brick->getCols()*j + i;
                if(brick->m_part[k] == 0)
                {
                    brick->m_part[k] = 1;
                    break;
                }
            }
        }
        break;
    case LEFT:
        for(int j = 0; j < brick->getRows(); ++j)
        {
            for(int i = 0; i< brick->getCols(); ++i)
            {
                int k = (brick->getCols()*brick->getRows()) - brick->getCols()*j - i - 1;
                if(brick->m_part[k] == 0)
                {
                    brick->m_part[k] = 1;
                    break;
                }
            }
        }
        break;
    case RIGHT:
        for(int j = 0; j < brick->getRows(); ++j)
        {
            for(int i = 0; i< brick->getCols(); ++i)
            {
                int k =  brick->getCols()*j + i;
                if(brick->m_part[k] == 0)
                {
                    brick->m_part[k] = 1;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}


void Game::think()
{
    ++m_loopCounter;
    if(m_controlKey == Qt::Key_Space)
    {
        if(m_canMove && m_lastPlayerFire < m_loopCounter - 5)
        {
            createBullet(m_player.at(0));
            m_lastPlayerFire = m_loopCounter;
        }
        m_controlKey = Qt::Key_unknown;
    }
    for (auto enemy : m_enemy)
    {
        if(qrand()%10 == 0)
            m_directionMap[enemy->getObjectId()] = static_cast<DIRECTION>(qrand()%4);
        if(qrand()%100 == 0)
            createBullet(enemy);
        if(enemy->getPositionX()<=0)
            m_directionMap[enemy->getObjectId()] = RIGHT;
        else if(enemy->getPositionX()>=(m_width - enemy->getWidth()))
            m_directionMap[enemy->getObjectId()] = LEFT;
        else if(enemy->getPositionY()<=0)
            m_directionMap[enemy->getObjectId()] = DOWN;
        else if(enemy->getPositionY()>=(m_height - enemy->getHeight()))
            m_directionMap[enemy->getObjectId()] = UP;
    }
    for(auto bullet : m_bullet)
    {
        for(auto enemy : m_enemy)
        {
            if(std::abs(getDistance(bullet,enemy)+5)<6)
            {
                if(m_player.at(0)==bullet->parent())
                {
                    ++m_fragsCounter;
                    emit fragsCountChanged();
                    createExplosion(enemy);
                    m_directionMap.erase(enemy->getObjectId());
                    m_enemy.removeOne(enemy);
                    if(!m_frags.empty() && !m_enemyRespawnList.empty())
                    {
                        int ind = qrand()%m_enemyRespawnList.size();
                        createEnemy(m_enemyRespawnList[ind].first,m_enemyRespawnList[ind].second);
                    }
                    if(m_enemy.isEmpty())
                    {
                        setMessage("You win!");
                    }
                    emit enemyChanged();
                }
                else
                    createExplosion(bullet);
                destroyBullet(bullet);
                break;
            }
        }
        for(auto brick : m_brick)
        {
            if(std::abs(getPartedDistance(bullet,brick)+5)<6)
            {
                hitBrick(brick, m_directionMap[bullet->getObjectId()]);
                createExplosion(brick);
                emit brick->partChanged();
                if(!brick->m_part.contains(0))
                {
                    m_brick.removeOne(brick);
                    emit brickChanged();
                }
                destroyBullet(bullet);
                break;
            }
        }
        for(auto brick : m_concrete)
        {
            if(std::abs(getPartedDistance(bullet,brick)+5)<6)
            {
                hitBrick(brick, m_directionMap[bullet->getObjectId()]);
                createExplosion(brick);

                emit brick->partChanged();
                if(!brick->m_part.contains(0))
                {
                    m_concrete.removeOne(brick);
                    emit concreteChanged();
                }
                destroyBullet(bullet);
                break;
            }
        }
        for(auto base : m_base)
        {
            if(std::abs(getDistance(bullet,base)+10)<6)
            {
                base->setObjectImagePath("qrc:/img/base_destroyed.png");
                m_canMove = false;
                setMessage("Game over");
                destroyBullet(bullet);
                break;
            }
        }
        for(auto player : m_player)
        {
            if(std::abs(getDistance(bullet,player)+10)<6)
            {
                if(m_player.at(0)!=bullet->parent())
                {
                    createExplosion(player);
                    if(m_lifesCounter>0)
                    {
                        --m_lifesCounter;
                        player->updatePositionAndRotation(m_playerRespawn.first,m_playerRespawn.second,0);
                    }
                    else
                    {
                        m_canMove = false;
                        setMessage("Game over");
                    }
                    emit lifesCountChanged();
                }
                destroyBullet(bullet);
                break;
            }

        }
    }
    for(auto explosion : m_explosion)
    {
        if(m_explosionMap[explosion->getObjectId()] < m_loopCounter - 2)
        {
            m_explosionMap.erase(explosion->getObjectId());
            m_explosion.removeOne(explosion);
            emit explosionChanged();
        }
    }
}


void Game::createBullet(GraphicItem * parent)
{
    GraphicItem * bullet = new GraphicItem(parent);
    bullet->setObjectId(++m_objectCounter);
    bullet->setObjectImagePath("qrc:/img/bullet.png");
    bullet->updatePositionAndRotation(parent->getPositionX(),parent->getPositionY(),parent->getRotation());
    m_bullet.append(bullet);
    m_directionMap[m_objectCounter] = m_directionMap[parent->getObjectId()];
    if(m_directionMap[m_objectCounter] == NONE)
        m_directionMap[m_objectCounter] = static_cast<DIRECTION>((parent->getRotation()/90 + 3)%4);
    QSound::play(":/sounds/shot.wav");
    emit bulletChanged();
}

void Game::createExplosion(GraphicItem *parent)
{
    AnimatedItem * explosion = new AnimatedItem(0);
    explosion->setObjectId(++m_objectCounter);
    explosion->setObjectImagePath("qrc:/img/explosion.png");
    explosion->updatePositionAndRotation(parent->getPositionX(),parent->getPositionY(),0);
    explosion->setAnimationPaused(false);
    m_explosion.append(explosion);
    m_explosionMap[m_objectCounter] = m_loopCounter;
    QSound::play(":/sounds/explosion.wav");
    emit explosionChanged();
}

void Game::createBrick(int x, int y)
{
    PartedItem * brick = new PartedItem(4,4,0);
    brick->setObjectId(++m_objectCounter);
    brick->setObjectImagePath("qrc:/img/board/brickwall.png");
    brick->updatePositionAndRotation(x,y,0);
    for(int i = 0; i < 16; ++i)
    {
        brick->m_part.push_back(0);
    }
    m_brick.append(brick);
    emit brickChanged();
}

void Game::createConcrete(int x, int y)
{
    PartedItem * brick = new PartedItem(2,2,0);
    brick->setObjectId(++m_objectCounter);
    brick->setObjectImagePath("qrc:/img/board/armoredwall.png");
    brick->updatePositionAndRotation(x,y,0);
    for(int i = 0; i < 4; ++i)
    {
        brick->m_part.push_back(0);
    }
    m_concrete.append(brick);
    emit concreteChanged();
}

void Game::createBase(int x, int y)
{
    GraphicItem * base = new GraphicItem(0);
    base->setObjectId(++m_objectCounter);
    base->setObjectImagePath("qrc:/img/mybase.png");
    base->updatePositionAndRotation(x,y,0);
    m_base.append(base);
    emit baseChanged();
}

void Game::createPlayer(int x, int y)
{
    AnimatedItem * player = new AnimatedItem(0);
    player->setObjectId(0);
    player->setObjectImagePath("qrc:/img/mytank.png");
    player->updatePositionAndRotation(x,y,0);
    m_player.append(player);
    m_directionMap[0] = NONE;
    emit playerChanged();
}

void Game::createEnemy(int x, int y)
{
    AnimatedItem * enemy = new AnimatedItem(0);
    enemy->setObjectId(++m_objectCounter);
    enemy->setObjectImagePath("qrc:/img/enemytank.png");
    enemy->updatePositionAndRotation(x,y,0);
    m_enemy.append(enemy);
    m_directionMap[m_objectCounter] = DOWN;
    if(!m_frags.empty())
        m_frags.removeLast();
    emit enemyChanged();
    emit fragsChanged();
}

void Game::createOrUpdateFrags(int count)
{
    m_frags.clear();
    for(int i = 0; i < count; ++i)
    {
        IconItem * frag = new IconItem(0);
        frag->setObjectId(++m_objectCounter);
        frag->setObjectImagePath("qrc:/img/frag_icon.png");
        frag->updatePositionAndRotation(0,0,0);
        m_frags.append(frag);
    }
}

void Game::destroyBullet(GraphicItem *bullet)
{
    m_directionMap.erase(bullet->getObjectId());
    m_bullet.removeOne(bullet);
    emit bulletChanged();
}

int Game::checkWay(AnimatedItem *item)
{
    if(!checkBorders(item))
        return 0;
    int near = 100;
    for (auto enemy : m_enemy)
    {
        if(enemy->getObjectId() == item->getObjectId())
            continue;
        int dst = getDistance(item, enemy);
        if(dst >=0)
            near = std::min(near, dst);
    }
    int dst = getDistance(item, m_player.at(0));
    if(dst>=0)
        near = std::min(near, dst);
    for (auto brick : m_brick)
    {
        int dst = getPartedDistance(item, brick);
        if(dst >=0)
            near = std::min(near, dst);
    }
    for (auto brick : m_concrete)
    {
        int dst = getPartedDistance(item, brick);
        if(dst >=0)
            near = std::min(near, dst);
    }
    for (auto base : m_base)
    {
        int dst = getDistance(item, base);
        if(dst >=0)
            near = std::min(near, dst);
    }

    return near;
}
int Game::getPartedDistance(GraphicItem *current, PartedItem *brick)
{

    int cLeftX = current->getPositionX();
    int cRightX = current->getPositionX() + current->getWidth();
    int cUpY = current->getPositionY();
    int cDownY = current->getPositionY()+current->getHeight();

    int oLeftX = brick->getPositionX() +
            ((brick->m_part.indexOf(0)%brick->getCols()) *
             (brick->getWidth()/brick->getCols()));
    int oRightX = brick->getPositionX() +
            ((1 + brick->m_part.lastIndexOf(0)%brick->getCols()) *
             (brick->getWidth()/brick->getCols()));
    int oUpY = brick->getPositionY() +
            ((brick->m_part.indexOf(0)/brick->getCols()) *
             (brick->getHeight()/brick->getRows()));
    int oDownY = brick->getPositionY() +
            ((1 + brick->m_part.lastIndexOf(0)/brick->getCols()) *
             (brick->getHeight()/brick->getRows()));

    DIRECTION direction = m_directionMap[current->getObjectId()];
    int dst = 100;
    switch (direction) {
    case UP:
        if(cLeftX>=oRightX || cRightX <= oLeftX)
            return dst;
        return cUpY-oDownY;
    case DOWN:
        if(cLeftX>=oRightX || cRightX <= oLeftX)
            return dst;
        return oUpY-cDownY;

    case LEFT:
        if(cUpY >= oDownY || cDownY <= oUpY)
            return dst;
        return cLeftX - oRightX;
    case RIGHT:
        if(cUpY >= oDownY || cDownY <= oUpY)
            return dst;
        return oLeftX - cRightX;
    default:
        break;
    }

    return dst;
}
int Game::getDistance(GraphicItem *current, GraphicItem *other)
{
    DIRECTION direction = m_directionMap[current->getObjectId()];
    if(current->getObjectId() == other->getObjectId())
        return 50;
    int dx = current->getPositionX() - other->getPositionX();
    int dy = current->getPositionY() - other->getPositionY();
    int cX = current->getPositionX();
    int cY = current->getPositionY();
    int oX = other->getPositionX();
    int oY = other->getPositionY();
    switch (direction) {
    case UP:
        if(std::abs(dx)>=current->getWidth())
            return 55;
        return cY - oY - other->getHeight();
    case DOWN:
        if(std::abs(dx)>=current->getWidth())
            return 55;
        return oY - cY - current->getHeight();
    case LEFT:
        if(std::abs(dy)>=current->getHeight())
            return 55;
        return cX - oX - other->getWidth();
    case RIGHT:
        if(std::abs(dy)>=current->getHeight())
            return 55;
        return oX - cX - current->getWidth();
    default:
        break;
    }
    return 0;
}
