#include "statusbarwidget.h"
#include <QGraphicsDropShadowEffect>

StatusBarWidget::StatusBarWidget(QWidget *parent) :
    QStatusBar(parent)
{
    this->setAutoFillBackground(true);
    this->setGeometry(0, 0, 10, 20);
    this->setFixedHeight(this->height());

    QGraphicsDropShadowEffect *dropShadow = new QGraphicsDropShadowEffect(this);
    dropShadow->setBlurRadius(20.0);
    dropShadow->setOffset(0, 0);
    dropShadow->setColor(QColor(0, 0, 0, 204));
    this->setGraphicsEffect(dropShadow);
}
