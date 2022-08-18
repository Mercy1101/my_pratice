/**
 * @file            kmp.h
 * @brief           KMP �㷨ʵ��
 * @author          lijiancong(lijiancong@gbcom.com.cn)
 * @version         0.1
 * @date            2022-06-19 18:44:15
 * @copyright       Copyright (c) 2021-2022 GBCOM
 */

#ifndef PRATICE_KMP_H__
#define PRATICE_KMP_H__

#include <string>
#include <vector>

/// [��ǳ���׶��� KMP
/// �㷨����](https://www.bilibili.com/video/BV1AY4y157yL?spm_id_from=333.337.search-card.all.click&vd_source=003277bbac87532b8fd4a5d0a6f6de3c)
class KMP
{
public:
    static size_t kmp(const std::string& str, const std::string& patt)
    {
        auto next = findNext(patt);

        auto i = 0;  ///< �����±�
        auto j = 0;  ///< �ִ��±�

        while (i < str.size())
        {
            if (str[i] == patt[j])
            {
                /// ƥ��ɹ�������
                i++;
                j++;
            }
            else if (j > 0)
            {
                /// �Ӵ�ʧ�䣬�����±���ת
                j = next[j - 1];
            }
            else
            {
                /// ����ĸʧ�䣬 i++
                i += 1;
            }

            /// Ѱ�ҵ��Ӵ�ĩβ�ɹ�ƥ��
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
                /// ǰ׺��׺ƥ��ɹ�������������
                vec.push_back(++prefix_len);
                i++;
            }
            else
            {
                if (prefix_len == 0)
                {
                    /// ��һ����û��ʧ�䣬i++
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

#include <iostream>

using namespace std;

#define FOR(i, j, n, m)         \
    for (int i = 0; i < n; i++) \
        for (int j = 0; j < m; j++)

int n, m;
char maze[101][101];

//��x��y��Ϊ��ǰ������λ��
void dfs(int x, int y)
{
    //�ӣ�x��y����������������ͨ�ĵ㶼����
    maze[x][y] = '.';
    int dx, dy;
    //����8������
    for (dx = -1; dx <= 1; dx++)
    {
        for (dy = -1; dy <= 1; dy++)
        {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < n && ny >= 0 && ny < m && maze[nx][ny] == 'W')
            {
                dfs(nx, ny);
            }
        }
    }
}

int main()
{
    int i, j;
    cin >> n >> m;
    FOR(i, j, n, m) cin >> maze[i][j];
    int ans = 0;
    FOR(i, j, n, m)
    {
        if (maze[i][j] == 'W')
        {
            dfs(i, j);
            ans++;
        }
    }
    cout << ans << endl;
    return 0;
}

//��x��y��Ϊ��ǰ������λ��
void dfs(int x, int y)
{
    //�ӣ�x��y����������������ͨ�ĵ㶼����
    if (x >= 0 && x < n && y >= 0 && y < m && maze[x][y] == 'W')
    {
        maze[x][y] = '.';
        int dx, dy;
        //����8������
        for (dx = -1; dx <= 1; dx++)
        {
            for (dy = -1; dy <= 1; dy++)
            {
                int nx = x + dx;
                int ny = y + dy;
                {
                    dfs(nx, ny);
                }
            }
        }
    }
    int i = 10;
}
