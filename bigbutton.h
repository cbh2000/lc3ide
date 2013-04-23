#ifndef BIGBUTTON_H
#define BIGBUTTON_H

#include <QPushButton>

class BigButton : public QPushButton
{
    Q_OBJECT
public:
    explicit BigButton(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize maximumSize() const;
    
signals:
    
public slots:

protected:
};

#endif // BIGBUTTON_H
