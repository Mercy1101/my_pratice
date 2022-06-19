/**
 * @file            kmp.h
 * @brief           KMP 算法实现
 * @author          lijiancong(lijiancong@gbcom.com.cn)
 * @version         0.1
 * @date            2022-06-19 18:44:15
 * @copyright       Copyright (c) 2021-2022 GBCOM
 */

#ifndef PRATICE_KMP_H__
#define PRATICE_KMP_H__
/// [最浅显易懂的 KMP
/// 算法讲解](https://www.bilibili.com/video/BV1AY4y157yL?spm_id_from=333.337.search-card.all.click&vd_source=003277bbac87532b8fd4a5d0a6f6de3c)
class KMP
{
public:
    static size_t kmp(const std::string& str, const std::string& patt)
    {
        auto next = findNext(patt);

        auto i = 0;  ///< 主串下标
        auto j = 0;  ///< 字串下标

        while (i < str.size())
        {
            if (str[i] == patt[j])
            {
                /// 匹配成功，自增
                i++;
                j++;
            }
            else if (j > 0)
            {
                /// 子串失配，根据下表跳转
                j = next[j - 1];
            }
            else
            {
                /// 首字母失配， i++
                i += 1;
            }

            /// 寻找到子串末尾成功匹配
            if (j == patt.size())
            {
                return i - j;
            }
        }
    }

private:
    std::vector<int> findNext(const std::string& patt)
    {
        std::vector<int> vec;
        vec.push_back(0);

        int prefix_len = 0;
        int i = 1;

        while (i < patt.size())
        {
            if (patt[prefix_len] == patt[i])
            {
                /// 前缀后缀匹配成功，两个都自增
                vec.push_back(++prefix_len);
                i++;
            }
            else
            {
                if (prefix_len == 0)
                {
                    /// 第一个就没有失配，i++
                    prefix_len = 0;
                    i++;
                }
                else
                {
                    prefix_len = vec[prefix_len - 1];
                }
            }
        }
        return vec;
    }
};

#endif