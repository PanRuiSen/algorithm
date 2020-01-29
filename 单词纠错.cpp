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

//http://norvig.com/spell-correct.html


inline int& GetValue( vector<int>& vet, int r, int c, int colSize )
{
    return vet[((r) * (colSize)) + (c)];
}

int EditDistance( const string& a, const string& b )
{
    //#define MY_GET_VALUE(r, c)\
    //    ((r) * (col)) + (c)


    int m = a.length(), n = b.length();
    int row = (m + 1), col = (n + 1);

    vector<int> dp;
    dp.resize( row * col );

    for ( int i = 0; i <= m; i++ )
        GetValue( dp, i, 0, col ) = i;

    for ( int j = 0; j <= n; j++ )
        GetValue( dp, 0, j, col ) = j;


    for ( int i = 1; i <= m; i++ )
    {
        for ( int j = 1; j <= n; j++ )
        {
            if ( a[i - 1] != b[j - 1] )
            {

                GetValue( dp, i, j, col ) = min(
                    //abc -> abd
                    //cbd -> cbd
                    1 + GetValue( dp, i - 1, j - 1, col ),   //替换  
                    min(
                        //abc -> ab
                        //cbd -> cbd    
                        1 + GetValue( dp, i - 1, j, col ),

                        //abc -> abcd
                        //cbd ->  cbd    
                        1 + GetValue( dp, i, j - 1, col )
                    )
                );
            }
            else
                GetValue( dp, i, j, col ) = GetValue( dp, i - 1, j - 1, col );
        }
    }


    return GetValue( dp, m, n, col );

#undef MY_GET_VALUE
}

struct Node
{
    Node( const string& word ) :word( word )
    {

    }
    Node()
    {

    }

    void Insert( const std::string& text )
    {
        if ( count == 0 )
        {
            word = text;
            count = 1;
        }

        int dis = EditDistance( word, text );

        if ( dis == 0 )
        {
            ++count;
            return;
        }

        auto it = childens.insert( { dis, Node() } );

        if ( it.second )
        {
            it.first->second = text;
            it.first->second.count = 1;
        }

        it.first->second.Insert( text );
    }

    void GetWords( map<int, vector<string>>& strings, const string& s, int n )
    {
        if ( word.empty() )
            return;

        int dist = EditDistance( word, s );

        if ( dist <= n )
        {
            strings[count].push_back( word );
        }

        /*
                        x
            A
        B
        D(X,A) = x
        D(A,B) = z
        D(X,B) = n

        n <= x + z -----> z >= n - x
        x <= n + z -----> z <= x - n

        */

        for ( auto& child : childens )
        {
            if ( (dist - n) <= child.first && child.first <= (dist + n) )
            {
                child.second.GetWords( strings, s, n );
            }
        }
    }

    string word;
    size_t count = 0;
    map<int, Node> childens;
};

class BKTree
{
public:
    BKTree()
    {

    }
    ~BKTree()
    {

    }

    void Insert( const std::string& text )
    {
        if ( text.empty() )
            return;

        _root.Insert( text );
    }

    void GetWords( map<int, vector<string>>& strings, const string& s, int n )
    {
        strings.clear();

        return _root.GetWords( strings, s, n );
    }


private:
    Node _root;
};

int main()
{
    ifstream is("单词本分割 (2)");
    istream_iterator<string> in{ is }, end;

    BKTree tree;

    while ( in != end )
    {
        string str = *in;
        ++in;

        //转小写
        transform( str.begin(), str.end(), str.begin(), ::tolower );

        auto it1 = find_if( str.begin(), str.end(), [](char ch) {
            return isalpha( ch ) != 0;
            } );
        if ( it1 == str.end() )
            continue;

        auto it2 = find_if( str.rbegin(), str.rend(), []( char ch ) {
            return isalpha( ch ) != 0;
            } );

        str = string( it1, it2.base() );

        tree.Insert( str );
    }

    map<int, vector<string>> words;

    tree.GetWords( words, "ther", 1 );

    return 0;
}