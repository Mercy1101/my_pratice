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
    void on_btnConfirm_clicked(bool checked);

    void on_start_test_triggered();

    void on_rbA_toggled(bool checked);

    void on_rbB_toggled(bool checked);

    void on_rbC_toggled(bool checked);

    void on_rbD_toggled(bool checked);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;

    QButtonGroup radio_selection_;
    QuestionController controller_;

    /// 获取问题的 index
    int GetQuestionIndex();
    /// 开始作答
    void start();
    /// 更新问题
    void update_question(const int &question_index, const QuestionData &question);
    /// 更新正确答案显示
    void UpdateWrongAnswerUI(const QuestionData &question);
    /// 更新错误答案显示
    void UpdateCorrectAnswerUI(const QuestionData &question);
    /// 初始化题库
    void InitQuestions();
};
#endif // MAINWINDOW_H
