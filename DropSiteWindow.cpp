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
#include <QImageReader>

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
    connect(clearButton, &QAbstractButton::clicked, this, &DropSiteWindow::clear);
    connect(copyButton, &QAbstractButton::clicked, this, &DropSiteWindow::copy);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(abstractLabel);
    mainLayout->addWidget(dropArea);
    mainLayout->addWidget(seedLabel);
    mainLayout->addWidget(buttonBox);

    setWindowTitle(tr("Seed Ripper"));
    resize(700, 500);
    clear();
}

void DropSiteWindow::updateSeedLabel(const QMimeData *mimeData) {
    copyButton->setEnabled(false);
    if (!mimeData) {
        return;
    }

    // mimeData->data("text/uri-list").toStdString()

    QString q_filename = mimeData->text().simplified();
    std::string inFile = q_filename.toStdString();
    char delimiter = ' ';
    if (inFile.find(delimiter) == std::string::npos) {
        std::string path = inFile.substr(8);
        processFile(&path);
    } else {
        size_t pos = 0;
        std::string token;
        while ((pos = inFile.find(delimiter)) !=
               std::string::npos) {  // do while to get rid of if statement, try to figure out how to not have the dangler
            token = inFile.substr(0, pos).substr(8);
            std::cout << "token: " << token << std::endl;
            processFile(&token);
            inFile.erase(0, pos + 1);
        }
        token = inFile.substr(8);
        processFile(&token);
    }
}

void DropSiteWindow::copy() {
    QString str = seedLabel->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(str);
}

void DropSiteWindow::clear() {
    seedLabel->setText("");

#if QT_CONFIG(clipboard)
    copyButton->setEnabled(seedLabel->text().length() > 0);
#endif
}

void DropSiteWindow::processFile(std::string *filename) {
    std::cout << *filename << std::endl;


}

void DropSiteWindow::processTxtFile(std::string *filename) {
    std::ifstream inf{*filename};

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
    } else {
        seedLabel->setText(seedLabel->text() + ", " + QString::fromStdString(strInput));
    }

#if QT_CONFIG(clipboard)
    copyButton->setEnabled(seedLabel->text().length() > 0);
#endif
}

void DropSiteWindow::processPngFile() {
    QImageReader *reader = new QImageReader();
    reader->setFileName(QString::fromStdString(*filename));
    for (QString key: reader->textKeys()) {
        std::cout << key.toStdString() << std::endl;

    }

    std::cout << reader->text("parameters").toStdString() << std::endl;
}