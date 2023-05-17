//
// Created by lordgasmic on 5/17/23.
//

#ifndef SEEDRIPPER_DROPAREA_H
#define SEEDRIPPER_DROPAREA_H


#include <QLabel>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

class DropArea : public QLabel {
Q_OBJECT

public:
    explicit DropArea(QWidget *parent = nullptr);

public slots:

    void clear();

signals:

    void changed(const QMimeData *mimeData = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dragLeaveEvent(QDragLeaveEvent *event) override;

    void dropEvent(QDropEvent *event) override;
};


#endif //SEEDRIPPER_DROPAREA_H
