#include <iostream>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitQuestions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitQuestions()
{
    std::string question_body = "如果项目之间的联系仅限于共享顾主、供应商、技术或资源，则这些项目应该()";
    std::map<int, std::string> answer;
    answer[0] = "由项目管理力公室统一管理";
    answer[1] = "被作为项目组合来协调管理";
    answer[2] = "被作为项目集来协调管理";
    answer[3] = "被归入项目作战室统管理";
    int current_index = 0;
    std::vector<std::string> notes;
    notes.push_back(
        "解析：A.该答案有一定的道理，但不如B那么贴合题干。项目管理办公室的功能因组织而异。B."
        "正确答案。项目之间没有内在联系，只适合当作项目组合进行管理。C."
        "项目之间没有内在联系，通常不可能用项目集管理来获取额外利益。D.作战室是项目内的集中办公场所， "
        "用手团队建设，不是管理多个项目的机构。考点与页码，项目组合和项目集的区别。《指南》第14-15页");

    QuestionData question(question_body, answer, current_index, notes);
    controller_.add_question(question);
}

void MainWindow::start()
{
    /// 把 radio button 绑定在一个组内
    radio_selection_->addButton(ui->rbA, 0);
    radio_selection_->addButton(ui->rbB, 1);
    radio_selection_->addButton(ui->rbC, 2);
    radio_selection_->addButton(ui->rbD, 3);
    controller_.start();

    QuestionData question;
    int index = -1;
    if (controller_.get_next_question(question, index))
    {
        update_question(index, question);
    }
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

void MainWindow::UpdateCorrectAnswerUI(const QuestionData& question)
{
    std::string strResult = "回答正确!";
    ui->labelResult->setText(strResult.c_str());
    /// 设置颜色
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::green);
    ui->labelResult->setPalette(palette);
}

void MainWindow::UpdateWrongAnswerUI(const QuestionData& question)
{
    std::string strResult = "回答错误!";
    ui->labelResult->setText(strResult.c_str());
    /// 设置颜色
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->labelResult->setPalette(palette);
}

void MainWindow::slot_click_answer()
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_->checkedId());
}

int MainWindow::GetQuestionIndex()
{
    QString strQuestionIndex = ui->labelQuestionIndex->text();
    return strQuestionIndex.toInt();
}

void MainWindow::on_btnConfirm_clicked(bool checked)
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

    /// 正确答案显示
    std::string strCorrectAnswer;
    switch (question.get_correct_answer_index())
    {
        case 0:
            strCorrectAnswer = "A";
            break;
        case 1:
            strCorrectAnswer = "B";
            break;
        case 2:
            strCorrectAnswer = "C";
            break;
        case 3:
            strCorrectAnswer = "D";
            break;
        default:
            break;
    }

    ui->labelCorrectAnswer->setText(strCorrectAnswer.c_str());

    std::vector<std::string> vecNote = question.get_notes();
    std::string strNote;
    for (const auto& element : vecNote)
    {
        strNote = element + "\n";
    }

    ui->edtNote->setText(strNote.c_str());
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    start();
}
