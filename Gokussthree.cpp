#include "Gokussthree.h"

Gokussthree::Gokussthree(double scale,QGraphicsItem* parent)
    :CharacterBase(scale,parent)
{
    m_attackHurtNum=4;
    m_specialHurtNum=3;
    m_specialDamage=10;
    setupAnimations();
}

void Gokussthree::setupAnimations(){
    const QString resourceDir=":/ima/gokuss3/";
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
    // 攻击动画
    m_animations[Attack] = new AnimatedPixmapItem(
        this,
        resourceDir + "attack/",
        22
    );
    m_animations[Attack]->setFrameRate(10);
    m_animations[Attack]->setEndFrameNum(6);
    m_animations[Attack]->setFrameDuration(5,300);
    m_animations[Attack]->setFrameDuration(11,300);
    m_animations[Attack]->setFrameDuration(17,300);
    m_animations[Attack]->setFrameDuration(21,900);
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
    m_animations[Charge]->setFrameRate(8);
    m_animations[Charge]->setPerFrameMovement(0,QPointF(6,0));
    m_animations[Charge]->setPerFrameMovement(1,QPointF(-6,0));

    // 大招动画
    m_animations[Ultimate] = new AnimatedPixmapItem(
        this,
        resourceDir + "ultimate/",
        19
    );
    m_animations[Ultimate]->setFrameRate(8);
    m_animations[Ultimate]->setFrameCollision(6, QRectF(40, 20, 150, 70));
    m_animations[Ultimate]->setFrameCollision(10, QRectF(40, 20, 150, 70));
    m_animations[Ultimate]->setFrameCollision(17, QRectF(0, -100, 1000, 300));
    m_animations[Ultimate]->setSegmentLoop(1,2,2);
    m_animations[Ultimate]->setFrameDuration(4,75);
    m_animations[Ultimate]->setFrameDuration(5,75);
    m_animations[Ultimate]->setFrameDuration(7,500);
    m_animations[Ultimate]->setFrameDuration(11,500);
    m_animations[Ultimate]->setFrameDuration(17,1000);
    m_animations[Ultimate]->setPerFrameMovement(1,QPointF(0,-50));
    m_animations[Ultimate]->setPerFrameMovement(15,QPointF(-80,-52));
    m_animations[Ultimate]->setPerFrameMovement(16,QPointF(55,52));
    m_animations[Ultimate]->setPerFrameMovement(17,QPointF(0,-80));
    m_animations[Ultimate]->setPerFrameMovement(18,QPointF(0,80));
    connect(m_animations[Ultimate],&AnimatedPixmapItem::animationUpdated,this,[this](){
        m_animations[Ultimate]->setPerFramePos(4,
                                               QPointF((m_transform ? m_enemy->collisionBoundingBox().right()+80
                                                       : m_enemy->collisionBoundingBox().left()-80),
                                                       m_enemy->pos().y()));
        m_animations[Ultimate]->setPerFramePos(9,
                                               QPointF((m_transform ? m_enemy->collisionBoundingBox().right()+80
                                                       : m_enemy->collisionBoundingBox().left()-80),
                                                       m_enemy->pos().y()));
        m_animations[Ultimate]->setPerFramePos(13,
                                               QPointF((m_transform ? m_enemy->pos().x()+500
                                                       : m_enemy->pos().x()-500),
                                                       m_enemy->pos().y()));
    });

    // 受击动画
    m_animations[Hurt] = new AnimatedPixmapItem(
        this,
        resourceDir + "hurt/",
        8
    );
    m_animations[Hurt]->setFrameRate(5);
    m_animations[Hurt]->setFrameDuration(6,500);
    m_animations[Hurt]->setPerFrameMovement(QPointF(10,0));
    m_animations[Hurt]->setPerFrameMovement(6,QPointF(10,50));
    m_animations[Hurt]->setPerFrameMovement(7,QPointF(0,-50));

    //死亡动画
    m_animations[Dead] = new AnimatedPixmapItem(
        this,
        resourceDir + "dead/",
        5
    );
    m_animations[Dead]->setFrameRate(5);
    m_animations[Dead]->setPerFrameMovement(2,QPointF(10,0));
    m_animations[Dead]->setPerFrameMovement(3,QPointF(0,12));
    m_animations[Dead]->setPerFrameMovement(4,QPointF(0,12));

    //special动画
    m_animations[Special] = new AnimatedPixmapItem{
        this,
        resourceDir + "special/",
        7
    };

    m_animations[Special]->setFrameCollision(5, QRectF(0, -100, 350, 200));
    m_animations[Special]->setPerFrameMovement(5,QPointF(0,-130));
    m_animations[Special]->setPerFrameMovement(6,QPointF(0,150));
    m_animations[Special]->setFrameDuration(5,1000);
    connect(m_animations[Special], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);

    //entrance动画
    m_animations[Entrance] = new AnimatedPixmapItem(
        this,
        resourceDir + "entrance/",
        27
    );
    m_animations[Entrance]->setFrameRate(8);
    m_animations[Entrance]->setFrameDuration(0,500);
    m_animations[Entrance]->setFrameDuration(3,500);
    m_animations[Entrance]->setFrameDuration(6,500);
    m_animations[Entrance]->setFrameDuration(23,300);
    m_animations[Entrance]->setFrameDuration(24,500);
    m_animations[Entrance]->setPerFrameMovement(2,QPointF(6,-8));
    m_animations[Entrance]->setPerFrameMovement(3,QPointF(-38,-42));
    m_animations[Entrance]->setPerFrameMovement(4,QPointF(-6,-2));
    m_animations[Entrance]->setPerFrameMovement(5,QPointF(-8,-6));
    m_animations[Entrance]->setPerFrameMovement(6,QPointF(14,8));
    m_animations[Entrance]->setPerFrameMovement(7,QPointF(-6,-2));
    m_animations[Entrance]->setPerFrameMovement(8,QPointF(6,2));
    m_animations[Entrance]->setPerFrameMovement(12,QPointF(-6,-2));
    m_animations[Entrance]->setPerFrameMovement(13,QPointF(-24,0));
    m_animations[Entrance]->setPerFrameMovement(23,QPointF(-39,-60));
    m_animations[Entrance]->setPerFrameMovement(24,QPointF(84,100));
    m_animations[Entrance]->setPerFrameMovement(25,QPointF(3,8));
    m_animations[Entrance]->setPerFrameMovement(26,QPointF(8,0));

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void Gokussthree::playMove(){
    m_animations[Move]->setPerFrameMovement(5,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    CharacterBase::playMove();
}

void Gokussthree::playSpecial(){
    if(m_energy<10){
        return;
    }else{
        consumeEnergy(10);
    }
    setZValue(1);
    m_enemy->setZValue(2);
    m_animations[Special]->setPerFramePos(4,
                                           QPointF((m_transform ? m_enemy->collisionBoundingBox().right()+140+m_enemy->getDirection().x()*40
                                                   : m_enemy->collisionBoundingBox().left()-140+m_enemy->getDirection().x()*40),
                                                   pos().y()));
    CharacterBase::playSpecial();
}

QPointF Gokussthree::checkCollision(QPointF newPos){
    if(m_currentState==Special) return newPos;
    return CharacterBase::checkCollision(newPos);
}

void Gokussthree::cheakHitByUltimate(){
    if (m_currentState==Ultimate) {
        QRectF attackArea = m_animations[Ultimate]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            switch (m_animations[Ultimate]->getCurrentFrame()) {
            case 6:
                m_enemy->m_animations[Hurt]->setEndFrameNum(3);
                emit Hit(5);
                break;
            case 10:
                m_enemy->m_animations[Hurt]->setEndFrameNum(3);
                emit Hit(5);
                break;
            case 17:
                m_enemy->m_animations[Hurt]->setEndFrameNum(-1);
                emit Hit(20);
                break;
            }
        }
    }
}

void Gokussthree::cheakHitBySpecial(){
    if (m_currentState==Special) {
        QRectF attackArea = m_animations[Special]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            m_enemy->m_animations[Hurt]->setEndFrameNum(-1);
            emit Hit(m_specialDamage);
        }
    }
}
