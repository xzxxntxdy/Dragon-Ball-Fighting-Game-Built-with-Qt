#include "GokuBlue.h"

GokuBlue::GokuBlue(double scale,QGraphicsItem* parent)
    :CharacterBase(scale,parent)
{
    m_attackHurtNum=3;
    m_specialHurtNum=3;
    m_ultimateDamage=30;
    m_specialDamage=10;
    setupAnimations();
}

void GokuBlue::setupAnimations(){
    const QString resourceDir=":/ima/gokublue/";
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
        17
    );
    m_animations[Attack]->setFrameRate(8);
    m_animations[Attack]->setEndFrameNum(5);
    m_animations[Attack]->setFrameDuration(4,300);
    m_animations[Attack]->setFrameDuration(9,300);
    m_animations[Attack]->setFrameDuration(14,300);
    m_animations[Attack]->setFrameDuration(16,1000);
    m_animations[Attack]->setPerFrameMovement(QPointF(10, 0)); // 攻击前冲效果
    // 设置攻击有效帧
    for (int i = 0; i <= 16; i+=5) {
        m_animations[Attack]->setFrameCollision(i, QRectF(40, 20, 100, 70));
        m_animations[Attack]->setFrameCollision(i+1, QRectF(40, 20, 100, 70));
    }

    // 移动动画
    m_animations[Move] = new AnimatedPixmapItem(
        this,
        resourceDir + "move/",
        9
    );
    m_animations[Move]->setFrameRate(30);
    m_animations[Move]->setPerFrameMovement(8,QPointF(0,-1));

    // 聚气动画
    m_animations[Charge] = new AnimatedPixmapItem(
        this,
        resourceDir + "charge/",
        2
    );
    m_animations[Charge]->setLooping(true);
    m_animations[Charge]->setFrameRate(8);
    m_animations[Charge]->setPerFrameMovement(0,QPointF(0,1));
    m_animations[Charge]->setPerFrameMovement(1,QPointF(0,-1));

    // 大招动画
    m_animations[Ultimate] = new AnimatedPixmapItem(
        this,
        resourceDir + "ultimate/",
        8
    );
    m_animations[Ultimate]->setFrameRate(12);
    m_animations[Ultimate]->setPerFrameMovement(2,QPointF(-60,-18));
    m_animations[Ultimate]->setPerFrameMovement(3,QPointF(0,6));
    m_animations[Ultimate]->setPerFrameMovement(4,QPointF(54,18));
    m_animations[Ultimate]->setPerFrameMovement(6,QPointF(0,-65));
    m_animations[Ultimate]->setPerFrameMovement(7,QPointF(0,65));
    m_animations[Ultimate]->setFrameDuration(6,1000);
    //设置大招有效帧
    m_animations[Ultimate]->setFrameCollision(6, QRectF(0, -50, 750, 250));

    // 受击动画
    m_animations[Hurt] = new AnimatedPixmapItem(
        this,
        resourceDir + "hurt/",
        5
    );
    m_animations[Hurt]->setFrameRate(5);
    m_animations[Hurt]->setPerFrameMovement(QPointF(10,0));
    m_animations[Hurt]->setPerFrameMovement(3,QPointF(30,50));
    m_animations[Hurt]->setPerFrameMovement(4,QPointF(0,-50));
    m_animations[Hurt]->setFrameDuration(3,500);

    //死亡动画
    m_animations[Dead] = new AnimatedPixmapItem(
        this,
        resourceDir + "dead/",
        3
    );
    m_animations[Dead]->setFrameRate(5);

    //special动画
    m_animations[Special] = new AnimatedPixmapItem{
        this,
        resourceDir + "special/",
        5
    };
    m_animations[Special]->setFrameCollision(3, QRectF(0, 20, 250, 70));
    m_animations[Special]->setFrameCollision(4, QRectF(0, 20, 250, 70));
    m_animations[Special]->setFrameDuration(2,50);
    m_animations[Special]->setFrameDuration(0,175);
    m_animations[Special]->setFrameDuration(1,175);
    connect(m_animations[Special], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);

    //entrance动画
    m_animations[Entrance] = new AnimatedPixmapItem{
        this,
        resourceDir + "entrance/",
        11
    };

    m_animations[Entrance]->setFrameRate(15);
    m_animations[Entrance]->setFrameDuration(0,300);
    m_animations[Entrance]->setPerFrameMovement(0,QPointF(-10,0));
    m_animations[Entrance]->setPerFrameMovement(1,QPointF(10,0));
    m_animations[Entrance]->setPerFrameMovement(2,QPointF(-44,-60));
    m_animations[Entrance]->setFrameDuration(2,500);
    m_animations[Entrance]->setFrameDuration(3,500);
    m_animations[Entrance]->setSegmentLoop(2,3,3);
    m_animations[Entrance]->setPerFrameMovement(4,QPointF(-7,-40));
    m_animations[Entrance]->setPerFrameMovement(5,QPointF(6,20));
    m_animations[Entrance]->setPerFrameMovement(6,QPointF(17,80));
    m_animations[Entrance]->setPerFrameMovement(7,QPointF(-10,0));
    m_animations[Entrance]->setPerFrameMovement(8,QPointF(-10,0));
    m_animations[Entrance]->setPerFrameMovement(9,QPointF(26,-66));
    m_animations[Entrance]->setPerFrameMovement(10,QPointF(24,60));
    m_animations[Entrance]->setFrameDuration(4,1500);
    m_animations[Entrance]->setFrameDuration(5,20);
    m_animations[Entrance]->setFrameDuration(9,1000);
    m_animations[Entrance]->setFrameDuration(10,10);

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void GokuBlue::playMove(){
    m_animations[Move]->setPerFrameMovement(6,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    CharacterBase::playMove();
}

void GokuBlue::playSpecial(){
    if(m_health<=10) return;
    takeDamage(10);
    m_animations[Special]->setPerFramePos(3,
                                           QPointF((m_transform ? m_enemy->collisionBoundingBox().right()+174+m_enemy->getDirection().x()*45
                                                   : m_enemy->collisionBoundingBox().left()-174+m_enemy->getDirection().x()*45),
                                                   m_enemy->pos().y()+m_enemy->getDirection().y()*45));
    CharacterBase::playSpecial();
}

