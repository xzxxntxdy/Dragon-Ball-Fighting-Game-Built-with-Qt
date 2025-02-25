#include "Bullet.h"
#include <QGraphicsScene>

Bullet::Bullet(QGraphicsPixmapItem* enemy,
             QPointF startPos,
             QPointF direction,
             qreal speed,
             QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent),
      m_enemy(enemy), m_direction(direction),
      m_speed(speed)
{
    setPos(startPos);
    m_moveTimer = new QTimer(this);
    connect(m_moveTimer, &QTimer::timeout, this, &Bullet::updatePosition);
}

void Bullet::startMoving()
{
    m_moveTimer->start(10);
}

void Bullet::setCollisionCheckEnabled(bool enabled)
{
    m_collisionCheck = enabled;
}

void Bullet::updatePosition()
{
    // 更新位置
    setPos(pos() + m_direction * m_speed);

    // 场景边界检测
    if (!scene() || !scene()->sceneRect().contains(pos())) {
        emit bulletExpired();
        deleteLater();
        return;
    }

    // 碰撞检测
    if (m_collisionCheck) {
        checkCollisions();
    }
}

void Bullet::checkCollisions()
{
    if (!scene() || !m_enemy) return;
    QList<QGraphicsItem*> collidingItems = scene()->collidingItems(this);
    foreach (QGraphicsItem* item, collidingItems) {
        // 角色碰撞检测
        QGraphicsPixmapItem* character = qgraphicsitem_cast<QGraphicsPixmapItem*>(item);
        if (character && character == m_enemy) {
            if (this->collidesWithItem(character, Qt::IntersectsItemBoundingRect)) {
                emit hitTarget();
                deleteLater();
                return;
            }
        }

        // 新增子弹间碰撞检测
        Bullet* otherBullet = qgraphicsitem_cast<Bullet*>(item);
        if (otherBullet && otherBullet != this && otherBullet->m_enemy!=m_enemy) {
            // 比较子弹尺寸
            QSize thisSize = this->pixmap().size();
            QSize otherSize = otherBullet->pixmap().size();

            // 添加尺寸容差（±2像素）
            bool sizeMatch = qAbs(thisSize.width() - otherSize.width()) <= 2 &&
                            qAbs(thisSize.height() - otherSize.height()) <= 2;

            if (sizeMatch && this->collidesWithItem(otherBullet, Qt::IntersectsItemShape)) {
                // 双方互相销毁
                this->deleteLater();
                otherBullet->deleteLater();
                return;
            }
        }
    }
}
