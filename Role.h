#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "AnimatedPixmapItem.h"
#include <QTimer>
#include <QGraphicsScene>

class Role : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum AnimationState {
        Idle,
        Move,
        Attack,
        Charge,
        Ultimate,
        Hurt,
        Dead
    };

    explicit Role(const QString& resourceDir, double scale = 1.0 ,QGraphicsItem* parent = nullptr);

    // 动画控制接口
    void playAttack();
    void playCharge();
    void playMove();
    void playUltimate();
    void playHurt();
    void playDead();
    void playSlowMove();
    void stopAll();
    void setupAnimations(const QString& resourceDir);
    void switchAnimation(AnimationState newState);

    // 位置控制
    void setMovement(const QPointF& direction);
    QPointF getDirection();

    // 碰撞系统
    QRectF collisionBoundingBox() const;
    QPointF checkCollision(QPointF newPos);

    //敌人设置
    void setEnemy(Role* enemy);

    //状态检测
    bool isplaying();

    //血量系统
    int health() const;
    void takeDamage(int damage);
    void setAttackDamage(int attackDamage);
    void setUltimateDamage(int ultimateDamage);

    //连击系统
    void startComboTimer();
    void initComboSystem();

    //精力系统
    void addEnergy(int amount);  // 增加精力
    void consumeEnergy(int amount);  // 消耗精力
    int energy() const;  // 获取当前精力
    void initEnergySystem();

signals:
    void startCombo();
    void healthChanged(int health);
    void energyChanged(int energy);

public slots:
    void backIdle();
    void failedCombo();
    void checkHitByAttack();
    void regenerateEnergy();

private:
    QMap<AnimationState, AnimatedPixmapItem*> m_animations;
    AnimationState m_currentState = Idle;
    QPointF m_moveDirection=QPointF(0,0);
    QPixmap m_idlePixmap;
    QPixmap m_landPixmap;
    QPixmap m_floatPixmap;
    double m_scale;
    bool m_transform=false;
    Role* m_enemy=nullptr;
    int m_health = 100;
    int m_attackDamage=5;
    int m_ultimateDamage=20;
    QTimer *comboTimer;
    int m_energy = 10;  // 初始精力值
    const int MAX_ENERGY = 50;  // 精力上限
    const int ENERGY_REGEN_NORMAL = 1;  // 普通状态每秒回复精力
    const int ENERGY_REGEN_CHARGE = 5;  // 聚气状态每秒回复精力
    const int MAX_HEALTH = 100;
    QTimer* m_energyRegenTimer;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // ROLE_H
