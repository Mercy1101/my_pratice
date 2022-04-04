#include <iostream>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /// 把 radio button 绑定在一个组内
    group_selection_->addButton(ui->rbA, 0);
    group_selection_->addButton(ui->rbB, 1);
    group_selection_->addButton(ui->rbC, 2);
    group_selection_->addButton(ui->rbD, 3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_question(const int& question_index, const QuestionData& question)
{
    /// 显示问题
    ui->edtQuestion->setText(question.get_question().c_str());

    std::map<int, std::string> answers = question.get_answer();
    if (answers.size() != 4)
    {
        qDebug() << "Invalid Answer";
        return;
    }

    /// 保存问题的 index
    ui->labelQuestionIndex->setText(std::to_string(question_index).c_str());

    /// 显示答案
    ui->edtAnswerA->setText(answers[0].c_str());
    ui->edtAnswerB->setText(answers[1].c_str());
    ui->edtAnswerC->setText(answers[2].c_str());
    ui->edtAnswerD->setText(answers[3].c_str());

    /// 绑定记录答案的东西
    connect(ui->rbA, SIGNAL(clicked(bool)), this, SLOT(slot_click_answer));
    connect(ui->rbB, SIGNAL(clicked(bool)), this, SLOT(slot_click_answer));
    connect(ui->rbC, SIGNAL(clicked(bool)), this, SLOT(slot_click_answer));
    connect(ui->rbD, SIGNAL(clicked(bool)), this, SLOT(slot_click_answer));
}

void MainWindow::on_btnConfirm_clicked()
{
    QuestionData question;
    if (!controller_.get_question(GetQuestionIndex(), question))
    {
        qDebug() << "get_question failed!";
    }

    /// 确认答案
    if (controller_.compare_answer(GetQuestionIndex()))
    {
        /// 答案对了
        UpdateCorrectAnswerUI(question);
    }
    else
    {
        /// 答案错了
        UpdateWrongAnswerUI(question);
    }
}

void MainWindow::UpdateCorrectAnswerUI(const QuestionData& question)
{
    std::string strResult = "正确答案!";
    ui->edtResult->setText(strResult.c_str());

    std::vector<std::string> vecNote = question.get_notes();
    std::string strNote;
    for (const auto& element : vecNote)
    {
        strNote = element + "\n";
    }

    ui->edtNote->setText(strNote);
}

void MainWindow::UpdateWrongAnswerUI(const QuestionData& question) {}

void MainWindow::slot_click_answer()
{
    controller_.write_answer(GetQuestionIndex(), group_selection_->checkedId());
}

int MainWindow::GetQuestionIndex()
{
    QString strQuestionIndex = ui->labelQuestionIndex->text();
    return strQuestionIndex.toInt();
}
