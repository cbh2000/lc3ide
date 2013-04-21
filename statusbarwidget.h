#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QStatusBar>

class StatusBarWidget : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget *parent = 0);
    
signals:

public slots:
    
};

#endif // STATUSBARWIDGET_H
