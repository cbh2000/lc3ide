#ifndef STATUSBARWIDGET_H
#define STATUSBARWIDGET_H

#include <QStatusBar>

class QGraphicsDropShadowEffect;

class StatusBarWidget : public QStatusBar
{
    Q_OBJECT
public:
    explicit StatusBarWidget(QWidget *parent = 0);
    
signals:

public slots:

private:
    QGraphicsDropShadowEffect *dropShadow;
    
};

#endif // STATUSBARWIDGET_H
