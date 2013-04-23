#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QToolBar>

class QToolBar;
class QGraphicsDropShadowEffect;

class ToolBarWidget : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBarWidget(QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    QGraphicsDropShadowEffect *dropShadow;
};

#endif // TOOLBARWIDGET_H
