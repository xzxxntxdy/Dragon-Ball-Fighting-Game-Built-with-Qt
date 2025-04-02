#include "Gogetass.h"

Gogetass::Gogetass(double scale,QGraphicsItem* parent)
    :CharacterBase(scale,parent)
{
    m_attackHurtNum=3;
    m_specialHurtNum=3;
    m_ultimateDamage=35;
    m_specialDamage=15;
    setupAnimations();
}

void Gogetass::setupAnimations(){
    const QString resourceDir=":/ima/gogetass/";
    // 初始化站立和移动状态图片
    m_idlePixmap = QPixmap(resourceDir + "pre/0.png");
    m_landPixmap = QPixmap(resourceDir + "pre/2.png");
    m_floatPixmap = QPixmap(resourceDir + "pre/1.png");
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
    }
    //攻击动画
    m_animations[Attack] = new AnimatedPixmapItem(
        this,
        resourceDir + "attack/",
        24
    );
    m_animations[Attack]->setFrameRate(10);
    m_animations[Attack]->setEndFrameNum(6);
    m_animations[Attack]->setFrameDuration(5,300);
    m_animations[Attack]->setFrameDuration(11,300);
    m_animations[Attack]->setFrameDuration(17,300);
    m_animations[Attack]->setFrameDuration(23,800);
    m_animations[Attack]->setPerFrameMovement(QPointF(10, 0)); // 攻击前冲效果
    // 设置攻击有效帧
    for (int i = 0; i <= 21; i+=6) {
        m_animations[Attack]->setFrameCollision(i, QRectF(40, 20, 100, 70));
        m_animations[Attack]->setFrameCollision(i+1, QRectF(40, 20, 100, 70));
        m_animations[Attack]->setFrameCollision(i+2, QRectF(40, 20, 100, 70));
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
    m_animations[Charge]->setFrameRate(6);
    m_animations[Charge]->setPerFrameMovement(0,QPointF(6,0));
    m_animations[Charge]->setPerFrameMovement(1,QPointF(-6,0));

    // 大招动画
    m_animations[Ultimate] = new AnimatedPixmapItem(
        this,
        resourceDir + "ultimate/",
        21
    );
    m_animations[Ultimate]->setFrameRate(14);
    m_animations[Ultimate]->setPerFrameMovement(13,QPointF(0,-100));
    m_animations[Ultimate]->setPerFrameMovement(14,QPointF(0,20));
    m_animations[Ultimate]->setPerFrameMovement(15,QPointF(0,70));
    m_animations[Ultimate]->setPerFrameMovement(16,QPointF(0,10));
    m_animations[Ultimate]->setFrameDuration(13,1000);
    //设置大招有效帧
    m_animations[Ultimate]->setFrameCollision(13, QRectF(0, -70, 1500, 270));

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
        9
    );
    m_animations[Dead]->setFrameRate(5);
    m_animations[Dead]->setCenter();

    //special动画
    m_animations[Special] = new AnimatedPixmapItem{
        this,
        resourceDir + "special/",
        11
    };
    m_animations[Special]->setFrameRate(8);
    m_animations[Special]->setFrameCollision(8, QRectF(0, 35, 520, 150));
    m_animations[Special]->setPerFrameMovement(6,QPointF(0,-40));
    m_animations[Special]->setPerFrameMovement(7,QPointF(-24,-30));
    m_animations[Special]->setPerFrameMovement(8,QPointF(14,50));
    m_animations[Special]->setPerFrameMovement(10,QPointF(0,20));
    m_animations[Special]->setFrameDuration(7,400);
    m_animations[Special]->setFrameDuration(8,1000);
    connect(m_animations[Special], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);

    //entrance动画
    m_animations[Entrance] = new AnimatedPixmapItem{
        this,
        resourceDir + "entrance/",
        26
    };

    m_animations[Entrance]->setFrameRate(6);
    m_animations[Entrance]->setCenter();
    m_animations[Entrance]->setPerFrameMovement(0,QPointF(-130,0));
    m_animations[Entrance]->setFrameDuration(0,500);

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void Gogetass::playMove(){
    m_animations[Move]->setPerFrameMovement(5,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    CharacterBase::playMove();
}

QPointF Gogetass::checkCollision(QPointF newPos){
    if(m_currentState==Special) return newPos;
    return CharacterBase::checkCollision(newPos);
}

void Gogetass::cheakHitBySpecial(){
    if (m_currentState==Special) {
        QRectF attackArea = m_animations[Special]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            m_enemy->m_animations[Hurt]->setEndFrameNum(-1);
            emit Hit(m_specialDamage);
        }
    }
}

void Gogetass::playSpecial(){
    if(m_energy<10){
        return;
    }else{
        consumeEnergy(10);
    }
    if(m_enemy->getCurrentState()!=Ultimate){
        setZValue(1);
        m_enemy->setZValue(2);
    }
    CharacterBase::playSpecial();
}

void Gogetass::GetHit(int damage){
    if(m_currentState==Special) return;
    CharacterBase::GetHit(damage);
}
