#include "widget.h"
#include <QVBoxLayout>
#include <QAction>
#include <QTextEdit>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QProcess>
#include <QDir>
#include <QString>
#include <QMessageBox>

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    // Negative margins to remove spacing on edges
    this->setContentsMargins(-11, -11, -11, -11);
    this->setGeometry(0, 0, 480, 640);

    // Layout
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    this->setLayout(layout);

    // Toolbar
    toolBarLayout = new ToolBarWidget(this);
    layout->addWidget(toolBarLayout, 0);
    toolBarLayout->raise(); // Show shadow

    // Toolbar: Open
    openNewProjectButton = new BigButton(this);
    openNewProjectButton->setText("Open");
    toolBarLayout->addWidget(openNewProjectButton);

    // Toolbar: Edit
    switchToEditModeButton = new BigButton(this);
    switchToEditModeButton->setText("Edit");
    switchToEditModeButton->setCheckable(true);
    switchToEditModeButton->setChecked(true);
    toolBarLayout->addWidget(switchToEditModeButton);
    switchToEditModeButton->connect(switchToEditModeButton, SIGNAL(clicked()),
                                    this, SLOT(switchModeButtonClicked()));

    // Toolbar: Assemble
    assembleButton = new BigButton(this);
    assembleButton->setText("Compile");
    toolBarLayout->addWidget(assembleButton);
    assembleButton->connect(assembleButton, SIGNAL(clicked()),
                            this, SLOT(assembleButtonClicked()));

    // Toolbar: Set PC
    setPcButton = new BigButton(this);
    setPcButton->setText("Set PC");
    toolBarLayout->addWidget(setPcButton);
    setPcButton->connect(setPcButton, SIGNAL(clicked()),
                         this, SLOT(setPcButtonClicked()));

    // Toolbar: Next
    nextButton = new BigButton(this);
    nextButton->setText("Next");
    toolBarLayout->addWidget(nextButton);
    nextButton->connect(nextButton, SIGNAL(clicked()),
                        this, SLOT(nextButtonClicked()));

    // Text edit
    sourceEditor = new QTextEdit(this); // FIXME: This line takes FOREVER to finish!!!
    sourceEditor->lower(); // Show toolbar's shadow
    sourceEditor->setFontFamily("Monospace");
    sourceEditor->setAutoFillBackground(true); // Required to change color
    sourceEditor->setPalette(QPalette(Qt::white)); // Change color
    sourceEditor->setText("; Hello, world!\n\n.ORIG x0000\n\n\tLEA R0,HelloWorld\n\tPUTS\n\tHALT\n\n"
                        "HelloWorld\t.STRINGZ \"Hello, world!\"\n\n.END");
    layout->addWidget(sourceEditor, 0);

    programCounterPosition = -1;
    debuggerProcess = new QProcess(this);

    debuggingTableWidget = new QTableWidget(this);
    debuggingTableWidget->setGeometry(sourceEditor->geometry());
    sourceEditor->setHidden(true);
    debuggingTableWidget->lower();
    debuggingTableWidget->setRowCount(0x301);
    debuggingTableWidget->setColumnCount(2);
    debuggingTableWidget->setAlternatingRowColors(true);
    debuggingTableWidget->setShowGrid(true);
//    debuggingTableWidget
    QStringList verticalHeaderLabels;
    for (int i = 0x000; i < 0x0300; ++i) {
        QString address;
        address.setNum(i, 16); // base 16
        while (address.length() < 4)
            address.prepend('0');
        address.prepend('x');
        verticalHeaderLabels << address;
    }
    debuggingTableWidget->setVerticalHeaderLabels(verticalHeaderLabels);
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << "LABEL" << "INSTRUCTION";
    debuggingTableWidget->setHorizontalHeaderLabels(horizontalHeaderLabels);
    layout->addWidget(debuggingTableWidget);
    debuggingTableWidget->hide();
    sourceEditor->show();

    // Syntax highlighter
    sourceEditorSyntaxHighlighter = new SyntaxHighlighter(sourceEditor->document());

    // Status bar
    statusBarWidget = new StatusBarWidget(this);
    statusBarWidget->addAction(new QAction("Status", statusBarWidget));
    QLabel *statusLabel = new QLabel(statusBarWidget);
    statusLabel->setText("Normal");
    statusLabel->setGeometry(0, 0, 80, 20);
    statusBarWidget->addWidget(statusLabel);
    layout->addWidget(statusBarWidget);
}

Widget::~Widget()
{
}

void Widget::addToTable(const QString &text, int row, int column) {
    QTableWidgetItem *item = new QTableWidgetItem(text);
    debuggingTableWidget->setItem(row, column, item);
}

