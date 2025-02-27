#include "MainMenu.h"
MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    setFixedSize(1770, 1024);

    // 背景图
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/ima/menu_bg.png").scaled(1770,1024)));
    setPalette(palette);
    setAutoFillBackground(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(250, 400, 730, 400);
    layout->setSpacing(100);

    QPushButton *randomBtn = createMenuButton("随机角色");
    QPushButton *selectBtn = createMenuButton("选择角色");

    layout->addWidget(randomBtn);
    layout->addWidget(selectBtn);

    connect(randomBtn, &QPushButton::clicked, this, &MainMenu::randomBattleSelected);
    connect(selectBtn, &QPushButton::clicked, this, &MainMenu::characterSelectSelected);
}

QPushButton* MainMenu::createMenuButton(const QString& text)
{
    QPushButton *btn = new QPushButton(this);
    btn->setFixedSize(300, 100);
    btn->setText(text);
    btn->setStyleSheet(
        "QPushButton { border: 3px solid transparent;}"
        "QPushButton:hover { border-color: yellow; }"
        "QPushButton {"
        "   font: 30pt \"宋体\";"
        "   color: rgb(89,92,105);"
        "}"
    );
    return btn;
}
