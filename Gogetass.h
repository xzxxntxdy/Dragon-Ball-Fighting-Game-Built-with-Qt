#ifndef GOGETASS_H
#define GOGETASS_H


#include<CharacterBase.h>

class Gogetass: public CharacterBase
{
public:
    explicit Gogetass(double scale = 1.0, QGraphicsItem* parent = nullptr);
    void setupAnimations();
    void playMove() override;
    void playSpecial() override;
    QPointF checkCollision(QPointF newPos) override;
    void cheakHitBySpecial() override;
    void GetHit(int damage) override;
};

#endif // GOGETASS_H
