//
// Created by lordgasmic on 5/17/23.
//

#ifndef SEEDRIPPER_DROPSITEWINDOW_H
#define SEEDRIPPER_DROPSITEWINDOW_H


#include <QWidget>
#include <QMimeData>
#include <QTableWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include "DropArea.h"
#include <QHeaderView>

class DropSiteWindow : public QWidget {
Q_OBJECT

public:
    DropSiteWindow();

public
    slots:

    void updateSeedLabel(const QMimeData *mimeData);

    void copy();

private:
    DropArea *dropArea;
    QLabel *abstractLabel;
    QLabel *seedLabel;

    QPushButton *clearButton;
    QPushButton *copyButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
};

#endif //SEEDRIPPER_DROPSITEWINDOW_H
