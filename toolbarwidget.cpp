#include "toolbarwidget.h"

ToolBarWidget::ToolBarWidget(QWidget *parent) :
    QToolBar(parent)
{
    this->setAutoFillBackground(true);
    this->setGeometry(0, 0, 20, 80);
    this->setFixedHeight(this->height());

    // Shadow
    dropShadow = new QGraphicsDropShadowEffect(this);
    dropShadow->setBlurRadius(20.0);
    dropShadow->setOffset(0, 0);
    dropShadow->setColor(QColor(0, 0, 0, 205));
    this->setGraphicsEffect(dropShadow);
}
