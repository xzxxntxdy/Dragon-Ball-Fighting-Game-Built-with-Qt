#ifndef CHARACTERSELECTION_H
#define CHARACTERSELECTION_H

#include <QWidget>
#include <QMap>
#include <QGridLayout>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QVariant>
#include "SoundEffect.h"

enum CharacterType {
    GokuRedType,
    GokuBlueType,
    FriezaType,
    GokussthreeType,
    GogetassType,
    BuuType,
    Random
};

class CharacterSelection : public QWidget
{
    Q_OBJECT
public:
    explicit CharacterSelection(QWidget *parent = nullptr);
    void resetSelection();
    void stopBgm();
    void playBgm();

signals:
    void charactersSelected(CharacterType p1, CharacterType p2);

private:
    QMap<CharacterType, QToolButton*> m_buttons;
    QList<CharacterType> m_selected;
    QPushButton *resetbutton;
    void createCharacterButton(CharacterType type, const QPixmap& image, const QPoint& pos);
    void updateSelection();
    void updateButtonAppearance(QToolButton* btn, int count);
    SoundEffect* bgm;
};

#endif // CHARACTERSELECTION_H
