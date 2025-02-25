#ifndef BULLET_H
#define BULLET_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Bullet(QGraphicsPixmapItem* enemy,
                   QPointF startPos,
                   QPointF direction,
                   qreal speed,
                   QGraphicsItem* parent = nullptr);

    void startMoving();
    void setCollisionCheckEnabled(bool enabled);

signals:
    void hitTarget();
    void bulletExpired();

private slots:
    void updatePosition();

private:
    void checkCollisions();

    QGraphicsPixmapItem* m_enemy;
    QPointF m_direction;
    qreal m_speed;
    QTimer* m_moveTimer;
    bool m_collisionCheck = true;
};

#endif // BULLET_H
