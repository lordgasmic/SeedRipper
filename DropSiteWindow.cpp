//
// Created by lordgasmic on 5/17/23.
//

#include <QVBoxLayout>
#include <QGuiApplication>
#include <QClipboard>
#include "DropSiteWindow.h"

DropSiteWindow::DropSiteWindow() {
    abstractLabel = new QLabel(tr("This example accepts drags from other "
                                  "applications and displays the MIME types "
                                  "provided by the drag object."));
    abstractLabel->setWordWrap(true);
    abstractLabel->adjustSize();
    dropArea = new DropArea;
    connect(dropArea, &DropArea::changed, this, &DropSiteWindow::updateSeedLabel);
    seedLabel = new QLabel("");
    seedLabel->setWordWrap(true);
    seedLabel->adjustSize();
    clearButton = new QPushButton(tr("Clear"));
    copyButton = new QPushButton(tr("Copy"));
    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(copyButton, QDialogButtonBox::ActionRole);
#if !QT_CONFIG(clipboard)
    copyButton->setVisible(false);
#endif

    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(clearButton, &QAbstractButton::clicked, dropArea, &DropArea::clear);
    connect(copyButton, &QAbstractButton::clicked, this, &DropSiteWindow::copy);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(abstractLabel);
    mainLayout->addWidget(dropArea);
    mainLayout->addWidget(seedLabel);
    mainLayout->addWidget(buttonBox);

    setWindowTitle(tr("Drop Site"));
    resize(700, 500);
}

void DropSiteWindow::updateSeedLabel(const QMimeData *mimeData) {
    copyButton->setEnabled(false);
    if (!mimeData) {
        return;
    }

    QString text;
    text = mimeData->text().simplified();
    if (seedLabel->text().length() == 0) {
        seedLabel->setText(text);
    }
    else {
        seedLabel->setText(seedLabel->text() + ", " + text);
    }

#if QT_CONFIG(clipboard)
    copyButton->setEnabled(seedLabel->text().length() > 0);
#endif
}

void DropSiteWindow::copy() {
    QString str = seedLabel->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(str);
}
