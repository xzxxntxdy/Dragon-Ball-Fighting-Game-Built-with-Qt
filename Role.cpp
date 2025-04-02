#include "Role.h"

Role::Role(const QString& resourceDir, double scale ,QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
      m_scale(scale)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true); // 启用位置变化通知
    // 设置各动画参数
    setupAnimations(resourceDir);
    initComboSystem();
    initEnergySystem();
}

void Role::initEnergySystem(){
    m_energyRegenTimer = new QTimer(this);
    connect(m_energyRegenTimer, &QTimer::timeout, this, &Role::regenerateEnergy);
    m_energyRegenTimer->start(1000);
}

void Role::initComboSystem(){
    comboTimer=new QTimer(this);
    comboTimer->setSingleShot(true);
    connect(comboTimer,&QTimer::timeout,this,&Role::failedCombo);
    connect(this,&Role::startCombo,this,&Role::startComboTimer);
}

void Role::setupAnimations(const QString& resourceDir)
{
    // 初始化站立和移动状态图片
    m_idlePixmap = QPixmap(resourceDir + "pre/0.png");
    m_landPixmap = QPixmap(resourceDir + "move/2.png");
    m_floatPixmap = QPixmap(resourceDir + "move/0.png");
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
    setPixmap(m_idlePixmap);
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

    //死亡动画
    m_animations[Dead] = new AnimatedPixmapItem(
        this,
        resourceDir + "dead/",
        5
    );
    m_animations[Dead]->setFrameRate(5);
    m_animations[Dead]->setPerFrameMovement(QPointF(0,2));

    //设置比例
    if(m_scale!=1.0){
        foreach (auto anim, m_animations) {
            anim->setScaleFactor(m_scale);
        }
    }
}

void Role::switchAnimation(AnimationState newState)
{
    if (m_currentState == newState) return;

    // 停止当前动画
    if (m_currentState!=Idle&&m_animations.contains(m_currentState)) {
        m_animations[m_currentState]->stopAnimation();
    }

    // 启动新动画
    m_animations[newState]->startAnimation();
    m_currentState = newState;
}

void Role::playAttack()
{
    m_animations[Attack]->setPerFrameMovement(QPointF((m_transform ? -10 : 10), 0)); // 攻击前冲效果
    switchAnimation(Attack);
    connect(m_animations[Attack], &AnimatedPixmapItem::animationStopped,this,[this](){
                           m_animations[Attack]->setcombo(true);m_currentState=Idle;});
    connect(m_animations[Attack], &AnimatedPixmapItem::animationStopped,this,&Role::startCombo);
}

void Role::playCharge()
{
    switchAnimation(Charge);
}

void Role::playMove()
{
    if (m_energy >= 5) {
        consumeEnergy(5);
    }else{
        return;
    }
    m_animations[Move]->setPerFrameMovement(5,QPointF(m_moveDirection.x()*100,m_moveDirection.y()*100));
    switchAnimation(Move);
    connect(m_animations[Move], &AnimatedPixmapItem::animationStopped,this,&Role::backIdle);
}

void Role::playUltimate()
{
    if (m_energy >= 20) {
        consumeEnergy(20);
    }else{
        return;
    }
    this->setZValue(1);
    m_enemy->setZValue(2);
    m_animations[Ultimate]->setPerFramePos(3,
                                           QPointF((m_transform ? m_enemy->pos().x()+500+m_enemy->getDirection().x()*60
                                                   : m_enemy->pos().x()-500+m_enemy->getDirection().x()*60),
                                                   m_enemy->pos().y()+m_enemy->getDirection().y()*50));
    switchAnimation(Ultimate);
    connect(m_animations[Ultimate], &AnimatedPixmapItem::animationStopped,this,&Role::backIdle);
}

void Role::playHurt()
{
    m_animations[Hurt]->setPerFrameMovement(QPointF((m_enemy->m_transform ? -10 : 10),0));
    m_animations[Hurt]->setPerFrameMovement(6,QPointF((m_enemy->m_transform ? -10 : 10),50));
    m_animations[Hurt]->setPerFrameMovement(7,QPointF(0,-50));
    switchAnimation(Hurt);
    connect(m_animations[Hurt], &AnimatedPixmapItem::animationStopped,this,&Role::backIdle);
}

void Role::playDead()
{
    switchAnimation(Dead);
}

void Role::playSlowMove(){
    if(m_moveDirection.x()){
        setPixmap(m_landPixmap);
    }else{
        setPixmap(m_floatPixmap);
    }
    setPos(pos() + m_moveDirection);
}

void Role::stopAll()
{
    foreach (auto anim, m_animations) {
        anim->stopAnimation();
    }
    backIdle();
}

void Role::setMovement(const QPointF& direction)
{
    // 自动翻转角色朝向
    if (!direction.isNull()&&direction.x()&&m_moveDirection.x()!=direction.x()) {
        setTransform(QTransform().scale(direction.x() >= 0 ? 1 : -1, 1));
        if((!m_transform&&direction.x()<0)||(m_transform&&direction.x()>0))
        m_transform=!m_transform;
    }
    m_moveDirection = direction;
}

