//* Based code by Joshaven Potter
//* https://github.com/joshaven/string_score
//

#include <fstream>
#include <assert.h>
#include <iostream>
#include <iterator>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <set>
#include <time.h>


using namespace std;


#define MATCH_SCORE                     0.1
#define TOTAIL_ADD_START_CHAR_SCORE     0.1

double score( const string& word, const string& input, double fuzziness = 0 )
{
    auto wordLength = word.length();
    auto inputLength = input.length();

    if ( wordLength == inputLength && word == input )
        return 1;

    if ( inputLength == 0 )
        return 0;

    auto* wordNext = word.c_str();
    auto* inputString = input.c_str();
    
    int c;

    auto score = 0.0;
    int start = 0;
    int wordFindIndex;

    double fuzzies = 1.0;

    int startStringIndex = 0;
    int i = 0;
    while( 1 )
    {
    LOOP:;
        if ( i == inputLength )
        {
            break;
        }
        //不区分大小
        c = tolower( inputString[i]);

        wordFindIndex = 0;

        while ( 1 )
        {
            if ( wordNext[wordFindIndex] == '\0' )
            {
                if ( fuzziness > 0.0 )
                {
                    fuzzies += 1.0 - fuzziness;
                    ++i;
                    goto LOOP;
                }

                //没有设定模糊度
                return 0.0;
            }
            else if ( tolower( wordNext[wordFindIndex] ) == c )
            {
                if ( wordNext[wordFindIndex] == c )
                {
                    score += (MATCH_SCORE + MATCH_SCORE);
                    break;
                }
                score += MATCH_SCORE;
                break;
            }
            ++wordFindIndex;
        }
        //来到这里就相当于找到了


        //算连续字母开始的分数
        if ( wordFindIndex == 0 ) //==0 说明跟上个字符连续
        {
            score += 0.7;

            if ( i == 0 ) //单词（word）与用户输入的(input) 第一个相等
            {
                startStringIndex = 1;
            }
        }
        else if ( wordNext[wordFindIndex - 1] == ' ' ) //字母的开始
        {
            score += 0.8;
        }

        wordNext += wordFindIndex + 1;

        ++i;
    }

    score /= (double)inputLength; //此时score 转换为 input 每个字符的得分。

    score = ((score * (inputLength / (double)wordLength)) + score) / 2;
    score /= fuzzies;

    if ( startStringIndex && (score + TOTAIL_ADD_START_CHAR_SCORE < 1) )
    {
        score += TOTAIL_ADD_START_CHAR_SCORE;
    }

    return score;
}


int main()
{
    double x = score( "hello world", "hello wor1", 1 );
    x = score( "hello world", "he" );

    return 0;
}