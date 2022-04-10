#ifndef QUESTIONDATA_H
#define QUESTIONDATA_H

#include <string>
#include <map>
#include <set>
#include <vector>

class QuestionData
{
    using ANSWER_INDEX_TYPE = int;  ///< 答案索引类型
public:
    QuestionData() {}
    QuestionData(const std::string& question, const std::map<int, std::string>& answer, const int& correct_answer_index,
                 const std::vector<std::string>& notes, const std::vector<std::string>& label)
        : question_(question),
          answer_(answer),
          correct_answer_index_(correct_answer_index),
          notes_(notes),
          label_(label)
    {
    }

    QuestionData(const QuestionData& other)
    {
        question_ = other.question_;
        answer_ = other.answer_;
        correct_answer_index_ = other.correct_answer_index_;
        notes_ = other.notes_;
        label_ = other.label_;
    }

    QuestionData& operator=(const QuestionData& other)
    {
        if (&other != this)
        {
            question_ = other.question_;
            answer_ = other.answer_;
            correct_answer_index_ = other.correct_answer_index_;
            notes_ = other.notes_;
            label_ = other.label_;
        }

        return *this;
    }

    std::string get_question() const
    {
        return question_;
    }

    std::map<int, std::string> get_answer() const
    {
        return answer_;
    }

    ANSWER_INDEX_TYPE get_correct_answer_index() const
    {
        return correct_answer_index_.at(0);
    }

    std::vector<std::string> get_notes() const
    {
        return notes_;
    }

public:
    int id; ///< 本问题的唯一标识符
    std::string question_;                                 ///< 问题的文字描述
    std::map<ANSWER_INDEX_TYPE, std::string> answer_;      ///< 答案以及对应编号
    std::vector<ANSWER_INDEX_TYPE> correct_answer_index_;  ///< 正确问题的编号
    std::vector<std::string> notes_;                       ///< 备注/详解
    std::vector<std::string> label_;                       ///< 标签
};

#endif  // QUESTIONDATA_H
