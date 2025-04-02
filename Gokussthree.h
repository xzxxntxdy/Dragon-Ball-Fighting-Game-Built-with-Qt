#ifndef GOKUSSTHREE_H
#define GOKUSSTHREE_H

#include<CharacterBase.h>

class Gokussthree: public CharacterBase
{
public:
    explicit Gokussthree(double scale = 1.0, QGraphicsItem* parent = nullptr);
    QPointF checkCollision(QPointF newPos) override;
    void cheakHitByUltimate() override;
    void cheakHitBySpecial() override;
    void setupAnimations();
    void playMove() override;
    void playSpecial() override;
    void playUltimate() override;
};
#endif // GOKUSSTHREE_H
