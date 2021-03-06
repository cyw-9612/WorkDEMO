#include "PageWidget.h"
#include "ui_PageWidget.h"

#include <QtGlobal>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

PageWidget::PageWidget(int blockSize, QWidget *parent) : QWidget(parent),
    ui(new Ui::PageWidget) {
    ui->setupUi(this);
    setBlockSize(blockSize);
    initialize();

    maxPage = 0;
    setMaxPage(1);

    QString qss = QString(".QLabel[page=\"true\"] { padding: 1px; }")
            + QString(".QLabel[currentPage=\"true\"] { color: rgb(190, 0, 0);}")
            + QString(".QLabel[page=\"true\"]:hover { color: white; border-radius: 4px; background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(53, 121, 238, 255), stop:1 rgba(0, 202, 237, 255));}");

    QString qss1 = QString(".QLabel[page=\"true\"] {\
                           color: rgb(85, 85, 85);\
                           background-color: rgb(255, 255, 255);\
                           border-width: 1px;\
                           border-style: solid;\
                           border-color: rgb(185, 185, 185);}")
            + QString(".QLabel[currentPage=\"true\"] { border-color: rgb(56, 168, 168);\
                            color: rgb(56, 168, 168);}")
            + QString(".QLabel[page=\"true\"]:hover {\
                      border-color: rgb(56, 168, 168);\
                      color: rgb(56, 168, 168);}");

    this->setStyleSheet(qss1);

}

PageWidget::~PageWidget() {
    delete ui;
    delete pageLabels;
}

bool PageWidget::eventFilter(QObject *watched, QEvent *e) {
    if (e->type() == QEvent::MouseButtonRelease) {
        int page = -1;
        if (watched == ui->previousPageLabel) { page = getCurrentPage() - 1; }

        if (watched == ui->nextPageLabel) { page = getCurrentPage() + 1; }

        for (int i = 0; i < pageLabels->count(); ++i) {
            if (watched == pageLabels->at(i)) {
                page = pageLabels->at(i)->text().toInt();
                break;
            }
        }

        if (-1 != page) {
            setCurrentPage(page, true);
            return true;
        }
    }

    if (watched == ui->pageLineEdit && e->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return) {
            setCurrentPage(ui->pageLineEdit->text().toInt(), true);
            //setMaxPage(ui->pageLineEdit->text().toInt()); // ????????????????????????
            return true;
        }
    }

    return QWidget::eventFilter(watched, e);
}

int PageWidget::getBlockSize() const {
    return blockSize;
}

int PageWidget::getMaxPage() const {
    return maxPage;
}

int PageWidget::getCurrentPage() const {
    return currentPage;
}

void PageWidget::setMaxPage(int page) {
    page = qMax(page, 1);

    if (maxPage != page) {
        this->maxPage = page;
        this->currentPage = 1;
        updatePageLabels();
    }
}

//????????????????????????
void PageWidget::setTotalCount(int count)
{
    QString totalCount = QString(tr("???%1???")).arg(count);
    ui->label_total->setText(totalCount);
}

void PageWidget::setCurrentPage(int page, bool signalEmitted) {
    page = qMax(page, 1);
    page = qMin(page, maxPage);
    qDebug() << page;
    if (page != this->currentPage) {
        this->currentPage = page;
        updatePageLabels();

        if (signalEmitted) {
            emit currentPageChanged(page);
        }
    }
}

void PageWidget::setBlockSize(int blockSize) {
    // ??????????????????, block size ???????????????, ????????????3
    blockSize = qMax(blockSize, 3);
    if (blockSize % 2 == 0) {
        ++blockSize;
    }
    this->blockSize = blockSize;
}

