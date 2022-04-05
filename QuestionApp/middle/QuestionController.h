#ifndef QUESTIONCONTROLLER_H
#define QUESTIONCONTROLLER_H

#include <map>
#include <QDebug>
#include "QuestionData.h"
#include "DataLoader.h"

class QuestionController
{
public:
    using QUESTION_ID_TYPE = int;
    QuestionController()
    {
        std::vector<QuestionData> vecQuestion;
        data_loader.query_all_question(vecQuestion);
        for(const auto& it : vecQuestion)
        {
            add_question(vecQuestion);
        }
    }

    /**
     * @brief 添加问题
     * @param data
     */
    void add_question(const QuestionData& data)
    {
        questions_[static_cast<int>(questions_.size())] = data;
    }

    /**
     * @brief 获取问题
     * @param id
     * @param question
     * @return
     */
    bool get_question(const QUESTION_ID_TYPE& id, QuestionData& question) const
    {
        auto it = questions_.find(id);
        if (questions_.end() == it)
        {
            return false;
        }

        question = it->second;
        return true;
    }

    /**
     * @brief 从第一题开始
     */
    void start()
    {
        current_question_index_ = -1;
        people_answer_.clear();
        is_test_finish_ = false;
    }

    /// 结束作答
    void finish(int& iCurrectQuestionNumber, int& iTotalQuestionNumber)
    {
        iTotalQuestionNumber = questions_.size();
        iCurrectQuestionNumber = 0;

        for (const auto& it : people_answer_)
        {
            if (compare_answer(it.first))
            {
                iCurrectQuestionNumber++;
            }
        }
        is_test_finish_ = true;
    }

    bool get_test_is_finish() const
    {
        return is_test_finish_;
    }

    /// 更新当前在哪个试题
    bool update_current_question_index(const int& current_index)
    {
        if (current_index < 0 || current_index >= questions_.size())
        {
            return false;
        }
        current_question_index_ = current_index;
        return true;
    }

    /**
     * @brief 获取下个问题
     * @param question  [out]
     * @return
     */
    bool get_next_question(QuestionData& question, QUESTION_ID_TYPE& index)
    {
        if (!get_question(++current_question_index_, question))
        {
            --current_question_index_;
            return false;
        }
        index = get_current_question_index();
        return true;
    }

    /**
     * @brief   获取上一个问题
     * @param   question  [out]
     * @param   index     [out]
     * @return
     */
    bool get_prev_question(QuestionData& question, QUESTION_ID_TYPE& index)
    {
        if (!get_question(--current_question_index_, question))
        {
            ++current_question_index_;
            return false;
        }
        index = get_current_question_index();
        return true;
    }

    /**
     * @brief   获取当前
     * @return
     */
    QUESTION_ID_TYPE get_current_question_index() const
    {
        return current_question_index_;
    }

    /**
     * @brief 写答案
     * @param id
     * @param answer
     */
    void write_answer(const QUESTION_ID_TYPE& id, const int& answer)
    {
        if (questions_.find(id) == questions_.end())
        {
            /// 这个题不存在就直接返回
            return;
        }
        qDebug() << "people_answer_[" << id << "] = " << answer;
        people_answer_[id] = answer;
    }

    /**
     * @brief   获取作答答案
     * @return  获取成功或失败
     */
    bool get_people_answer(const QUESTION_ID_TYPE& id, int& answer) const
    {
        auto it = people_answer_.find(id);
        if (it == people_answer_.end())
        {
            return false;
        }

        answer = it->second;
        return true;
    }

    /**
     * @brief 比较答案是否正确
     * @param id        [in]
     * @return
     */
    bool compare_answer(const QUESTION_ID_TYPE& id)
    {
        if (people_answer_.find(id) == people_answer_.end() || questions_.find(id) == questions_.end())
        {
            return false;
        }

        return people_answer_[id] == questions_[id].get_correct_answer_index();
    }

private:
    std::map<QUESTION_ID_TYPE, QuestionData> questions_;  ///< 题的 id 和信息
    QUESTION_ID_TYPE current_question_index_ = -1;        ///< 当前问题的 index
    std::map<QUESTION_ID_TYPE, int> people_answer_;       ///< 考生回答的问题
    bool is_test_finish_ = false;                         ///< 考试是否结束

    DataLoader data_loader;  ///< 数据加载器
};

#endif  // QUESTIONCONTROLLER_H