void Role::backIdle(){
    setPixmap(m_idlePixmap);
    m_currentState=Idle;
}

void Role::failedCombo(){
    m_animations[Attack]->setcombo(false);
    if(!isplaying()&&pixmap().size()!=m_floatPixmap.size()&&pixmap().size()!=m_landPixmap.size()&&m_currentState!=Charge){
        backIdle();
    }
}

bool Role::isplaying(){
    if(m_currentState!= Idle && m_currentState!= Charge) return true;
    else return false;
}

void Role::setEnemy(Role* enemy) {
    m_enemy = enemy;
    connect(m_animations[Attack],&AnimatedPixmapItem::animationUpdated,m_enemy,&Role::checkHitByAttack);
    connect(m_animations[Ultimate],&AnimatedPixmapItem::animationUpdated,m_enemy,&Role::checkHitByAttack);
}

QPointF Role::getDirection(){
    return m_moveDirection;
}

void Role::takeDamage(int damage)
{
    m_health = qMax(0, m_health - damage);
    emit healthChanged(m_health);
    if(m_health <= 0){
        playDead();
    }
}

void Role::setAttackDamage(int attackDamage){
    m_attackDamage=attackDamage;
}

void Role::setUltimateDamage(int ultimateDamage){
    m_ultimateDamage=ultimateDamage;
}

int Role::health() const
{
    return m_health;
}

QRectF Role::collisionBoundingBox() const {
    QSizeF size = boundingRect().size();
    return QRectF(
        m_transform ? pos().x()-size.width():pos().x(),
        pos().y(),
        size.width(),
        size.height()
    );
}

void Role::startComboTimer(){
    comboTimer->start(1000);
}


QPointF Role::checkCollision(QPointF newPos){
    // 临时计算新碰撞盒
    QRectF newCollision = collisionBoundingBox().translated(newPos - pos());

    // 与敌人碰撞检测
    if (m_enemy && newCollision.intersects(m_enemy->collisionBoundingBox())) {
        // 计算排斥方向
        QLineF pushDir(m_enemy->collisionBoundingBox().center(),
            newCollision.center());
            pushDir.setLength(1.0); // 单位向量

        // 逐步调整位置直到不重叠
        while (newCollision.intersects(m_enemy->collisionBoundingBox())) {
            newPos += pushDir.p2() - pushDir.p1();
            newCollision.translate(pushDir.p2() - pushDir.p1());
        }
    }
    return newPos;
}

void Role::checkHitByAttack() {
    if (!m_enemy || m_currentState == Hurt || m_currentState == Dead)
        return;
    bool hit = false;
    // 检测敌方攻击碰撞
    if (m_enemy->m_currentState==Attack) {
        auto attackAnim = m_enemy->m_animations.value(Role::Attack);
        QRectF attackArea = attackAnim->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(collisionBoundingBox())) {
            m_animations[Hurt]->setEndFrameNum(5);
            takeDamage(m_attackDamage);  // 普通攻击扣血
            hit = true;
        }
    }
    // 检测敌方大招碰撞（同理可添加其他技能）
    if (m_enemy->m_currentState==Ultimate) {
        auto ultimateAnim = m_enemy->m_animations.value(Role::Ultimate);
        QRectF attackArea = ultimateAnim->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(collisionBoundingBox())) {
            m_animations[Hurt]->setEndFrameNum(-1);
            takeDamage(m_ultimateDamage);  // 大招扣血
            hit = true;
        }
    }
    if(hit && m_currentState != Dead){
        playHurt();
    }
}

void Role::addEnergy(int amount) {
    m_energy = qMin(m_energy + amount, MAX_ENERGY);
    emit energyChanged(m_energy);
}

void Role::consumeEnergy(int amount) {
    if (m_energy >= amount) {
        m_energy -= amount;
        emit energyChanged(m_energy);
    }
}

int Role::energy() const {
    return m_energy;
}

void Role::regenerateEnergy() {
    if (m_currentState == Charge) {
        addEnergy(ENERGY_REGEN_CHARGE);
    } else {
        addEnergy(ENERGY_REGEN_NORMAL);
    }
}

QVariant Role::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();

        if(m_currentState==Move||m_currentState==Idle||m_currentState==Attack){
            newPos=checkCollision(newPos);
        }


        QRectF sceneRect = scene()->sceneRect();
        QSizeF size = boundingRect().size();

        // 计算允许的最大坐标
        qreal maxX;
        qreal maxY = sceneRect.bottom() - size.height();
        qreal minX;
        if(m_transform){
            minX = sceneRect.left() + size.width();
            maxX = sceneRect.right();
        }
        else{
            minX = sceneRect.left();
            maxX = sceneRect.right() - size.width();
        }

        // 限制坐标在边界内
        newPos.setX(qMax(minX, qMin(newPos.x(), maxX)));
        newPos.setY(qMax(sceneRect.top(), qMin(newPos.y(), maxY)));

        return newPos;
    }
    return QGraphicsPixmapItem::itemChange(change, value);
}
