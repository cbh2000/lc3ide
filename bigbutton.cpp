#include "bigbutton.h"

BigButton::BigButton(QWidget *parent) :
    QPushButton(parent) {
    this->setGeometry(0, 0, 40, 40);
    this->setSizePolicy(QSizePolicy::Preferred,
                        QSizePolicy::Preferred);
}

QSize BigButton::sizeHint() const {
    static QSize __sizeHint__(40, 40);
    return __sizeHint__;
}

QSize BigButton::maximumSize() const {
    return sizeHint();
}
