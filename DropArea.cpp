//
// Created by lordgasmic on 5/17/23.
//

#include "DropArea.h"

DropArea::DropArea(QWidget *parent) : QLabel(parent) {
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
    setAutoFillBackground(true);
    clear();
}

void DropArea::dragEnterEvent(QDragEnterEvent *event) {
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
    emit changed(event->mimeData());
}

void DropArea::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void DropArea::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasText()) {
        setText(mimeData->text());
        setTextFormat(Qt::PlainText);
    }
    else {
        setText(tr("Cannot display data"));
    }

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
}

void DropArea::dragLeaveEvent(QDragLeaveEvent *event) {
    clear();
    event->accept();
}

void DropArea::clear() {
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Dark);

    emit changed();
}
