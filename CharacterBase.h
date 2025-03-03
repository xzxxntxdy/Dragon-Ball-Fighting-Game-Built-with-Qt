#ifndef CHARACTERBASE_H
#define CHARACTERBASE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "AnimatedPixmapItem.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QtMath>
#include "Bullet.h"

class CharacterBase : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum AnimationState {
        Idle,
        Move,
        Attack,
        Charge,
        Ultimate,
        Special,
        Hurt,
        Dead,
        Entrance,
        Abnormal
    };

    explicit CharacterBase(double scale = 1.0,QGraphicsItem* parent = nullptr);
    virtual ~CharacterBase() = default;

    // 动画控制接口
    QMap<AnimationState, AnimatedPixmapItem*> m_animations;
    virtual void playAttack();
    virtual void playCharge();
    virtual void playMove();
    virtual void playUltimate();
    virtual void playHurt();
    virtual void playDead();
    virtual void playSpecial();
    virtual void playEntrance();
    void playSlowMove();
    void stopAll();
    void switchAnimation(AnimationState newState);

    // 位置控制
    void setMovement(const QPointF& direction);
    QPointF getDirection();
    bool getTransform();

    // 碰撞系统
    virtual QRectF collisionBoundingBox();
    virtual QPointF checkCollision(QPointF newPos);
    virtual void cheakHitByAttack();
    virtual void cheakHitByUltimate();
    virtual void cheakHitBySpecial();

    //敌人设置
    void setEnemy(CharacterBase* enemy);

    //状态检测
    bool isplaying();
    void setState(AnimationState state);
    int getCurrentState() const;

    //血量系统
    int health() const;
    void takeDamage(int damage);
    void setAttackDamage(int attackDamage);
    void setUltimateDamage(int ultimateDamage);
    void setHealth(int health);

    //连击系统
    void startComboTimer();

    //精力系统
    void addEnergy(int amount);  // 增加精力
    void consumeEnergy(int amount);  // 消耗精力
    int energy() const;  // 获取当前精力
    void setEnergy(int energy);

    //子弹系统
    QPointF calculateDirectionToEnemy(QPointF startPos) const;
    void fireBullet(qreal speed, int damage, QPointF startPos, bool toEnemy=true);

signals:
    void startCombo();
    void healthChanged(int health);
    void energyChanged(int energy);
    void change();
    void entranceFinished();
    void roundEnded();
    void Hit(int damage);

public slots:
    virtual void checkHit();
    virtual void GetHit(int damage);
    void backIdle();
    void failedCombo();
    void regenerateEnergy();

private:
    QTimer *comboTimer;
    QTimer *m_energyRegenTimer;
    void initComboSystem();
    void initEnergySystem();

protected:
    AnimationState m_currentState = Idle;
    QPointF m_moveDirection=QPointF(0,0);
    QPixmap m_idlePixmap;
    QPixmap m_landPixmap;
    QPixmap m_floatPixmap;
    QPixmap m_bulletPixmap;
    double m_scale;
    CharacterBase* m_enemy=nullptr;
    int m_health = 100;
    int m_attackHurtNum;
    int m_specialHurtNum;
    int m_attackDamage=5;
    int m_ultimateDamage=20;
    int m_specialDamage;
    int m_energy = 0;  // 初始精力值
    const int MAX_ENERGY = 50;  // 精力上限
    const int ENERGY_REGEN_NORMAL = 1;  // 普通状态每秒回复精力
    const int ENERGY_REGEN_CHARGE = 5;  // 聚气状态每秒回复精力
    const int MAX_HEALTH = 100;
    bool m_transform=false;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // CHARACTERBASE_H
