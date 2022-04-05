#include <iostream>
#include <QMouseEvent>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitQuestions();

    /// 把 radio button 绑定在一个组内
    radio_selection_.addButton(ui->rbA, 0);
    radio_selection_.addButton(ui->rbB, 1);
    radio_selection_.addButton(ui->rbC, 2);
    radio_selection_.addButton(ui->rbD, 3);

    /// 设置本窗口背景色
    setStyleSheet("background-color:rgb(250,249,222);");

    /// 安装事件过滤器，为了实现标签点击事件的响应函数
    ui->labelAnswerA->installEventFilter(this);
    ui->labelAnswerB->installEventFilter(this);
    ui->labelAnswerC->installEventFilter(this);
    ui->labelAnswerD->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if (watched == ui->labelAnswerA)
            {
                ui->rbA->setChecked(true);
            }
            else if (watched == ui->labelAnswerB)
            {
                ui->rbB->setChecked(true);
            }
            else if (watched == ui->labelAnswerC)
            {
                ui->rbC->setChecked(true);
            }
            else if (watched == ui->labelAnswerD)
            {
                ui->rbD->setChecked(true);
            }
            else
            {
                // pass the event on to the parent class
                return QMainWindow::eventFilter(watched, event);
            }
        }
    }
    return false;
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
    ui->labelQuestion->setText(question.get_question().c_str());

    std::map<int, std::string> answers = question.get_answer();
    if (answers.size() != 4)
    {
        qDebug() << "Invalid Answer";
        return;
    }

    /// 保存问题的 index
    ui->labelQuestionIndex->setText(std::to_string(question_index + 1).c_str());

    /// 显示答案
    ui->labelAnswerA->setText(answers[0].c_str());
    ui->labelAnswerB->setText(answers[1].c_str());
    ui->labelAnswerC->setText(answers[2].c_str());
    ui->labelAnswerD->setText(answers[3].c_str());
}

void MainWindow::UpdateCorrectAnswerUI(const QuestionData& question)
{
    std::string strResult = "回答正确!";
    ui->labelResult->setText(strResult.c_str());
    /// 设置颜色
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::green);
    ui->labelResult->setPalette(palette);

    /// 正确答案显示
    std::string strCorrectAnswer = "正确答案是: ";
    switch (question.get_correct_answer_index())
    {
        case 0:
            strCorrectAnswer += "A";
            break;
        case 1:
            strCorrectAnswer += "B";
            break;
        case 2:
            strCorrectAnswer += "C";
            break;
        case 3:
            strCorrectAnswer += "D";
            break;
        default:
            break;
    }

    ui->labelCorrectAnswer->setText(strCorrectAnswer.c_str());
}

void MainWindow::UpdateWrongAnswerUI(const QuestionData& question)
{
    std::string strResult = "回答错误!";
    ui->labelResult->setText(strResult.c_str());
    /// 设置颜色
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->labelResult->setPalette(palette);

    std::string strCorrectAnswer = "正确答案是: ";
    switch (question.get_correct_answer_index())
    {
        case 0:
            strCorrectAnswer += "A";
            break;
        case 1:
            strCorrectAnswer += "B";
            break;
        case 2:
            strCorrectAnswer += "C";
            break;
        case 3:
            strCorrectAnswer += "D";
            break;
        default:
            break;
    }

    std::string strPeopleAnswer = "你的答案是: ";
    int people_answer = -1;
    if (controller_.get_people_answer(GetQuestionIndex(), people_answer))
    {
        switch (people_answer)
        {
            case 0:
                strPeopleAnswer += "A";
                break;
            case 1:
                strPeopleAnswer += "B";
                break;
            case 2:
                strPeopleAnswer += "C";
                break;
            case 3:
                strPeopleAnswer += "D";
                break;
            default:
                break;
        }
    }

    std::string strAnswerText = strCorrectAnswer + " " + strPeopleAnswer;
    ui->labelCorrectAnswer->setText(strAnswerText.c_str());
}

int MainWindow::GetQuestionIndex()
{
    QString strQuestionIndex = ui->labelQuestionIndex->text();
    return strQuestionIndex.toInt() - 1;
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

    std::vector<std::string> vecNote = question.get_notes();
    std::string strNote;
    for (const auto& element : vecNote)
    {
        strNote = element + "\n";
    }

    ui->edtNote->setText(strNote.c_str());
}

void MainWindow::on_start_test_triggered()
{
    start();
}

void MainWindow::on_rbA_toggled(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}

void MainWindow::on_rbB_toggled(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}

void MainWindow::on_rbC_toggled(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}

void MainWindow::on_rbD_toggled(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}
