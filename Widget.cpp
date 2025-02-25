#include "Widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    srand(time(NULL));
    this->setFixedSize(1770,1024);
    mscene.setSceneRect(0,0,1770,1024);
    mview.setScene(&mscene);
    mview.setParent(this);
    mview.setFocusPolicy(Qt::NoFocus);
    auto pixmap=QPixmap(":/ima/background.png");
    pixmap = pixmap.scaled(pixmap.size() * 2,
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
    m_background = new QGraphicsPixmapItem(pixmap);
    mscene.addItem(m_background);
    // 创建血条
    m_heroHealth = new ValueBar(&mscene,
        QPointF(20, 20),
        Qt::red,
        "HERO",
        true,
        100,
        100);

    m_enemyHealth = new ValueBar(&mscene,
        QPointF(mscene.width() - 320, 20),
        QColor(0, 100, 255),
        "ENEMY",
        true,
        100,
        100);
    //创建精力条
    m_heroEnergy = new ValueBar(&mscene,
        QPointF(20, 50),
        Qt::green,
        "HERO",
        false,
        50,
        0);
    m_enemyEnergy = new ValueBar(&mscene,
        QPointF(mscene.width() - 170, 50),
        Qt::green,
        "ENEMY",
        false,
        50,
        0);
    updateWidget();
    moveTimer=new QTimer(this);
    connect(moveTimer,&QTimer::timeout,this,&Widget::rolemove);
    connect(moveTimer,&QTimer::timeout,this,&Widget::enemymove);
    moveTimer->start(10);
    m_roundStartTimer = new QTimer(this);
    m_roundStartTimer->setSingleShot(true);
    connect(m_roundStartTimer, &QTimer::timeout, this, &Widget::showCountDown);
    connect(m_roundStartTimer, &QTimer::timeout,this,[=](){
        mscene.removeItem(m_roundText);
        delete m_roundText;
    });
}
void Widget::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
        case Qt::Key_J:
        case Qt::Key_L:
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_K:
        case Qt::Key_H:
        case Qt::Key_I:
            m_keyList.append(event->key());
            break;
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_Up:
        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
            m_enemyKeyList.append(event->key());
            break;
    }
    QWidget::keyPressEvent(event);
}
void Widget::keyReleaseEvent(QKeyEvent *event){
    if(m_keyList.contains(event->key())){
        m_keyList.removeOne(event->key());
    }
    if(m_enemyKeyList.contains(event->key())){
        m_enemyKeyList.removeOne(event->key());
    }
    if(!event->isAutoRepeat()){
        if(!hero->isplaying())
            hero->stopAll();
        if(!enemy->isplaying())
            enemy->stopAll();
    }
    QWidget::keyReleaseEvent(event);
}

void Widget::rolemove(){
    static int flag;
    if (!m_roundActive) return;
    if(hero->isplaying()) return;
    if(m_keyList.contains(Qt::Key_H)){
        hero->setMovement(QPointF(0,0));
        hero->playCharge();
        flag=1;
    }else{
        flag=0;
    }
    if(m_keyList.contains(Qt::Key_J)){
        hero->setMovement(QPointF(0,0));
        hero->playAttack();
        return;
    }
    if(m_keyList.contains(Qt::Key_L)){
        hero->setMovement(QPointF(0,0));
        hero->playUltimate();
        return;
    }
    if(m_keyList.contains(Qt::Key_I)){
        hero->setMovement(QPointF(0,0));
        hero->playSpecial();
        return;
    }
    if(flag) return;
    hero->setMovement(QPointF(0,0));
    double move;
    if(m_keyList.size()==1||(m_keyList.size()==2&&m_keyList.contains(Qt::Key_K))){
        move=5;
    }else{
        move=3.5;
    }
    for(int keyCode:m_keyList){
        switch (keyCode) {
        case Qt::Key_W:hero->setMovement(hero->getDirection()+QPointF(0,-move/1.5));break;
        case Qt::Key_A:hero->setMovement(hero->getDirection()+QPointF(-move,0));break;
        case Qt::Key_S:hero->setMovement(hero->getDirection()+QPointF(0,move/1.5));break;
        case Qt::Key_D:hero->setMovement(hero->getDirection()+QPointF(move,0));break;
        }
    }
    if(m_keyList.contains(Qt::Key_K)){
        hero->playMove();
    }else if(m_keyList.contains(Qt::Key_W)||m_keyList.contains(Qt::Key_A)
             ||m_keyList.contains(Qt::Key_S)||m_keyList.contains(Qt::Key_D)){
        hero->playSlowMove();
    }
}

