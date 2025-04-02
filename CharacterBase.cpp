#include "CharacterBase.h"

CharacterBase::CharacterBase(double scale, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
      m_scale(scale)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    initEnergySystem();
    initComboSystem();
    setSoundEffect();
}

void CharacterBase::setSoundEffect(){
    m_soundEffect[Charge]=new SoundEffect(this);
    m_soundEffect[Charge]->load("qrc:/sound/sound/charge.wav");
    m_soundEffect[Charge]->setLoop(true);
    m_soundEffect[Charge]->setVolume(30);
    m_soundEffect[Move]=new SoundEffect(this);
    m_soundEffect[Move]->load("qrc:/sound/sound/move.wav");
    m_soundEffect[Move]->setPlaybackSpeed(1.5);
    m_soundEffect[Move]->setVolume(40);
}

void CharacterBase::initEnergySystem(){
    m_energyRegenTimer = new QTimer(this);
    connect(m_energyRegenTimer, &QTimer::timeout, this, &CharacterBase::regenerateEnergy);
    m_energyRegenTimer->start(1000);
}

void CharacterBase::initComboSystem(){
    comboTimer=new QTimer(this);
    comboTimer->setSingleShot(true);
    connect(comboTimer,&QTimer::timeout,this,&CharacterBase::failedCombo);
    connect(this,&CharacterBase::startCombo,this,&CharacterBase::startComboTimer);
}

void CharacterBase::switchAnimation(AnimationState newState)
{
    if (m_currentState == newState) return;

    // 停止当前动画
    if (m_currentState!=Idle&&m_animations.contains(m_currentState)) {
        m_animations[m_currentState]->stopAnimation();
        if(m_soundEffect.contains(m_currentState)){
            m_soundEffect[m_currentState]->stop();
        }
    }

    // 启动新动画
    m_currentState = newState;
    if(m_soundEffect.contains(m_currentState)){
        m_soundEffect[m_currentState]->play();
    }
    m_animations[newState]->startAnimation();
}

void CharacterBase::playAttack()
{
    m_animations[Attack]->transform(m_transform);
    switchAnimation(Attack);
    connect(m_animations[Attack], &AnimatedPixmapItem::animationStopped,this,[this](){
                           m_animations[Attack]->setcombo(true);m_currentState=Idle;});
    connect(m_animations[Attack], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::startCombo);
}

void CharacterBase::playCharge()
{
    m_animations[Charge]->transform(m_transform);
    switchAnimation(Charge);
}

void CharacterBase::playMove()
{
    if (m_energy >= 5) {
        consumeEnergy(5);
    }else{
        return;
    }
    switchAnimation(Move);
    connect(m_animations[Move], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);
}

void CharacterBase::playUltimate()
{
    if (m_energy >= 20) {
        consumeEnergy(20);
    }else{
        return;
    }
    m_animations[Ultimate]->transform(m_transform);
    this->setZValue(1);
    m_enemy->setZValue(2);
    switchAnimation(Ultimate);
    connect(m_animations[Ultimate], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);
}

void CharacterBase::playSpecial(){
    m_animations[Special]->transform(m_transform);
    switchAnimation(Special);
}

void CharacterBase::playHurt()
{
    m_animations[Hurt]->transform(m_enemy->getTransform());
    switchAnimation(Hurt);
    connect(m_animations[Hurt], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);
}

void CharacterBase::playEntrance()
{
    m_animations[Entrance]->transform(m_transform);
    switchAnimation(Entrance);
    connect(m_animations[Entrance], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::backIdle);
    connect(m_animations[Entrance], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::entranceFinished);
}

void CharacterBase::playDead()
{
    m_animations[Dead]->transform(m_transform);
    switchAnimation(Dead);
    connect(m_animations[Dead], &AnimatedPixmapItem::animationStopped,this,&CharacterBase::roundEnded);
}

void CharacterBase::playSlowMove(){
    if(m_moveDirection.x()){
        setPixmap(m_landPixmap);
    }else{
        setPixmap(m_floatPixmap);
    }
    setPos(pos() + m_moveDirection);
}

void CharacterBase::stopAll()
{
    foreach (auto anim, m_animations) {
        anim->stopAnimation();
    }
    foreach (auto sound, m_soundEffect) {
        sound->stop();
    }
    backIdle();
}

void CharacterBase::setMovement(const QPointF& direction)
{
    // 自动翻转角色朝向
    if (!direction.isNull()&&direction.x()&&m_moveDirection.x()!=direction.x()) {
        setTransform(QTransform().scale(direction.x() >= 0 ? 1 : -1, 1));
        if((!m_transform&&direction.x()<0)||(m_transform&&direction.x()>0))
        m_transform=!m_transform;
    }
    m_moveDirection = direction;
}

void CharacterBase::backIdle(){
    setPixmap(m_idlePixmap);
    m_currentState=Idle;
}

void CharacterBase::failedCombo(){
    m_animations[Attack]->setcombo(false);
    if(!isplaying()&&pixmap().size()!=m_floatPixmap.size()&&pixmap().size()!=m_landPixmap.size()&&m_currentState!=Charge){
        backIdle();
    }
}

bool CharacterBase::isplaying(){
    if(m_currentState!= Idle && m_currentState!= Charge) return true;
    else return false;
}

void CharacterBase::setState(AnimationState state){
    m_currentState=state;
}

int CharacterBase::getCurrentState() const{
    return m_currentState;
}