// ??????????????????labels
// ??????????????????, ???...???...???
void PageWidget::initialize() {
    ui->pageLineEdit->installEventFilter(this);
    ui->pageLineEdit->setValidator(new QIntValidator(1, 10000000, this));

    ui->nextPageLabel->setProperty("page", "true");
    ui->previousPageLabel->setProperty("page", "true");
    ui->nextPageLabel->installEventFilter(this);
    ui->previousPageLabel->installEventFilter(this);

    pageLabels = new QList<QLabel *>();

    QHBoxLayout *leftLayout = new QHBoxLayout();
    QHBoxLayout *centerLayout = new QHBoxLayout();
    QHBoxLayout *rightLayout = new QHBoxLayout();
    leftLayout->setContentsMargins(6, 0, 0, 0);
    leftLayout->setSpacing(6);
    centerLayout->setContentsMargins(6, 0, 6, 0);
    centerLayout->setSpacing(6);
    rightLayout->setContentsMargins(0, 0, 6, 0);
    rightLayout->setSpacing(6);

    for (int i = 0; i < blockSize * 3; ++i) {
        QLabel *label = new QLabel(QString::number(i + 1));
        label->setProperty("page", "true");
        label->installEventFilter(this);
        label->setMaximumSize(32,32);
        label->setMinimumSize(32,32);
        label->setAlignment(Qt::AlignCenter);
        //label->setStyleSheet(labStyle);

        pageLabels->append(label);

        if (i < blockSize) {
            leftLayout->addWidget(label);
        } else if (i < blockSize * 2) {
            centerLayout->addWidget(label);
        } else {
            rightLayout->addWidget(label);
        }
    }

    ui->leftPagesWidget->setLayout(leftLayout);
    ui->centerPagesWidget->setLayout(centerLayout);
    ui->rightPagesWidget->setLayout(rightLayout);
}

void PageWidget::updatePageLabels() {
    ui->leftSeparateLabel->hide();
    ui->rightSeparateLabel->hide();

    if (maxPage <= blockSize * 3) {
        for (int i = 0; i < pageLabels->count(); i += 1) {
            QLabel *label = pageLabels->at(i);

            if (i < maxPage) {
                label->setText(QString::number(i + 1));
                label->show();
            } else {
                label->hide();
            }

            if (currentPage - 1 == i) {
                label->setProperty("currentPage", "true");
            } else {
                label->setProperty("currentPage", "false");
            }

            label->setStyleSheet("/**/");
        }
        return;
    }

    // ???????????????maxPageNumber??????blockSize * 3, ???????????????label????????????
    // c ??? currentPage
    // n ??? block size
    // m ??? maxPage

    // 1. c ??? [1, n + n/2 + 1]: ????????? n * 2 ???, ??? n ???: ???????????????????????????
    // 2. c ??? [m - n - n/2, m]: ????????? n ???, ??? n * 2 ???: ???????????????????????????
    // 3. ??????[1, n], [c - n/2, c + n/2], [m - 2*n + 1, m]: ????????????????????????

    int c = currentPage;
    int n = blockSize;
    int m = maxPage;
    int centerStartPage = 0;

    if (c >= 1 && c <= n + n / 2 + 1) {
        // 1. c ??? [1, n + n/2 + 1]: ????????? n * 2 ???, ??? n ???: ???????????????????????????
        centerStartPage = n + 1;
        ui->rightSeparateLabel->show();
    } else if (c >= m - n - n / 2 && c <= m) {
        // 2. c ??? [m - n - n/2, m]: ????????? n ???, ??? n * 2 ???: ???????????????????????????
        centerStartPage = m - n - n + 1;
        ui->leftSeparateLabel->show();
    } else {
        // 3. ??????[1, n], [c - n/2, c + n/2], [m - n + 1, m]: ????????????????????????
        centerStartPage = c - n / 2;
        ui->rightSeparateLabel->show();
        ui->leftSeparateLabel->show();
    }

    for (int i = 0; i < n; ++i) {
        pageLabels->at(i)->setText(QString::number(i + 1));                     // ?????? n ???
        pageLabels->at(n + i)->setText(QString::number(centerStartPage + i));   // ?????? n ???
        pageLabels->at(3 * n - i - 1)->setText(QString::number(m - i));         // ?????? n ???
    }

    for (int i = 0; i < pageLabels->count(); ++i) {
        QLabel *label = pageLabels->at(i);
        int page = label->text().toInt();
        if (page == currentPage) {
            label->setProperty("currentPage", "true");
        } else {
            label->setProperty("currentPage", "false");
        }

        label->setStyleSheet("/**/");
        label->show();
    }
}
