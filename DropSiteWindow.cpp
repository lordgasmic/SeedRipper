//
// Created by lordgasmic on 5/17/23.
//

#include <QVBoxLayout>
#include <QGuiApplication>
#include <QClipboard>
#include "DropSiteWindow.h"
#include <fstream>
#include <iostream>
#include <string>

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

    QString q_filename = mimeData->text().simplified();
    std::string inFile = q_filename.toStdString();
    std::string path = inFile.substr(7);
    std::ifstream inf{path};

    if (!inf) {
        std::cerr << "file not found\n";
    }

    std::string strInput;
    while (inf) {
        std::getline(inf, strInput);
        if (strInput.find("Seed: ") != std::string::npos) {
            int index = strInput.find("Seed: ");
            strInput = strInput.substr(index + 6);
            strInput = strInput.substr(0, strInput.find_first_of(','));
            std::cout << strInput << '\n';
            break;
        }
    }

    if (seedLabel->text().length() == 0) {
        seedLabel->setText(QString::fromStdString(strInput));
    }
    else {
        seedLabel->setText(seedLabel->text() + ", " + QString::fromStdString(strInput));
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
