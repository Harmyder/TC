#include "stdafx.h"
#include "ParenthesisRemoval.h"

using namespace std;

namespace tasks {
    enum class ParType : short { Open, Close };
    ParType ToParType(char ch) {
        if (ch == '(') return ParType::Open;
        if (ch == ')') return ParType::Close;
        throw;
    }

    struct S {
        short pos;
        int count;
        int used;
        int closingMatchIndex;
    };

    ParType ParTypeFromSize(size_t sz) {
        if (sz % 2 == 0) return ParType::Close;
        if (sz % 2 == 1) return ParType::Open;
    }

    ParType ParTypeOfLast(const vector<S>& v) {
        return ParTypeFromSize(v.size());
    }

    void computePreInfo(const string& s, vector<S>& preInfo) {
        unordered_map<int, int> lastSeen;
        int level = 1;
        lastSeen[0] = 0;
        preInfo.push_back({ (short)0, 1, 0, -1 });
        for (int i = 1; i < (int)s.size(); ++i) {
            const auto t = ToParType(s[i]);
            if (t == ParType::Close && ParTypeOfLast(preInfo) == ParType::Close) ++preInfo.back().count;
            else {
                if (t == ParType::Open) {
                    lastSeen[level] = (int)preInfo.size();
                    preInfo.push_back({ (short)i, 1, 0, -1 });
                }
                else {
                    preInfo.push_back({ (short)i, 1, 0, -1 });
                }
            }
            if (t == ParType::Open) ++level;
            else {
                --level;
                preInfo[lastSeen[level]].closingMatchIndex = (int)preInfo.size() - 1;
            }
        }

        for (int i = 0; i < (int)preInfo.size(); ++i) {
            const auto& p = preInfo[i];
            cout << i << ":\tpos " << p.pos << ", count " << p.count << ", used " << p.used << ", closingMatchIndex " << p.closingMatchIndex << endl;
        }
    }

    int ParenthesisRemoval::countWays(string s) {
        if (s.size() == 0) return 0;

        vector<S> preInfo;
        computePreInfo(s, preInfo);

        long long res = 1;
        for (int i = 0; i < (int)preInfo.size(); ++i) {
            const ParType t = ParTypeFromSize(i + 1);
            const auto& p = preInfo[i];
            if (t == ParType::Open) {
                auto& pc = preInfo[p.closingMatchIndex];
                const int avail = pc.count - pc.used;
                res *= avail;
                ++pc.used;
            }
        }

        return res;
    }
}
