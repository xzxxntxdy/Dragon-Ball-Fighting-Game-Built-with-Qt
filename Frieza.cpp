#include "Frieza.h"

Frieza::Frieza(double scale,QGraphicsItem* parent)
    :CharacterBase(scale,parent)
{
    m_attackHurtNum=4;
    m_specialHurtNum=3;
    m_ultimateDamage=30;
    m_specialDamage=10;
    setupAnimations();
}

void Frieza::setupAnimations(){
    const QString resourceDir=":/ima/Frieza/";
    // 初始化站立和移动状态图片
    m_idlePixmap = QPixmap(resourceDir + "pre/0.png");
    m_landPixmap = QPixmap(resourceDir + "pre/2.png");
    m_floatPixmap = QPixmap(resourceDir + "pre/1.png");
    m_bulletPixmap = QPixmap(resourceDir + "ultimate/8.png");
    if (m_scale != 1.0) {
        m_idlePixmap = m_idlePixmap.scaled(m_idlePixmap.size() * m_scale,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
        m_landPixmap = m_landPixmap.scaled(m_landPixmap.size() * m_scale,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
        m_floatPixmap = m_floatPixmap.scaled(m_floatPixmap.size() * m_scale,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
        m_bulletPixmap = m_bulletPixmap.scaled(m_bulletPixmap.size() * m_scale,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }
    // 攻击动画
    m_animations[Attack] = new AnimatedPixmapItem(
        this,
        resourceDir + "attack/",
        20
    );
    m_animations[Attack]->setFrameRate(8);
    m_animations[Attack]->setEndFrameNum(5);
    m_animations[Attack]->setFrameDuration(4,300);
    m_animations[Attack]->setFrameDuration(9,300);
    m_animations[Attack]->setFrameDuration(14,300);
    m_animations[Attack]->setFrameDuration(19,800);
    m_animations[Attack]->setPerFrameMovement(QPointF(10, 0)); // 攻击前冲效果
    m_animations[Attack]->setPerFrameMovement(13,QPointF(10, -42));
    m_animations[Attack]->setPerFrameMovement(14,QPointF(10, 42));
    m_animations[Attack]->setPerFrameMovement(15,QPointF(10, -40));
    m_animations[Attack]->setPerFrameMovement(16,QPointF(10, 40));
    // 设置攻击有效帧
    for (int i = 1; i < 20; i+=5) {
        m_animations[Attack]->setFrameCollision(i, QRectF(40, 20, 100, 70));
        m_animations[Attack]->setFrameCollision(i+1, QRectF(40, 20, 100, 70));
    }

    // 移动动画
    m_animations[Move] = new AnimatedPixmapItem(
        this,
        resourceDir + "move/",
        8
    );
    m_animations[Move]->setFrameRate(30);
    m_animations[Move]->setPerFrameMovement(7,QPointF(0,-1));

    // 聚气动画
    m_animations[Charge] = new AnimatedPixmapItem(
        this,
        resourceDir + "charge/",
        2
    );
    m_animations[Charge]->setLooping(true);
    m_animations[Charge]->setFrameRate(8);
    m_animations[Charge]->setPerFrameMovement(0,QPointF(6,0));
    m_animations[Charge]->setPerFrameMovement(1,QPointF(-6,0));

    // 大招动画
    m_animations[Ultimate] = new AnimatedPixmapItem(
        this,
        resourceDir + "ultimate/",
        8
    );
    m_animations[Ultimate]->setFrameRate(8);
    m_animations[Ultimate]->setFrameDuration(7,500);
    connect(m_animations[Ultimate], &AnimatedPixmapItem::animationStopped,this,&Frieza::fire);

    // 受击动画
    m_animations[Hurt] = new AnimatedPixmapItem(
        this,
        resourceDir + "hurt/",
        6
    );
    m_animations[Hurt]->setFrameRate(5);
    m_animations[Hurt]->setPerFrameMovement(QPointF(10,0));
    m_animations[Hurt]->setPerFrameMovement(4,QPointF(30,50));
    m_animations[Hurt]->setPerFrameMovement(5,QPointF(0,-50));
    m_animations[Hurt]->setFrameDuration(4,500);

    //死亡动画
    m_animations[Dead] = new AnimatedPixmapItem(
        this,
        resourceDir + "dead/",
        11
    );
    m_animations[Dead]->setFrameRate(15);

    //special动画
    m_animations[Special] = new AnimatedPixmapItem{
        this,
        resourceDir + "special/",
        5
    };
    m_animations[Special]->setFrameCollision(4, QRectF(0, 20, 450, 70));
    connect(m_animations[Special], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);

    //entrance动画
    m_animations[Entrance] = new AnimatedPixmapItem(
        this,
        resourceDir + "entrance/",
        11
    );
    m_animations[Entrance]->setFrameRate(15);
    m_animations[Entrance]->setFrameDuration(0,1000);

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void Frieza::playMove(){
    m_animations[Move]->setPerFrameMovement(5,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    CharacterBase::playMove();
}

void Frieza::playUltimate(){
    if (m_energy < 20) {
        return;
    }
    if(m_transform){
        setPos(scene()->sceneRect().right(),0);
        m_animations[Ultimate]->setPerFramePos(7,QPointF(scene()->sceneRect().right(),0));
    }else{
        setPos(0,0);
        m_animations[Ultimate]->setPerFramePos(7,QPointF(0,0));
    }
    CharacterBase::playUltimate();
}

void Frieza::fire(){
    // 计算发射起点
    QRectF charRect = collisionBoundingBox();
    QPointF startPos = charRect.center()+QPointF(0,-130);
    fireBullet(3,m_ultimateDamage,startPos);
}

void Frieza::playSpecial(){
    if(m_energy<10){
        return;
    }else{
        consumeEnergy(10);
    }
    CharacterBase::playSpecial();
}

void Frieza::GetHit(int damage){
    if(m_currentState==Ultimate) return;
    CharacterBase::GetHit(damage);
}
