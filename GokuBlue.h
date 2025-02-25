#ifndef GOKUBLUE_H
#define GOKUBLUE_H


#include<CharacterBase.h>

class GokuBlue: public CharacterBase
{
public:
    explicit GokuBlue(double scale = 1.0, QGraphicsItem* parent = nullptr);
    void setupAnimations();
    void playMove() override;
    void playSpecial() override;
};

#endif // GOKUBLUE_H