void Widget::enemymove(){
    static int flag;
    if (!m_roundActive) return;
    if(enemy->isplaying()) return;
    if(m_enemyKeyList.contains(Qt::Key_5)){
        enemy->setMovement(QPointF(0,0));
        enemy->playCharge();
        flag=1;
    }else{
        flag=0;
    }
    if(m_enemyKeyList.contains(Qt::Key_1)){
        enemy->setMovement(QPointF(0,0));
        enemy->playAttack();
        return;
    }
    if(m_enemyKeyList.contains(Qt::Key_3)){
        enemy->setMovement(QPointF(0,0));
        enemy->playUltimate();
        return;
    }
    if(m_enemyKeyList.contains(Qt::Key_4)){
        enemy->setMovement(QPointF(0,0));
        enemy->playSpecial();
        return;
    }
    if(flag) return;
    enemy->setMovement(QPointF(0,0));
    double move;
    if(m_enemyKeyList.size()==1||(m_enemyKeyList.size()==2&&m_enemyKeyList.contains(Qt::Key_2))){
        move=5;
    }else{
        move=3.5;
    }
    for(int keyCode:m_enemyKeyList){
        switch (keyCode) {
        case Qt::Key_Up:enemy->setMovement(enemy->getDirection()+QPointF(0,-move/1.5));break;
        case Qt::Key_Left:enemy->setMovement(enemy->getDirection()+QPointF(-move,0));break;
        case Qt::Key_Down:enemy->setMovement(enemy->getDirection()+QPointF(0,move/1.5));break;
        case Qt::Key_Right:enemy->setMovement(enemy->getDirection()+QPointF(move,0));break;
        }
    }
    if(m_enemyKeyList.contains(Qt::Key_2)){
        enemy->playMove();
    }else if(m_enemyKeyList.contains(Qt::Key_Up)||m_enemyKeyList.contains(Qt::Key_Left)
             ||m_enemyKeyList.contains(Qt::Key_Down)||m_enemyKeyList.contains(Qt::Key_Right)){
        enemy->playSlowMove();
    }
}

void Widget::updateWidget(){
    int H=rand()%4;
    int E=rand()%4;
    m_roundActive = false;
    // 创建角色
    switch (H) {
    case 0:
        hero = new GokuRed(2.0);
        connect(hero, &GokuRed::change ,this,[=](){
            int health=hero->health();
            int energy=hero->energy();
            QPointF pos=hero->pos();
            bool transform=hero->getTransform();
            delete hero;
            hero = new GokuBlue(2.0);
            connect(hero, &CharacterBase::energyChanged, m_heroEnergy, &ValueBar::updateValue);
            connect(hero, &CharacterBase::healthChanged, m_heroHealth, &ValueBar::updateValue);
            connect(hero, &CharacterBase::roundEnded, this, &Widget::checkRoundWinner);
            hero->setHealth(health+50);
            hero->setEnergy(energy);
            mscene.addItem(hero);
            hero->setPos(pos);
            hero->setEnemy(enemy);
            enemy->setEnemy(hero);
            hero->setZValue(1);
            if(transform) hero->setMovement(QPointF(-1,0));
            hero->backIdle();
        });
        break;
    case 1:
        hero = new GokuBlue(2.0);
        break;
    case 2:
        hero = new Frieza(2.0);
        break;
    default:
        hero = new Gokussthree(2.0);
        break;
    }
    switch (E) {
    case 0:
        enemy = new GokuRed(2.0);
        connect(enemy, &GokuRed::change ,this,[=](){
            int health=enemy->health();
            int energy=enemy->energy();
            QPointF pos=enemy->pos();
            bool transform=enemy->getTransform();
            delete enemy;
            enemy = new GokuBlue(2.0);
            connect(enemy, &CharacterBase::energyChanged, m_enemyEnergy, &ValueBar::updateValue);
            connect(enemy, &CharacterBase::healthChanged, m_enemyHealth, &ValueBar::updateValue);
            connect(enemy, &CharacterBase::roundEnded, this, &Widget::checkRoundWinner);
            enemy->setHealth(health+50);
            enemy->setEnergy(energy);
            mscene.addItem(enemy);
            enemy->setPos(pos);
            hero->setEnemy(enemy);
            enemy->setEnemy(hero);
            enemy->setZValue(1);
            if(transform) enemy->setMovement(QPointF(-1,0));
            enemy->backIdle();
        });
        break;
    case 1:
        enemy = new GokuBlue(2.0);
        break;
    case 2:
        enemy = new Frieza(2.0);
        break;
    default:
        enemy = new Gokussthree(2.0);
        break;
    }
    hero->setPos(200,300);
    mscene.addItem(enemy);
    mscene.addItem(hero);
    enemy->setPos(1550,300);
    hero->setEnemy(enemy);
    enemy->setEnemy(hero);
    hero->setZValue(1);
    enemy->setZValue(1);
    enemy->setMovement(QPointF(-1,0));

    connect(hero, &CharacterBase::healthChanged, m_heroHealth, &ValueBar::updateValue);
    connect(enemy, &CharacterBase::healthChanged, m_enemyHealth, &ValueBar::updateValue);

    connect(hero, &CharacterBase::energyChanged, m_heroEnergy, &ValueBar::updateValue);
    connect(enemy, &CharacterBase::energyChanged, m_enemyEnergy, &ValueBar::updateValue);

    hero->setHealth(100);
    hero->setEnergy(0);
    enemy->setHealth(100);
    enemy->setEnergy(0);

    connect(hero, &CharacterBase::roundEnded, this, &Widget::checkRoundWinner);
    connect(enemy, &CharacterBase::roundEnded, this, &Widget::checkRoundWinner);

    connect(hero, &CharacterBase::entranceFinished, this, &Widget::checkRoundStart);
    connect(enemy, &CharacterBase::entranceFinished, this, &Widget::checkRoundStart);

    hero->playEntrance();
    enemy->playEntrance();
}

