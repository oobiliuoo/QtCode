/********************************************************************************
** Form generated from reading UI file 'pfsserver.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PFSSERVER_H
#define UI_PFSSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PFSServer
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QLineEdit *severIp;
    QLineEdit *serverPort;
    QLabel *label_2;
    QLabel *label;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QTextEdit *record_2;
    QLabel *label_3;

    void setupUi(QWidget *PFSServer)
    {
        if (PFSServer->objectName().isEmpty())
            PFSServer->setObjectName(QStringLiteral("PFSServer"));
        PFSServer->resize(666, 300);
        widget = new QWidget(PFSServer);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 130, 231, 111));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        severIp = new QLineEdit(widget);
        severIp->setObjectName(QStringLiteral("severIp"));

        gridLayout->addWidget(severIp, 0, 1, 1, 1);

        serverPort = new QLineEdit(widget);
        serverPort->setObjectName(QStringLiteral("serverPort"));

        gridLayout->addWidget(serverPort, 1, 1, 1, 1);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        widget_2 = new QWidget(PFSServer);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(270, 30, 383, 241));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_2->addWidget(label_4);

        record_2 = new QTextEdit(widget_2);
        record_2->setObjectName(QStringLiteral("record_2"));

        verticalLayout_2->addWidget(record_2);

        label_3 = new QLabel(PFSServer);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(90, 40, 121, 71));
        QFont font;
        font.setFamily(QStringLiteral("Umpush"));
        font.setPointSize(18);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        label_3->setFont(font);

        retranslateUi(PFSServer);

        QMetaObject::connectSlotsByName(PFSServer);
    } // setupUi

    void retranslateUi(QWidget *PFSServer)
    {
        PFSServer->setWindowTitle(QApplication::translate("PFSServer", "Form", Q_NULLPTR));
        label_2->setText(QApplication::translate("PFSServer", "Port", Q_NULLPTR));
        label->setText(QApplication::translate("PFSServer", "IP", Q_NULLPTR));
        label_4->setText(QApplication::translate("PFSServer", "\346\223\215\344\275\234\350\256\260\345\275\225\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("PFSServer", "\346\254\242\350\277\216\344\275\277\347\224\250\357\274\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PFSServer: public Ui_PFSServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PFSSERVER_H
