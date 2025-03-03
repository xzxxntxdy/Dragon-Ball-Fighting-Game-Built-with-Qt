#include "Buu.h"

Buu::Buu(double scale,QGraphicsItem* parent)
    :CharacterBase(scale,parent)
{
    m_attackHurtNum=3;
    m_specialHurtNum=3;
    m_ultimateDamage=20;
    m_specialDamage=5;
    m_abnormalTimer=new QTimer(this);
    m_abnormalTimer->setSingleShot(true);
    setupAnimations();
}

void Buu::setupAnimations(){
    const QString resourceDir=":/ima/buu/";
    // 初始化站立和移动状态图片
    m_idlePixmap = QPixmap(resourceDir + "pre/0.png");
    m_landPixmap = QPixmap(resourceDir + "pre/2.png");
    m_floatPixmap = QPixmap(resourceDir + "pre/1.png");
    m_bulletPixmap = QPixmap(resourceDir + "ultimate/3.png");
    m_abnormalPixmap = QPixmap(resourceDir + "special/5.png");
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
        m_abnormalPixmap = m_abnormalPixmap.scaled(m_abnormalPixmap.size() * m_scale,
                             Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
    }
    //攻击动画
    m_animations[Attack] = new AnimatedPixmapItem(
        this,
        resourceDir + "attack/",
        12
    );
    m_animations[Attack]->setFrameRate(7);
    m_animations[Attack]->setEndFrameNum(4);
    m_animations[Attack]->setFrameDuration(3,300);
    m_animations[Attack]->setFrameDuration(7,300);
    m_animations[Attack]->setFrameDuration(11,800);
    m_animations[Attack]->setPerFrameMovement(QPointF(10, 0)); // 攻击前冲效果
    // 设置攻击有效帧
    m_animations[Attack]->setFrameCollision(1, QRectF(40, 20, 200, 70));
    m_animations[Attack]->setFrameCollision(5, QRectF(40, 20, 200, 70));
    m_animations[Attack]->setFrameCollision(10, QRectF(40, 20, 200, 70));

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
        3
    );
    m_animations[Charge]->setLooping(true);
    m_animations[Charge]->setFrameRate(6);
    m_animations[Charge]->setPerFrameMovement(0,QPointF(0,1));
    m_animations[Charge]->setPerFrameMovement(1,QPointF(0,-1));
    m_animations[Charge]->setPerFrameMovement(2,QPointF(0,-1));

    // 大招动画
    m_animations[Ultimate] = new AnimatedPixmapItem(
        this,
        resourceDir + "ultimate/",
        3
    );
    m_animations[Ultimate]->setFrameDuration(0,200);
    m_animations[Ultimate]->setFrameDuration(1,500);
    m_animations[Ultimate]->setFrameDuration(2,500);
    m_animations[Ultimate]->setPerFrameMovement(1,QPointF(-60,-180));
    m_animations[Ultimate]->setPerFrameMovement(2,QPointF(60,180));
    connect(m_animations[Ultimate], &AnimatedPixmapItem::animationStopped,this,&Buu::fire);

    // 受击动画
    m_animations[Hurt] = new AnimatedPixmapItem(
        this,
        resourceDir + "hurt/",
        8
    );
    m_animations[Hurt]->setFrameRate(5);
    m_animations[Hurt]->setPerFrameMovement(QPointF(10,0));
    m_animations[Hurt]->setPerFrameMovement(6,QPointF(30,50));
    m_animations[Hurt]->setPerFrameMovement(7,QPointF(0,-50));
    m_animations[Hurt]->setFrameDuration(6,500);

    //死亡动画
    m_animations[Dead] = new AnimatedPixmapItem(
        this,
        resourceDir + "dead/",
        11
    );
    m_animations[Dead]->setFrameRate(5);
    m_animations[Dead]->setCenter();
    m_animations[Dead]->setFrameDuration(3,1500);

    //special动画
    m_animations[Special] = new AnimatedPixmapItem{
        this,
        resourceDir + "special/",
        5
    };
    m_animations[Special]->setFrameCollision(4, QRectF(40, 20, 270, 70));
    connect(m_animations[Special], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);

    //entrance动画
    m_animations[Entrance] = new AnimatedPixmapItem{
        this,
        resourceDir + "entrance/",
        10
    };

    m_animations[Entrance]->setFrameRate(6);
    m_animations[Entrance]->setCenter();
    m_animations[Entrance]->setPerFrameMovement(0,QPointF(20,20));
    m_animations[Entrance]->setFrameDuration(0,300);
    m_animations[Entrance]->setFrameDuration(2,300);
    m_animations[Entrance]->setFrameDuration(4,500);
    m_animations[Entrance]->setFrameDuration(5,500);
    m_animations[Entrance]->setFrameDuration(7,500);
    m_animations[Entrance]->setSegmentLoop(4,5,3);

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void Buu::playMove(){
    m_animations[Move]->setPerFrameMovement(5,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    CharacterBase::playMove();
}

void Buu::cheakHitBySpecial(){
    if (m_currentState==Special) {
        QRectF attackArea = m_animations[Special]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            m_enemy->stopAll();
            if(m_enemy->health()>5){
                m_enemy->setState(Abnormal);
                m_enemy->setPixmap(m_abnormalPixmap);
                m_abnormalTimer->start(5000);
            }else{
                disconnect(m_abnormalTimer,&QTimer::timeout,m_enemy,&CharacterBase::backIdle);
            }
            m_enemy->takeDamage(m_specialDamage);
        }
    }
}

void Buu::playSpecial(){
    if(m_energy<10){
        return;
    }else{
        consumeEnergy(10);
    }
    setZValue(1);
    m_enemy->setZValue(2);
    connect(m_abnormalTimer,&QTimer::timeout,m_enemy,&CharacterBase::backIdle);
    CharacterBase::playSpecial();
}

QRectF Buu::collisionBoundingBox(){
    QSizeF size;
    if(m_currentState==Attack||m_currentState==Special)
         size = m_idlePixmap.size();
    else
        size = boundingRect().size();
    return QRectF(
        m_transform ? pos().x()-size.width():pos().x(),
        pos().y(),
        size.width(),
        size.height()
    );
}

void Buu::fire(){
    // 计算发射起点
    QRectF charRect = collisionBoundingBox();
    QPointF startPos = charRect.center()+QPointF(0,-90);
    fireBullet(3,m_ultimateDamage,startPos);
}

void Buu::playDead(){
    if(deadTime==0){
        m_animations[Dead]->setEndFrameNum(-1);
        deadTime++;
        connect(m_animations[Dead], &AnimatedPixmapItem::animationStopped,this,&Buu::revive);
    }else{
        m_animations[Dead]->setEndFrameNum(3);
        deadTime=0;
        disconnect(m_animations[Dead], &AnimatedPixmapItem::animationStopped,this,&Buu::revive);
        connect(m_animations[Dead], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::roundEnded);
    }
    m_animations[Dead]->transform(m_transform);
    switchAnimation(Dead);
}

void Buu::revive(){
    backIdle();
    setHealth(100);
}

void Buu::GetHit(int damage){
    if(m_currentState==Ultimate) return;
    CharacterBase::GetHit(damage);
}
