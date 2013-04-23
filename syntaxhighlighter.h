#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
class QTextDocument;

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument *parent = 0);

    void highlightBlock(const QString &text);
    
signals:
    
public slots:
    
};

#endif // SYNTAXHIGHLIGHTER_H
