#include <iostream>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AddRow();
    AddRadioButton();

    AddQuestion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddRow()
{
    /// ui->tableQuestion->insertRow(ui->tableQuestion->rowCount());
    /// int rowIndex = ui->tableQuestion->rowCount() - 1;
    /// QTableWidgetItem *item0 = new QTableWidgetItem("sdfdsf");
    /// QTableWidgetItem *item1 = new QTableWidgetItem("lkjl");

    /// ui->tableQuestion->setItem(rowIndex, 0, item0);
    /// ui->tableQuestion->setItem(rowIndex, 1, item1);
}

void MainWindow::AddRadioButton()
{
    /// groupButton1 = new QButtonGroup(this);
    /// groupButton1->addButton(ui->radioButton, 0);
    /// groupButton1->addButton(ui->radioButton_2, 1);
    /// groupButton1->addButton(ui->radioButton_3, 2);
    /// groupButton1->addButton(ui->radioButton_4, 3);

    /// connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(slots_pushButton()));
}

void MainWindow::slots_pushButton()
{
    /// switch (groupButton1->checkedId())
    /// {
    ///     case 0:
    ///         qDebug() << "由项目管理办公室统一管理";
    ///         break;
    ///     case 1:
    ///         qDebug() << "被作为项目组合来协调管理";
    ///         break;
    ///     case 2:
    ///         qDebug() << "被作为项目集来协调管理";
    ///         break;
    ///     case 3:
    ///         qDebug() << "被归入项目作战室统管理";
    ///         break;
    ///     default:
    ///         qDebug() << "未选择答案";

    ///         break;
    /// }
    /// switch (RadioButtonBox->checkedId())
    /// {
    ///
    /// }
}

void MainWindow::AddQuestion()
{
    /// ui->textEditQuestion->setText("如果项目之间的联系仅限于共享顾主、供应商、技术或资源，则这些项目应该()");
    /// ui->textEditAnswer0->setText("由项目管理办公室统一管理");
    /// ui->textEditAnswer1->setText("被作为项目组合来协调管理");
    /// ui->textEditAnswer2->setText("被作为项目集来协调管理");
    /// ui->textEditAnswer3->setText("被归入项目作战室统管理");
}
