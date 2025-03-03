#ifndef BUU_H
#define BUU_H


#include<CharacterBase.h>

class Buu: public CharacterBase
{
public:
    explicit Buu(double scale = 1.0, QGraphicsItem* parent = nullptr);
    void setupAnimations();
    void playMove() override;
    void playSpecial() override;
    void cheakHitBySpecial() override;
    QRectF collisionBoundingBox() override;
    void fire();
    void playDead() override;
    void GetHit(int damage) override;
private:
    void revive();
    QPixmap m_abnormalPixmap;
    QTimer* m_abnormalTimer;
    int deadTime=0;
};

#endif // BUU_H
