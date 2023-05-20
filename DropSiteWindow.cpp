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
#include <QFile>

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

    QList<QUrl> urls = mimeData->urls();
    for (QUrl url: urls) {
        QString s = url.toDisplayString(QUrl::PreferLocalFile);
        QString ext = s.sliced(s.lastIndexOf(".") + 1);
        if (ext.contains("png")) {
            processPngFile(s);
        } else if (ext.contains("txt")) {
            processTxtFile(s);
        }
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

void DropSiteWindow::processTxtFile(const QString &filename) {
    QFile f(filename);
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream stream(&f);
        QString line;
        while (!stream.atEnd()) {
            line = stream.readLine();
            if (line.indexOf("Seed: ") != std::string::npos) {
                qsizetype index = line.indexOf("Seed: ");
                line = line.sliced(index + 6);
                line = line.sliced(0, line.indexOf(','));
                break;
            }
        }

        if (seedLabel->text().length() == 0) {
            seedLabel->setText(line);
        } else {
            seedLabel->setText(seedLabel->text() + ", " + line);
        }
    }

    f.close();

#if QT_CONFIG(clipboard)
    copyButton->setEnabled(seedLabel->text().length() > 0);
#endif
}

void DropSiteWindow::processPngFile(QString &filename) {
    auto *reader = new QImageReader();
    reader->setFileName(filename);

    QString parameters = reader->text("parameters");
    if (parameters.indexOf("Seed: ") != std::string::npos) {
        qsizetype index = parameters.indexOf("Seed: ");
        QString seed = parameters.sliced(index + 6);
        seed = seed.sliced(0, seed.indexOf(','));

        if (seedLabel->text().length() == 0) {
            seedLabel->setText(seed);
        } else {
            seedLabel->setText(seedLabel->text() + ", " + seed);
        }
    }

    reader->setDevice(nullptr);

#if QT_CONFIG(clipboard)
    copyButton->setEnabled(seedLabel->text().length() > 0);
#endif
}