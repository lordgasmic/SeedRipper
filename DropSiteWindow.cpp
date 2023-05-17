//
// Created by lordgasmic on 5/17/23.
//

#include <QVBoxLayout>
#include "DropSiteWindow.h"

DropSiteWindow::DropSiteWindow() {
    abstractLabel = new QLabel(tr("This example accepts drags from other "
                                  "applications and displays the MIME types "
                                  "provided by the drag object."));
    abstractLabel->setWordWrap(true);
    abstractLabel->adjustSize();
    dropArea = new DropArea;
    connect(dropArea, &DropArea::changed,
            this, &DropSiteWindow::updateFormatsTable);
    formatsTable = new QTableWidget;
    formatsTable->setColumnCount(2);
    formatsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    formatsTable->setHorizontalHeaderLabels({tr("Format"), tr("Content")});
    formatsTable->horizontalHeader()->setStretchLastSection(true);
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
//    connect(copyButton, &QAbstractButton::clicked, this, &DropSiteWindow::copy);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(abstractLabel);
    mainLayout->addWidget(dropArea);
    mainLayout->addWidget(formatsTable);
    mainLayout->addWidget(buttonBox);

    setWindowTitle(tr("Drop Site"));
    resize(700, 500);
}

void DropSiteWindow::updateFormatsTable(const QMimeData *mimeData) {
    formatsTable->setRowCount(0);
    copyButton->setEnabled(false);
    if (!mimeData) {
        return;
    }

    const QStringList formats = mimeData->formats();
    for (const QString &format: formats) {
        QTableWidgetItem *formatItem = new QTableWidgetItem(format);
        formatItem->setFlags(Qt::ItemIsEnabled);
        formatItem->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);

        QString text;
        if (format == u"text/plain") {
            text = mimeData->text().simplified();
        }
        else {
//            QByteArray data = mimeData->data(format);
//            if (data.size() > 32) {
//                data.truncate(32);
//            }
//            text = QString::fromLatin1(data.toHex(' ')).toUpper();
            continue;
        }

        int row = formatsTable->rowCount();
        formatsTable->insertRow(row);
        formatsTable->setItem(row, 0, new QTableWidgetItem(format));
        formatsTable->setItem(row, 1, new QTableWidgetItem(text));
    }

    formatsTable->resizeColumnToContents(0);
#if QT_CONFIG(clipboard)
    copyButton->setEnabled(formatsTable->rowCount() > 0);
#endif
}