void Widget::saveSource(const QString &fileName) {
    QString sourceContents = sourceEditor->toPlainText();
    QFile tempFile(fileName);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCritical() << "Failed to open" << tempFile.fileName() << "for writing";
    }
    QTextStream stream(&tempFile);
    stream << sourceContents;
    tempFile.close();
}

const QString Widget::assemble(const QString &fileName) {
    assemblerProcess = new QProcess(this);
    // FIXME: This would be very inconvenient for somebody else to test drive:
    QString executable("/home/bryan/Downloads/lc3tools/lc3as");
    qDebug() << QFile(executable).exists();
    QStringList arguments(fileName);
    assemblerProcess->start(executable, arguments);
    assemblerProcess->waitForFinished();
    return assemblerProcess->readAllStandardOutput();
}

void Widget::populateLabels(const QString &fileName) {
    QString symbolsFileName = fileName;
    if (!fileName.contains('.')) {
        symbolsFileName.append(".sym");
    } else {
        symbolsFileName.replace(fileName.lastIndexOf('.'), INT_MAX, ".sym");
    }

    QFile symbolsFile(symbolsFileName);
    if (!symbolsFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Failed to open symbols file" << symbolsFile.fileName()
                       << "for reading";
        return;
    }

    // Discard first four lines
    symbolsFile.readLine();
    symbolsFile.readLine();
    symbolsFile.readLine();
    symbolsFile.readLine();

    QMap<int, QString>::const_iterator it = labels.constBegin();
    while (it != labels.constEnd()) {
        debuggingTableWidget->setItem(it.key(), 0, NULL);
        qDebug() << "Label:" << it.value() << "Address:" << it.key();
        ++it;
    }

    labels.clear();

    if (symbolsFile.atEnd()) { // No symbols?
        return;
    }

    QString line = symbolsFile.readLine().simplified();

    do
    {
        QStringList sections = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        qDebug() << sections.at(1) << sections.at(2);
        QString label = sections.at(1);
        unsigned int address = sections.at(2).toUInt(NULL, 16);
        labels.insert(address, label);
        line = symbolsFile.readLine().simplified();
    }
    while (!symbolsFile.atEnd());

    it = labels.constBegin();
    while (it != labels.constEnd()) {
        QTableWidgetItem *item = new QTableWidgetItem(it.value());
        debuggingTableWidget->setItem(it.key(), 0, item);
        qDebug() << "Label:" << it.value() << "Address:" << it.key();
        ++it;
    }
}

void Widget::switchModeButtonClicked() {
    qDebug() << "Switch mode";

    if (sourceEditor->isVisible()) {
        sourceEditor->hide();
        debuggingTableWidget->show();
    } else {
        sourceEditor->show();
        debuggingTableWidget->hide();
    }
}

void Widget::assembleButtonClicked() {
    QString fileName(QDir::homePath() + "/.lc3ide.asm");
    saveSource(fileName);
    assemble(fileName);
    populateLabels(fileName);
}

QString Widget::hexStringForAddress(int address) {
    QString hexStringBase("x0000");
    QString result = QString::number(address, 16); // To hexadecimal
    return hexStringBase.replace(hexStringBase.length() - result.length(), INT_MAX, result);
}

void Widget::setProgramCounter(int address) {
    Q_ASSERT(address >= 0);
    Q_ASSERT(address < MAX_VALUE_IN_BIT_LENGTH);

    // Restore old header for old PC
    debuggingTableWidget->setVerticalHeaderItem(programCounterPosition,
                                                new QTableWidgetItem(hexStringForAddress(programCounterPosition)));

    // Put PC label on current address
    debuggingTableWidget->setVerticalHeaderItem(address, new QTableWidgetItem("--> PC"));

    programCounterPosition = address;
}

void Widget::setPcButtonClicked() {
//    QList<QTableWidgetItem *> selected = debuggingTableWidget->selectedItems();
//    qDebug() << selected.size() << debuggingTableWidget->currentRow();
//    if (selected.size() > 1) {
//        QMessageBox::warning(this, "Invalid selection", "The PC can only be set to one address.", QMessageBox::Ok, QMessageBox::Cancel);
//        return; // Try again, user!
//    } else if (selected.isEmpty()) {
//        return; // No error message needed
//    }

    setProgramCounter(debuggingTableWidget->currentRow());
}

void Widget::nextButtonClicked() {
    initializeSimulator();


}

void Widget::initializeSimulator() {
    const QString simulatorPath("/home/bryan/Downloads/lc3tools/lc3sim");

    if (debuggerProcess->state() == QProcess::Running) {
        return;
    }

    QStringList args;
    args << QDir::homePath() + "/.lc3ide.obj";

    debuggerProcess->kill();
    debuggerProcess->start(simulatorPath, args);
    debuggerProcess->waitForFinished(3 * 1000); // Up to 3 seconds
    qDebug() << debuggerProcess->readAll();
    debuggerProcess->kill(); // We're done with him!  Hahaha!
}
