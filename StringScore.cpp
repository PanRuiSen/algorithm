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
        //�����ִ�С
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

                //û���趨ģ����
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
        //����������൱���ҵ���


        //��������ĸ��ʼ�ķ���
        if ( wordFindIndex == 0 ) //==0 ˵�����ϸ��ַ�����
        {
            score += 0.7;

            if ( i == 0 ) //���ʣ�word�����û������(input) ��һ�����
            {
                startStringIndex = 1;
            }
        }
        else if ( wordNext[wordFindIndex - 1] == ' ' ) //��ĸ�Ŀ�ʼ
        {
            score += 0.8;
        }

        wordNext += wordFindIndex + 1;

        ++i;
    }

    score /= (double)inputLength; //��ʱscore ת��Ϊ input ÿ���ַ��ĵ÷֡�

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