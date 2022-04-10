#ifndef DATALOADER_H
#define DATALOADER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "QuestionData.h"

class db_pmp
{
public:
    int id;
    std::string question;
    std::string answers;
    std::string correct_answer_indexes;
    std::string notes;
    std::string labels;
};

class DataLoader
{
public:
    DataLoader()
    {
        if (!connect("lijiancong"))
        {
            qDebug() << "connect failed!";
        }
    }

    ~DataLoader()
    {
        database.close();
    }

    bool query_all_question(std::vector<QuestionData>& vecQuestion)
    {
        /// sql_log_trace("query_all_question: ");
        QSqlQuery sql_query(database);
        QString query_sql = "SELECT id, question, answers, correct_answer_indexes, notes, labels FROM pmp";
        sql_query.prepare(query_sql);
        if (sql_query.exec(query_sql))
        {
            std::vector<db_pmp> vec;
            while (sql_query.next())
            {
                db_pmp data;
                data.id = sql_query.value(0).toInt();
                data.question = sql_query.value(1).toString().toStdString();
                data.answers = sql_query.value(2).toString().toStdString();
                data.correct_answer_indexes = sql_query.value(3).toString().toStdString();
                data.notes = sql_query.value(4).toString().toStdString();
                data.labels = sql_query.value(5).toString().toStdString();

                vec.push_back(data);
            }

            for (const auto& it : vec)
            {
                vecQuestion.push_back(convert_db_pmp_to_question(it));
            }
            return true;
        }
        else
        {
            /// sql_log_error("SqliteDataBaseOperation::query_all_address_book() error: " +
            ///              sql_query.lastError().text().toStdString());
            return false;
        }
    }

private:
    QSqlDatabase database;  ///< 数据库连接实例
    QuestionData convert_db_pmp_to_question(const db_pmp& it)
    {
        QuestionData question;

        question.id = it.id;
        question.question_ = it.question;

        std::vector<std::string> vecAnswers;
        string_split(it.answers, vecAnswers, "`,`");
        int index_answers = 0;
        for (const auto& it : vecAnswers)
        {
            question.answer_[index_answers++] = it;
        }

        std::vector<std::string> vecIndex;
        string_split(it.correct_answer_indexes, vecIndex, "`,`");
        for (const auto& it : vecIndex)
        {
            int i = std::stoi(it);
            question.correct_answer_index_.push_back(i);
        }

        string_split(it.notes, question.notes_, "`,`");
        string_split(it.labels, question.label_, "`,`");
        return question;
    }

    bool connect(const std::string& connection_name, const std::string& path = "./sql/pmp.sqlite")
    {
        /// sql_log_trace("SqliteDataBaseOperation::connect()");
        if (QSqlDatabase::contains(connection_name.c_str()))
        {
            database = QSqlDatabase::database(QLatin1String(connection_name.c_str()), false);
            /// database = QSqlDatabase::database(QLatin1String((database_name_+"_connection").c_str()), false);
        }
        else
        {
            database = QSqlDatabase::addDatabase("QSQLITE", connection_name.c_str());
        }

        database.setDatabaseName(path.c_str());
        if (!database.isValid())
        {
            /// sql_log_error("error connector in database!");
            throw "error connector in database";
        }

        if (!database.open())
        {
            /// sql_log_error("open database error");
            throw "open database error";
        }

        return true;
    }

    inline unsigned string_split(const std::string& s, std::vector<std::string>& tokens,
                                 const std::string& delimiters = "`,`")
    {
        std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
        std::string::size_type pos = s.find_first_of(delimiters, lastPos);
        while (std::string::npos != pos || std::string::npos != lastPos)
        {
            tokens.push_back(s.substr(lastPos, pos - lastPos));
            lastPos = s.find_first_not_of(delimiters, pos);
            pos = s.find_first_of(delimiters, lastPos);
        }
        return (unsigned)tokens.size();
    }
};

#if 0
INSERT INTO pmp(question, answers, correct_answer_indexes, notes, labels)
VALUES(
"如果项目之间的联系仅限于共享顾主、供应商、技术或资源，则这些项目应该(）",
"由项目管理力公室统管理`,`被作为项目组合来协调管理`,`被作为项目集来协调管理`,`被归入项目作战室统管理`,`",
 "0",
 "解析：A.该答案有一定的道理，但不如B那么贴合题干。项目管理办公室的功能因组织而异。B.正确
答案。项目之间没有内在联系，只适合当作项目组合进行管理。C.项目之间没有内在联系，
通常不可能用项目集管理来获取额外利益。D.作战室是项目内的集中办公场所， 用手团队建
设，不是管理多个项目的机构。考点与页码，项目组合和项目集的区别。
《指南》第14-15页",
 "");

INSERT INTO pmp(question, answers, correct_answer_indexes, notes, labels)
VALUES(
"以下哪种方法能使组织资源得到最大利用以支持组织战略的实现?",
"PMO统一管理`,`项目组合管理`,`项目集管理`,`项目管理",
"1",
"解析：A.缺三针对性。PMO为项目工作提供支持，有三种不同类型的PMO。B.正确答案。项目组合
强调服务于组织战略，对项目进行优先级排序，实现组织资源价值最大化。、C.项目集也支
持组织战略，但不是直接的，而是通过项目组合。D项目也通过项目组合来支持组织战略的
实现。考点与页码：项目组合管理的关注重点。《指南》第15页",
""
);


INSERT INTO pmp(question, answers, correct_answer_indexes, notes, labels)
VALUES(
"以下关于项目管理过程的哪个说法是正确的？",
"运用工貝与技术把输入转化成输出`,`由特定的人把输入转化成输出`,`在特定的时间把输入转化成输出`,`按特定的顺序把输入转化成输出",
"0",
"解析：A.正确答案。每一个过程都由输入、工具与技术、输出构成。B. 《指南》中没有强调某个过
程应该由谁来开展。C.这个说法不是绝对错误的，但不如A那么准确。D.这个说法不是绝对错
误的，但不如A那么准确。考点与页码：项目管理过程的构成和作用。《指南》第22页。",
""
);

#endif

#endif  // DATALOADER_H
