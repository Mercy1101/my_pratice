#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "middle/QuestionController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void on_btnConfirm_clicked();
    void slot_click_answer();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QButtonGroup *group_selection_;
    QuestionController controller_;

    int GetQuestionIndex();

    void update_question(const int &question_index, const QuestionData &question);
    void UpdateWrongAnswerUI(const QuestionData &question);
    void UpdateCorrectAnswerUI(const QuestionData &question);
};
#endif // MAINWINDOW_H
