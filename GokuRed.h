#ifndef GOKURED_H
#define GOKURED_H

#include<CharacterBase.h>

class GokuRed: public CharacterBase
{
public:
    explicit GokuRed(double scale = 1.0, QGraphicsItem* parent = nullptr);
    void setupAnimations();
    void playMove() override;
    void playUltimate() override;
    void playSpecial() override;
    void GetHit(int damage) override;
};

#endif // GOKURED_H