void Widget::showRoundNumber() {
    m_roundText = new QGraphicsTextItem(QString("ROUND %1").arg(m_currentRound));
    QFont font("Times New Roman", 72, QFont::Bold);
    m_roundText->setFont(font);
    m_roundText->setDefaultTextColor(Qt::white);
    QRectF textRect = m_roundText->boundingRect();
    m_roundText->setPos(mscene.width()/2 - textRect.width()/2,
                       mscene.height()/2 - textRect.height()/2);
    mscene.addItem(m_roundText);

    m_roundStartTimer->start(1500);
}

void Widget::showCountDown() {
    if (m_countdownText) {
        mscene.removeItem(m_countdownText);
        delete m_countdownText;
    }

    static int count = 3;
    m_countdownText = new QGraphicsTextItem(QString::number(count));
    QFont font("Times New Roman", 120, QFont::Bold);
    m_countdownText->setFont(font);
    m_countdownText->setDefaultTextColor(Qt::red);
    QRectF textRect = m_countdownText->boundingRect();
    m_countdownText->setPos(mscene.width()/2 - textRect.width()/2,
                           mscene.height()/2 - textRect.height()/2);
    mscene.addItem(m_countdownText);

    if (count > 0) {
        QTimer::singleShot(1000, this, &Widget::showCountDown);
        count--;
    } else {
        mscene.removeItem(m_countdownText);
        delete m_countdownText;
        m_countdownText = nullptr;
        count = 3;
        m_roundActive = true;
    }
}

void Widget::startNewRound() {
    hero->stopAll();
    enemy->stopAll();
    delete hero;
    delete enemy;
    updateWidget();
}

void Widget::checkRoundStart() {
    static bool heroReady = false;
    static bool enemyReady = false;

    if (sender() == hero) heroReady = true;
    if (sender() == enemy) enemyReady = true;

    if (heroReady && enemyReady) {
        heroReady = enemyReady = false;
        QTimer::singleShot(500, this, &Widget::showRoundNumber);
    }
}

void Widget::checkRoundWinner() {
    if (hero->health() <= 0) {
        m_enemyWins++;
    } else if (enemy->health() <= 0) {
        m_heroWins++;
    }

    // 检查胜利条件
    if (m_heroWins >= 4 || m_enemyWins >= 4) {
        showFinalWinner();
        return;
    }

    // 开始下一回合
    m_currentRound++;
    QTimer::singleShot(3000, this, &Widget::startNewRound);
}

void Widget::showFinalWinner() {
    QGraphicsTextItem* winText = new QGraphicsTextItem(
        m_heroWins >=4 ? "HERO WINS!" : "ENEMY WINS!");
    QFont font("Times New Roman", 96, QFont::Bold);
    winText->setFont(font);
    winText->setDefaultTextColor(Qt::yellow);
    QRectF textRect = winText->boundingRect();
    winText->setPos(mscene.width()/2 - textRect.width()/2,
                   mscene.height()/2 - textRect.height()/2);
    mscene.addItem(winText);
}

Widget::~Widget()
{
}

