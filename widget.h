#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "toolbarwidget.h"
#include "statusbarwidget.h"
#include "bigbutton.h"
#include <QVBoxLayout>
#include <QAction>
#include <QTextEdit>
#include "syntaxhighlighter.h"
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QProcess>

#define BIT_LENGTH (16)
#define MAX_VALUE_IN_BIT_LENGTH (2 << BIT_LENGTH)

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
protected slots:
    void switchModeButtonClicked();
    void assembleButtonClicked();

private:
    QVBoxLayout *layout;
    ToolBarWidget *toolBarLayout;
    StatusBarWidget *statusBarWidget;
    BigButton *openNewProjectButton;
    BigButton *switchToEditModeButton;
    BigButton *assembleButton;
    QTextEdit *sourceEditor;
    SyntaxHighlighter *sourceEditorSyntaxHighlighter;
    QTableWidget *debuggingTableWidget;
    QProcess *assemblerProcess;
    QProcess *debuggerProcess;

    void addToTable(const QString &text, int row, int column);

    void saveSource(const QString &fileName);
    const QString &assemble(const QString &fileName);
    void populateLabels(const QString &fileName);
    QMap<int, QString> labels;
};

#endif // WIDGET_H
