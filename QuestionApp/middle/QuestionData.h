#ifndef QUESTIONDATA_H
#define QUESTIONDATA_H

#include <string>
#include <map>

class QuestionData
{
    using ANSWER_INDEX_TYPE = int;  ///< 答案索引类型
public:
    QuestionData(const std::string& question, const std::map<int, std::string>& answer, const int& correct_answer_index)
        : question_(question), answer_(answer), correct_answer_index_(correct_answer_index)
    {
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
        return correct_answer_index_;
    }

private:
    std::string question_;                             ///< 问题的文字描述
    std::map<ANSWER_INDEX_TYPE, std::string> answer_;  ///< 答案以及对应编号
    ANSWER_INDEX_TYPE correct_answer_index_;           ///< 正确问题的编号
};

#endif // QUESTIONDATA_H
