#include <fstream>
#include <assert.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <time.h>

using namespace std;


//
//inline int& GetValue( vector<int>& vet, int r, int c, int colSize )
//{
//    return vet[((r) * (colSize)) + (c)];
//}
//
//int EditDistance( const string& a, const string& b )
//{
//    //#define MY_GET_VALUE(r, c)\
//    //    ((r) * (col)) + (c)
//
//
//    int m = a.length(), n = b.length();
//    int row = (m + 1), col = (n + 1);
//
//    vector<int> dp;
//    dp.resize( row * col );
//
//    for ( int i = 0; i <= m; i++ )
//        GetValue( dp, i, 0, col ) = i;
//
//    for ( int j = 0; j <= n; j++ )
//        GetValue( dp, 0, j, col ) = j;
//
//
//    for ( int i = 1; i <= m; i++ )
//    {
//        for ( int j = 1; j <= n; j++ )
//        {
//            if ( a[i - 1] != b[j - 1] )
//            {
//
//                GetValue( dp, i, j, col ) = min(
//                    //abc -> abd
//                    //cbd -> cbd
//                    1 + GetValue( dp, i - 1, j - 1, col ),   //Ìæ»»  
//                    min(
//                        //abc -> ab
//                        //cbd -> cbd    
//                        1 + GetValue( dp, i - 1, j, col ),
//
//                        //abc -> abcd
//                        //cbd ->  cbd    
//                        1 + GetValue( dp, i, j - 1, col )
//                    )
//                );
//            }
//            else
//                GetValue( dp, i, j, col ) = GetValue( dp, i - 1, j - 1, col );
//        }
//    }
//
//
//    return GetValue( dp, m, n, col );
//
//#undef MY_GET_VALUE
//}

int EditDistance( const string& a, const string& b )
{
    int m = a.length(), n = b.length();
    int col = (n + 1);

    vector<int> dp;
    dp.resize( 2 * col );

    int* pre = dp.data();
    int* cur = pre + col;

    for ( int j = 0; j <= n; j++ )
    {
        pre[j] = j;
    }


    for ( int i = 1; i <= m; i++ )
    {
        cur[0] = i;
        for ( int j = 1; j <= n; j++ )
        {
            if ( a[i - 1] != b[j - 1] )
            {

                cur[j] = min(
                    //abc -> abd
                    //cbd -> cbd
                    1 + pre[j - 1],   //Ìæ»»  
                    min(
                        //abc -> ab
                        //cbd -> cbd    
                        1 + pre[j],

                        //abc -> abcd
                        //cbd ->  cbd    
                        1 + cur[j - 1]
                    )
                );
            }
            else
                cur[j] = pre[j - 1];
        }
        swap( pre, cur );
    }


    return pre[n];

}


struct Node
{  
    Node( const string& word ) :word( word )
    {

    }
    Node() 
    {
    
    }

    bool Insert( const std::string& text )
    {
        if ( word.empty() )
        {
            word = text;
            return true;
        }

        
        int dis = EditDistance( word, text );

        if ( dis == 0 )
            return false;

        auto it = childens.insert( { dis, Node()} );

        if ( it.second )
        {
            it.first->second = text;
            return true;
        }

        return it.first->second.Insert( text );
    }

    void GetWords( vector<string>& strings, const string& s, int n )
    {
        if ( word.empty() )
            return;

        int dist = EditDistance( word, s );

        if ( dist <= n )
            strings.push_back( word );

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

        /*for ( auto& child : childens )
        {
            if ( (dist - n) <= child.first && child.first <= (dist + n) )
            {
                child.second.GetWords( strings, s, n );
            }
        }*/


        auto it1 = childens.lower_bound( (dist - n) );
        auto it2 = childens.upper_bound( (dist + n) );

        while ( it1 != it2 )
        {
            it1->second.GetWords( strings, s, n );
            ++it1;
        }
    }



    string word;
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

    bool Insert(const std::string& text)
    {
        if ( text.empty() )
            return false;

        return _root.Insert(text);
    }

    void GetWords( vector<string>& strings, const string& s, int n )
    {
        strings.clear();

        return _root.GetWords( strings, s, n );
    }


private:
    Node _root;
};


int main()
{
    //{
    //    BKTree tree;
    //    assert( tree.Insert( "" ) == false );
    //    tree.Insert( "a" );
    //    tree.Insert( "b" );
    //    assert( tree.Insert( "" ) == false );
    //}
    

    BKTree tree;

    vector<string> strings;

    tree.Insert( "123" );
    tree.Insert( "Hello" );
    tree.Insert( "Hallo" );

    assert( tree.Insert( "123" ) == false );
    assert( tree.Insert( "Hello" ) == false );
    assert( tree.Insert( "HaHa" ) == true );
    tree.Insert( "pwd" );
    tree.Insert( "virtual" );
    tree.Insert( "super" );
    tree.Insert( "this" );
    tree.Insert( "kkk" );

    tree.GetWords( strings, "ke", 5 );


    return 0;
}