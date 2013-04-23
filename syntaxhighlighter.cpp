#include "syntaxhighlighter.h"
#include <QTextDocument>
#include <QGuiApplication>

#include <QDebug>

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    QString simplified = text.simplified();

    if (text.startsWith(".ORIG")) {
        int nextBoundary = text.indexOf(QRegExp("\\s"));
        QFont font = QGuiApplication::font();
        font.setBold(true);
        setFormat(0, nextBoundary, font);
    }

    for (int i = 0; i < text.length(); ++i) {
        if (text.at(i) == ';') {
            setFormat(i, text.length() - i, Qt::gray);
            break;
        } else if (text.at(i).isDigit()) {
            if (i > 0 && text.at(i - 1) == 'x') {
                setFormat(i - 1, 1, Qt::green);
            }
            setFormat(i, 1, Qt::green);
            continue;
        } else if (text.at(i) == '\"') {
            int nextQuotationMark = text.indexOf('\"', i + 1);
            setFormat(i, nextQuotationMark, Qt::red);

            if (nextQuotationMark > 0) {
                i = nextQuotationMark;
            }
        }
    }
}