void CharacterBase::setEnemy(CharacterBase* enemy) {
    m_enemy = enemy;
    connect(m_animations[Attack],&AnimatedPixmapItem::animationUpdated,this,&CharacterBase::checkHit);
    connect(m_animations[Ultimate],&AnimatedPixmapItem::animationUpdated,this,&CharacterBase::checkHit);
    connect(m_animations[Special],&AnimatedPixmapItem::animationUpdated,this,&CharacterBase::checkHit);
    connect(this,&CharacterBase::Hit,m_enemy,&CharacterBase::GetHit);
}

QPointF CharacterBase::getDirection(){
    return m_moveDirection;
}

bool CharacterBase::getTransform(){
    return m_transform;
}

void CharacterBase::takeDamage(int damage)
{
    m_health = qMax(0, m_health - damage);
    emit healthChanged(m_health);
    if(m_health <= 0){
        playDead();
    }
}

void CharacterBase::setHealth(int health){
    m_health=qMin(health,MAX_HEALTH);
    emit healthChanged(m_health);
}

void CharacterBase::setAttackDamage(int attackDamage){
    m_attackDamage=attackDamage;
}

void CharacterBase::setUltimateDamage(int ultimateDamage){
    m_ultimateDamage=ultimateDamage;
}

int CharacterBase::health() const
{
    return m_health;
}

QRectF CharacterBase::collisionBoundingBox(){
    QSizeF size = boundingRect().size();
    return QRectF(
        m_transform ? pos().x()-size.width():pos().x(),
        pos().y(),
        size.width(),
        size.height()
    );
}

void CharacterBase::startComboTimer(){
    comboTimer->start(1000);
}


QPointF CharacterBase::checkCollision(QPointF newPos){
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

void CharacterBase::checkHit() {
    if (!m_enemy || m_enemy->m_currentState == Hurt || m_enemy->m_currentState == Dead)
        return;
    cheakHitByAttack();
    cheakHitByUltimate();
    cheakHitBySpecial();
}

void CharacterBase::GetHit(int damage){
    takeDamage(damage);
    if(m_currentState != Dead){
        playHurt();
    }
}

void CharacterBase::cheakHitByAttack(){
    if (m_currentState==Attack) {
        QRectF attackArea = m_animations[Attack]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            m_enemy->m_animations[Hurt]->setEndFrameNum(m_enemy->m_attackHurtNum);
            emit Hit(m_attackDamage);
        }
    }
}

void CharacterBase::cheakHitByUltimate(){
    if (m_currentState==Ultimate) {
        QRectF attackArea = m_animations[Ultimate]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            m_enemy->m_animations[Hurt]->setEndFrameNum(-1);
            emit Hit(m_ultimateDamage);
        }
    }
}

void CharacterBase::cheakHitBySpecial(){
    if (m_currentState==Special) {
        QRectF attackArea = m_animations[Special]->currentCollisionRect();
        if (!attackArea.isEmpty() && attackArea.intersects(m_enemy->collisionBoundingBox())) {
            m_enemy->m_animations[Hurt]->setEndFrameNum(m_enemy->m_specialHurtNum);
            emit Hit(m_specialDamage);
        }
    }
}

void CharacterBase::addEnergy(int amount) {
    m_energy = qMin(m_energy + amount, MAX_ENERGY);
    emit energyChanged(m_energy);
}

void CharacterBase::consumeEnergy(int amount) {
    if (m_energy >= amount) {
        m_energy -= amount;
        emit energyChanged(m_energy);
    }
}

int CharacterBase::energy() const {
    return m_energy;
}

void CharacterBase::regenerateEnergy() {
    if (m_currentState == Charge) {
        addEnergy(ENERGY_REGEN_CHARGE);
    } else {
        addEnergy(ENERGY_REGEN_NORMAL);
    }
}

void CharacterBase::setEnergy(int energy){
    m_energy=energy;
    emit energyChanged(m_energy);
}

QPointF CharacterBase::calculateDirectionToEnemy(QPointF startPos) const
{
    if (!m_enemy) return QPointF(1, 0); // 默认向右

    // 计算敌人位置
    QPointF enemyPos = QPointF(m_enemy->collisionBoundingBox().center().x(),
                               m_enemy->collisionBoundingBox().center().y()-m_bulletPixmap.height()/2);

    // 计算方向向量并归一化
    QPointF direction = enemyPos - startPos;
    qreal length = qSqrt(direction.x()*direction.x() + direction.y()*direction.y());
    return length > 0 ? direction / length : QPointF(1, 0);
}

void CharacterBase::fireBullet(qreal speed, int damage, QPointF startPos, bool toEnemy)
{
    if (!scene()) return;

    // 获取方向
    QPointF direction;
    if(toEnemy){
        direction = calculateDirectionToEnemy(startPos);
    }else{
        direction = QPointF((m_transform? -1: 1),0);
    }

    // 创建子弹
    Bullet* bullet = new Bullet(m_enemy, startPos, direction, speed);
    bullet->setPixmap(m_bulletPixmap);
    bullet->setTransformOriginPoint(bullet->boundingRect().left(),bullet->boundingRect().center().y());

    // 设置子弹旋转角度
    qreal angle = qRadiansToDegrees(qAtan2(direction.y(), direction.x()));
    bullet->setRotation(angle);

    scene()->addItem(bullet);
    bullet->startMoving();

    // 连接信号
    connect(bullet, &Bullet::hitTarget, this, [=]() {
        if (!m_enemy || m_currentState == Hurt || m_currentState == Dead)
            return;
        m_enemy->m_animations[Hurt]->setEndFrameNum(-1);
        emit Hit(damage);
    });
}

QVariant CharacterBase::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
        QPointF newPos = value.toPointF();

        if(m_currentState!=Ultimate && m_currentState!=Hurt &&m_currentState!=Dead){
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
