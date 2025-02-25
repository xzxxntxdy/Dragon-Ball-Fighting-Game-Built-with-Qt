#ifndef FRIEZA_H
#define FRIEZA_H

#include<CharacterBase.h>

class Frieza: public CharacterBase
{
public:
    explicit Frieza(double scale = 1.0, QGraphicsItem* parent = nullptr);
    void setupAnimations();
    void playMove() override;
    void playUltimate() override;
    void fire();
    void playSpecial() override;
    void GetHit(int damage) override;
};
#endif // FRIEZA_H
