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
    /// 添加问题
    InitQuestions();

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

void MainWindow::InitQuestions()
{
    std::string question_body = "如果项目之间的联系仅限于共享顾主、供应商、技术或资源，则这些项目应该()";
    std::map<int, std::string> answer;
    answer[0] = "由项目管理力公室统一管理";
    answer[1] = "被作为项目组合来协调管理";
    answer[2] = "被作为项目集来协调管理";
    answer[3] = "被归入项目作战室统管理";
    int correct_index = 0;
    std::vector<std::string> notes;
    notes.push_back(
        "解析：A.该答案有一定的道理，但不如B那么贴合题干。项目管理办公室的功能因组织而异。B."
        "正确答案。项目之间没有内在联系，只适合当作项目组合进行管理。C."
        "项目之间没有内在联系，通常不可能用项目集管理来获取额外利益。D.作战室是项目内的集中办公场所， "
        "用手团队建设，不是管理多个项目的机构。考点与页码，项目组合和项目集的区别。《指南》第14-15页");

    QuestionData question(question_body, answer, correct_index, notes);
    controller_.add_question(question);

    std::string question_body0 = "1如果项目之间的联系仅限于共享顾主、供应商、技术或资源，则这些项目应该()";
    std::map<int, std::string> answer0;
    answer0[0] = "1由项目管理力公室统一管理";
    answer0[1] = "1被作为项目组合来协调管理";
    answer0[2] = "1被作为项目集来协调管理";
    answer0[3] = "1被归入项目作战室统管理";
    int correct_index0 = 0;
    std::vector<std::string> notes0;
    notes0.push_back(
        "1 解析：A.该答案有一定的道理，但不如B那么贴合题干。项目管理办公室的功能因组织而异。B."
        "正确答案。项目之间没有内在联系，只适合当作项目组合进行管理。C."
        "项目之间没有内在联系，通常不可能用项目集管理来获取额外利益。D.作战室是项目内的集中办公场所， "
        "用手团队建设，不是管理多个项目的机构。考点与页码，项目组合和项目集的区别。《指南》第14-15页");

    QuestionData question0(question_body0, answer0, correct_index0, notes0);
    controller_.add_question(question0);

    /// 第三道题
    std::string question_body1 = "2如果项目之间的联系仅限于共享顾主、供应商、技术或资源，则这些项目应该()";
    std::map<int, std::string> answer1;
    answer1[0] = "2由项目管理力公室统一管理";
    answer1[1] = "2被作为项目组合来协调管理";
    answer1[2] = "2被作为项目集来协调管理";
    answer1[3] = "2被归入项目作战室统管理";
    int correct_index1 = 0;
    std::vector<std::string> notes1;
    notes1.push_back(
        "1 解析：A.该答案有一定的道理，但不如B那么贴合题干。项目管理办公室的功能因组织而异。B."
        "正确答案。项目之间没有内在联系，只适合当作项目组合进行管理。C."
        "项目之间没有内在联系，通常不可能用项目集管理来获取额外利益。D.作战室是项目内的集中办公场所， "
        "用手团队建设，不是管理多个项目的机构。考点与页码，项目组合和项目集的区别。《指南》第14-15页");

    QuestionData question1(question_body1, answer1, correct_index1, notes1);
    controller_.add_question(question1);
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
        QString strColor = "color: rgb(50,118,98);";
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
