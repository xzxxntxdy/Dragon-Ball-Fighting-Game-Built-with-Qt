#include "CharacterSelection.h"

CharacterSelection::CharacterSelection(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1770, 1024);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/ima/select_bg.png").scaled(1770,1024)));
    setPalette(palette);
    setAutoFillBackground(true);

    createCharacterButton(BuuType, QPixmap(":/ima/buu/pre/0.png").scaled(size(),
                                                                         Qt::KeepAspectRatio,
                                                                         Qt::SmoothTransformation),
                                                                         QPoint(50, 300));
    createCharacterButton(GokuRedType, QPixmap(":/ima/gokured/pre/0.png").scaled(size(),
                                                                                 Qt::KeepAspectRatio,
                                                                                 Qt::SmoothTransformation),
                                                                                 QPoint(350, 300));
    createCharacterButton(GokuBlueType, QPixmap(":/ima/gokublue/pre/0.png").scaled(size(),
                                                                                   Qt::KeepAspectRatio,
                                                                                   Qt::SmoothTransformation),
                                                                                   QPoint(650, 300));
    createCharacterButton(FriezaType, QPixmap(":/ima/Frieza/pre/0.png").scaled(size(),
                                                                               Qt::KeepAspectRatio,
                                                                               Qt::SmoothTransformation),
                                                                               QPoint(950, 300));
    createCharacterButton(GokussthreeType, QPixmap(":/ima/gokuss3/pre/0.png").scaled(size(),
                                                                                     Qt::KeepAspectRatio,
                                                                                     Qt::SmoothTransformation),
                                                                                     QPoint(1250, 300));
    createCharacterButton(GogetassType, QPixmap(":/ima/gogetass/pre/0.png").scaled(size(),
                                                                                     Qt::KeepAspectRatio,
                                                                                     Qt::SmoothTransformation),
                                                                                     QPoint(1550, 300));

    resetbutton=new QPushButton(this);
    resetbutton->setFixedSize(200, 100);
    resetbutton->move(775,700);
    resetbutton->setText("重置");
    resetbutton->setStyleSheet(
        "QPushButton { border: 3px solid transparent;}"
        "QPushButton:hover { border-color: yellow; }"
        "QPushButton {"
        "   font: 30pt \"宋体\";"
        "   color: black;"
        "}"
    );
    connect(resetbutton,&QPushButton::clicked,this,&CharacterSelection::resetSelection);

    bgm=new SoundEffect(this);
    bgm->load("qrc:/sound/sound/bgm2.wav");
    bgm->setLoop(true);
    bgm->setVolume(70);
}

void CharacterSelection::createCharacterButton(CharacterType type, const QPixmap& image, const QPoint& pos)
{
    QToolButton *btn = new QToolButton(this);
    btn->setFixedSize(100, 200);
    btn->setIcon(QIcon(image));
    btn->setIconSize(QSize(100, 200));
    btn->move(pos);
    btn->setProperty("selectedCount",0); // 添加自定义属性记录选中次数
    updateButtonAppearance(btn, 0);
    connect(btn, &QToolButton::clicked, [=](){
        int count = btn->property("selectedCount").toInt();

        if(m_selected.size() < 2) {
            m_selected.append(type);  // 允许重复添加
            btn->setProperty("selectedCount", ++count);
            // 更新按钮样式显示选中次数
            updateButtonAppearance(btn, count);
            updateSelection();
        }
    });

    m_buttons[type] = btn;
}

void CharacterSelection::updateSelection()
{
    if(m_selected.size() == 2) {
        emit charactersSelected(m_selected[0], m_selected[1]);
    }
}

void CharacterSelection::updateButtonAppearance(QToolButton* btn, int count)
{
    QString style = QString(
        "QToolButton {"
        "  border: 3px solid %1;"
        "}"
        "QToolButton:hover { border-color: yellow; }"
    ).arg(count > 0 ? "red" : "transparent");

    btn->setStyleSheet(style);
}

void CharacterSelection::resetSelection()
{
    m_selected.clear();
    for(auto& btn : m_buttons) {
        btn->setProperty("selectedCount", 0);
        updateButtonAppearance(btn, 0);
    }
}

void CharacterSelection::stopBgm(){
    bgm->stop();
}

void CharacterSelection::playBgm(){
    bgm->play();
}
