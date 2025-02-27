#include "GokuRed.h"

GokuRed::GokuRed(double scale,QGraphicsItem* parent)
    :CharacterBase(scale,parent)
{
    m_attackHurtNum=4;
    m_specialHurtNum=3;
    setupAnimations();
}

void GokuRed::setupAnimations(){
    const QString resourceDir=":/ima/gokured/";
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
        20
    );
    m_animations[Attack]->setFrameRate(8);
    m_animations[Attack]->setEndFrameNum(5);
    m_animations[Attack]->setFrameDuration(4,300);
    m_animations[Attack]->setFrameDuration(9,300);
    m_animations[Attack]->setFrameDuration(14,300);
    m_animations[Attack]->setFrameDuration(19,800);
    m_animations[Attack]->setPerFrameMovement(QPointF(10, 0)); // 攻击前冲效果
    // 设置攻击有效帧
    for (int i = 1; i <= 19; i+=5) {
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
    m_animations[Charge]->setPerFrameMovement(0,QPointF(0,1));
    m_animations[Charge]->setPerFrameMovement(1,QPointF(0,-1));

    // 大招动画
    m_animations[Ultimate] = new AnimatedPixmapItem(
        this,
        resourceDir + "ultimate/",
        7
    );
    m_animations[Ultimate]->setFrameRate(8);
    m_animations[Ultimate]->setFrameDuration(4,200);
    m_animations[Ultimate]->setFrameDuration(5,1000);
    m_animations[Ultimate]->setPerFrameMovement(4,QPointF(0,-20));
    m_animations[Ultimate]->setPerFrameMovement(5,QPointF(0,-60));
    m_animations[Ultimate]->setPerFrameMovement(6,QPointF(0,80));
    //设置大招有效帧
    m_animations[Ultimate]->setFrameCollision(5, QRectF(0, -50, 850, 250));

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
    m_animations[Dead]->setPerFrameMovement(QPointF(0,2));

    //special动画
    m_animations[Special] = new AnimatedPixmapItem{
        this,
        resourceDir + "special/",
        11
    };

    m_animations[Special]->setFrameRate(15);
    m_animations[Special]->setFrameDuration(0,300);
    m_animations[Special]->setPerFrameMovement(0,QPointF(-10,0));
    m_animations[Special]->setPerFrameMovement(1,QPointF(10,0));
    m_animations[Special]->setPerFrameMovement(2,QPointF(-44,-60));
    m_animations[Special]->setFrameDuration(2,500);
    m_animations[Special]->setFrameDuration(3,500);
    m_animations[Special]->setSegmentLoop(2,3,3);
    m_animations[Special]->setPerFrameMovement(4,QPointF(-7,-40));
    m_animations[Special]->setPerFrameMovement(5,QPointF(6,20));
    m_animations[Special]->setPerFrameMovement(6,QPointF(17,80));
    m_animations[Special]->setPerFrameMovement(7,QPointF(-10,0));
    m_animations[Special]->setPerFrameMovement(8,QPointF(-10,0));
    m_animations[Special]->setPerFrameMovement(9,QPointF(26,-66));
    m_animations[Special]->setPerFrameMovement(10,QPointF(24,60));
    m_animations[Special]->setFrameDuration(4,1500);
    m_animations[Special]->setFrameDuration(5,20);
    m_animations[Special]->setFrameDuration(9,1000);
    m_animations[Special]->setFrameDuration(10,10);
    connect(m_animations[Special], &AnimatedPixmapItem::animationStopped,this,&GokuRed::change);

    //entrance动画
    m_animations[Entrance] = new AnimatedPixmapItem(
        this,
        resourceDir + "entrance/",
        13
    );
    m_animations[Entrance]->setFrameRate(15);
    m_animations[Entrance]->setFrameDuration(0,500);
    m_animations[Entrance]->setFrameDuration(2,500);
    m_animations[Entrance]->setFrameDuration(3,750);
    m_animations[Entrance]->setFrameDuration(4,500);
    m_animations[Entrance]->setFrameDuration(9,1000);
    m_animations[Entrance]->setFrameDuration(10,750);
    m_animations[Entrance]->setFrameDuration(12,1000);
    m_animations[Entrance]->setPerFrameMovement(1,QPointF(-3,-10));
    m_animations[Entrance]->setPerFrameMovement(2,QPointF(-23,-16));
    m_animations[Entrance]->setPerFrameMovement(3,QPointF(0,-16));
    m_animations[Entrance]->setPerFrameMovement(4,QPointF(0,12));
    m_animations[Entrance]->setPerFrameMovement(5,QPointF(0,26));
    m_animations[Entrance]->setPerFrameMovement(6,QPointF(31,6));
    m_animations[Entrance]->setPerFrameMovement(7,QPointF(-10,0));
    m_animations[Entrance]->setPerFrameMovement(9,QPointF(-22,-60));
    m_animations[Entrance]->setPerFrameMovement(10,QPointF(17,20));
    m_animations[Entrance]->setPerFrameMovement(11,QPointF(21,54));

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void GokuRed::playMove(){
    m_animations[Move]->setPerFrameMovement(5,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    CharacterBase::playMove();
}

void GokuRed::playUltimate(){
    m_animations[Ultimate]->setPerFramePos(3,
                                           QPointF((m_transform ? m_enemy->pos().x()+500+m_enemy->getDirection().x()*60
                                                   : m_enemy->pos().x()-500+m_enemy->getDirection().x()*60),
                                                   m_enemy->pos().y()+m_enemy->getDirection().y()*50));
    CharacterBase::playUltimate();
}

void GokuRed::playSpecial(){
    if(m_energy<20||m_health>30) return;
    consumeEnergy(20);
    CharacterBase::playSpecial();
}

void GokuRed::GetHit(int damage){
    if(m_currentState==Special) return;
    CharacterBase::GetHit(damage);
}
