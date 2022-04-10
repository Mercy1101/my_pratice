#include <iostream>
#include <QMouseEvent>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /// 初始化界面
    InitUI();

    /// 安装事件过滤器，为了实现标签点击事件的响应函数
    ui->labelAnswerA->installEventFilter(this);
    ui->labelAnswerB->installEventFilter(this);
    ui->labelAnswerC->installEventFilter(this);
    ui->labelAnswerD->installEventFilter(this);

    /// 开始测试
    start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUI()
{
    /// 把 radio button 绑定在一个组内
    radio_selection_.addButton(ui->rbA, 0);
    radio_selection_.addButton(ui->rbB, 1);
    radio_selection_.addButton(ui->rbC, 2);
    radio_selection_.addButton(ui->rbD, 3);

    /// 设置本窗口背景色
    setStyleSheet("background-color:rgb(250,249,222);");
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
            /// 保存当前答案
            controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
        }
    }
    return false;
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

    ui->labelAnswerA->setStyleSheet("color: black;");
    ui->labelAnswerB->setStyleSheet("color: black;");
    ui->labelAnswerC->setStyleSheet("color: black;");
    ui->labelAnswerD->setStyleSheet("color: black;");
    /// 清空其他控件
    ui->labelCorrectAnswer->clear();
    ui->edtNote->clear();
    ui->labelResult->clear();

    QAbstractButton* checked = radio_selection_.checkedButton();
    if (checked)
    {
        radio_selection_.setExclusive(false);
        checked->setChecked(false);
        radio_selection_.setExclusive(true);
    }

    if (controller_.get_test_is_finish())
    {
        /// 考试结束则直接查看答案
        CheckAndShowAnswer();
        /// 选中正确答案
        QuestionData question;
        if (!controller_.get_question(GetQuestionIndex(), question))
        {
            qDebug() << "get_question failed!";
        }

        /// QPalette palette;
        /// palette.setColor(QPalette::WindowText, Qt::green);
        /// ui->labelAnswerA->setPalette(palette);
        /// QString strColor = "color: rgb(50,118,98);";
        QString strColor = "color: rgb(70,195,163);";
        switch (question.get_correct_answer_index())
        {
            case 0:
                ui->rbA->setChecked(true);
                ui->labelAnswerA->setStyleSheet(strColor);
                break;
            case 1:
                ui->rbB->setChecked(true);
                ui->labelAnswerB->setStyleSheet(strColor);
                break;
            case 2:
                ui->rbC->setChecked(true);
                ui->labelAnswerC->setStyleSheet(strColor);
                break;
            case 3:
                ui->rbD->setChecked(true);
                ui->labelAnswerD->setStyleSheet(strColor);
                break;
            default:
                break;
        }
    }
    else
    {
        /// 考试未结束
        /// 判断有没有作答过
        int answer = -1;
        if (controller_.get_people_answer(GetQuestionIndex(), answer))
        {
            switch (answer)
            {
                case 0:
                    ui->rbA->setChecked(true);
                    break;
                case 1:
                    ui->rbB->setChecked(true);
                    break;
                case 2:
                    ui->rbC->setChecked(true);
                    break;
                case 3:
                    ui->rbD->setChecked(true);
                    break;
                default:
                    break;
            }
        }
    }
}

void MainWindow::on_btnNextQuestion_clicked(bool checked)
{
    QuestionData question;
    int index = -1;
    if (controller_.get_next_question(question, index))
    {
        update_question(index, question);
    }
}

void MainWindow::on_btnPrevQuestion_clicked(bool checked)
{
    QuestionData question;
    int index = -1;
    if (controller_.get_prev_question(question, index))
    {
        update_question(index, question);
    }
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
                strPeopleAnswer += "未作答";
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

void MainWindow::CheckAndShowAnswer()
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
void MainWindow::on_btnConfirm_clicked(bool checked)
{
    CheckAndShowAnswer();
}

void MainWindow::on_start_test_triggered()
{
    start();
}

void MainWindow::on_finish_test_triggered()
{
    int iTotalQuestionNumber;
    int iCurrectQuestionNumber;
    /// 结束测试
    controller_.finish(iCurrectQuestionNumber, iTotalQuestionNumber);
    /// 结果弹窗
    std::string strResult =
        "测试结果: " + std::to_string(iCurrectQuestionNumber) + " / " + std::to_string(iTotalQuestionNumber) + " 道";
    QMessageBox::information(this, "正确: ", strResult.c_str(), QMessageBox::Ok);

    /// 更新当前界面
    if (!controller_.update_current_question_index(0))
    {
        qDebug() << "update_current_question_index error";
        return;
    }
    else
    {
        QuestionData question;
        if (!controller_.get_question(0, question))
        {
            qDebug() << "get_question failed!";
            return;
        }

        update_question(0, question);
    }
}

void MainWindow::on_rbA_clicked(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}

void MainWindow::on_rbB_clicked(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}

void MainWindow::on_rbC_clicked(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}

void MainWindow::on_rbD_clicked(bool checked)
{
    controller_.write_answer(GetQuestionIndex(), radio_selection_.checkedId());
}
