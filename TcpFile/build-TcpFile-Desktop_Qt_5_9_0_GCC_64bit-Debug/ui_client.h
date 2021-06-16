/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *fileName;
    QPushButton *btnRequest;
    QPushButton *btnSend;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *record;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QTextEdit *content;

    void setupUi(QWidget *Client)
    {
        if (Client->objectName().isEmpty())
            Client->setObjectName(QStringLiteral("Client"));
        Client->resize(723, 437);
        horizontalLayout_4 = new QHBoxLayout(Client);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(Client);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        fileName = new QLineEdit(Client);
        fileName->setObjectName(QStringLiteral("fileName"));

        verticalLayout->addWidget(fileName);

        btnRequest = new QPushButton(Client);
        btnRequest->setObjectName(QStringLiteral("btnRequest"));

        verticalLayout->addWidget(btnRequest);

        btnSend = new QPushButton(Client);
        btnSend->setObjectName(QStringLiteral("btnSend"));
        btnSend->setEnabled(false);

        verticalLayout->addWidget(btnSend);

        groupBox_2 = new QGroupBox(Client);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        record = new QTextEdit(groupBox_2);
        record->setObjectName(QStringLiteral("record"));

        horizontalLayout_2->addWidget(record);


        verticalLayout->addWidget(groupBox_2);


        horizontalLayout_3->addLayout(verticalLayout);

        groupBox = new QGroupBox(Client);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        content = new QTextEdit(groupBox);
        content->setObjectName(QStringLiteral("content"));

        horizontalLayout->addWidget(content);


        horizontalLayout_3->addWidget(groupBox);


        horizontalLayout_4->addLayout(horizontalLayout_3);


        retranslateUi(Client);

        QMetaObject::connectSlotsByName(Client);
    } // setupUi

    void retranslateUi(QWidget *Client)
    {
        Client->setWindowTitle(QApplication::translate("Client", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("Client", "\346\226\207\344\273\266\345\220\215\357\274\232", Q_NULLPTR));
        btnRequest->setText(QApplication::translate("Client", "\350\257\267\346\261\202", Q_NULLPTR));
        btnSend->setText(QApplication::translate("Client", "\345\217\221\351\200\201", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("Client", "\346\223\215\344\275\234\350\256\260\345\275\225\357\274\232", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Client", "\346\226\207\344\273\266\345\206\205\345\256\271\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
