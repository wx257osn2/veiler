/*
///////////////////////////////////////////////////////////////////////////////////////////
/////////                                   Var                                   /////////
/////////                           Variable Arguments                            /////////
/////////    Language : C++11                                                     /////////
/////////    Compiler : GCC(g++) version 4.7.0 20111210 (experimental)            /////////
/////////               Microsoft C/C++ Optimizing Compiler(cl)                   /////////
/////////                                                 Version 16.00.40219.01  /////////
/////////               Clang(clang++) version 3.1 (trunk)                        /////////
/////////     Version : 1.2                                                       /////////
/////////      Status : Deprecated                                                /////////
/////////        List : VEILER_VAR_FUNC(arg0,...)                                 /////////
/////////               VEILER_VAR_FUNC_USE_TAG(tag,arg0,...)                     /////////
/////////               VEILER_VAR_IMPL(T,argstype,impl)                          /////////
/////////               VEILER_VAR_IMPL_USE_TAG(T,tag,argstype,impl)              /////////
/////////               VEILER_VAR_IMPL_UNCONST(T,argstype,impl)                  /////////
/////////               VEILER_VAR_IMPL_USE_TAG_UNCONST(T,tag,argstype,impl)      /////////
/////////               VEILER_VAR_DET_IMPL_START(T,tag)                          /////////
/////////               VEILER_VAR_DET_IMPL_CTOR(...)                             /////////
/////////               VEILER_VAR_DET_IMPL_CTOR_UNCONST(...)                     /////////
/////////               VEILER_VAR_DET_IMPL_IMPL(Type)                            /////////
/////////               VEILER_VAR_DET_IMPL_IMPL_UNCONST(Type)                    /////////
/////////               VEILER_VAR_DET_IMPL_END(...)                              /////////
/////////      Author : I                                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////

0. Warning -- 警告
    このライブラリはDeprecated(非推奨)扱いです。使用する際は以下の点に注意してください。
    
    ・もともとこのライブラリはVariadic Templatesが使用できないMSVCにおいて
      型安全な可変長引数関数を実装するために作られた。
    ・2012/11/2、Visual C++ Nov 2012 Customer Technology Previewsが公開され、
      正式リリースではないとはいえMSVCにおいてもVariadic Templatesが使用できるようになった。
    ・これで主要なC++のコンパイラの内、可変長引数マクロに対応しつつ
      Variadic Templatesに対応しないコンパイラはなくなったと思われる。
      また、当然次期MSVC(恐らくmsvc12.0)においてはVariadic Templatesは
      リリース版のコンパイラに実装されるはずである。
    ・以上より、Veiler.Varを用いるよりVariadic Templatesを用いるべきだと考える。
    ・当然、この世のあらゆる開発現場において、リリース品でもないプレビュー版のコンパイラを
      使用できるとは限らないのは百も承知である。Veiler.Var自体は何も問題なく使用出来るので
      今後暫くご愛願いただければ幸いである。

1. What's this library? -- このライブラリについて
    このライブラリは型安全な可変長引数関数をVariadic Templatesに対応してない
    <del>ダメダメな</del>MSVCで使いたい、ということで作った
    擬似可変長引数関数定義用マクロライブラリです。

2. Usage -- 使い方
    このファイルをインクルードして使います。
    実装詳細はvar/var.hppを参照してください。
    
    
    A. Principle -- 原理
        このライブラリでは可変長引数関数を擬似的に再現するための方法として、
        可変長引数マクロとoperator,()を用いる若干黒魔術じみた手法をとっています。
        例えば、define マクロによって擬似可変長引数関数func()を定義したとします。
        func(a,b,c,d,e)と書いた時、これをdefineマクロによって
        (func_helper(a),b,c,d,e)に展開します。このとき、func_helperは
        operator,()によって特定の型の値を受け取り自身を返すクラスにします。
        すると、最終的にfunc(a,b,c,d,e)はfunc_helper型の値をとります。
        ここでさらに、func_helper型を特定の型から継承させておくか、
        オーバーロード演算子を定義しておけばfunc_helperから特定の型への暗黙変換が可能です。
        これにより、func()があたかも可変長引数を取る関数のように振る舞います。
    
    B. Reference -- リファレンス
        a. 簡易版
            例として、今回は独自の型T、Dを用いることにします。Tの実装は以下の通りです。
            
            struct T{int x;int y;};
            struct D{double x;double y;};
            
            0. 仕様決定
                生成する可変長引数関数について、
                ・戻り値の型はTとする
                ・第1引数の型はTとする
                またTを用いた複数の可変長引数関数を生成する場合、混同を避けるために
                タグを付けることができます。これを用いる場合について、
                ・タグはtagという名の型とする
            1. 実装定義
                VEILER_VAR_IMPL(T,argstype,impl)
                VEILER_VAR_IMPL_USE_TAG(T,tag,argstype,impl)
                VEILER_VAR_IMPL_UNCONST(T,argstype,impl)
                VEILER_VAR_IMPL_USE_TAG_UNCONST(T,tag,argstype,impl)
                これらのマクロを用いて、関数の実装を定義します。
                マクロの第1引数にはTを指定してください。
                タグを用いる場合、「_USE_TAG」が付いたマクロを用い、第2引数にtagを指定してください。
                第3引数には、生成する可変長引数関数の第2引数以降に来る可能性のある値の型を、
                そして第4引数にはその型における処理を書いてください。
                処理において、その時点で処理が済んでいる部分の結果をlhs(T)、処理対象をrhs(argstype)として扱います。
                また、可変長引数で受け取る値に変更を加える場合、「_UNCONST」の付いたマクロを用いてください。
                
                ex.)
                VEILER_VAR_IMPL(T,T,{lhs.x+=rhs.x;lhs.y+=rhs.y;})
                VEILER_VAR_IMPL(T,D,{lhs.x+=rhs.x;lhs.y+=rhs.y;})
            2. 関数宣言/使用
                VEILER_VAR_FUNC(arg0,...)
                VEILER_VAR_FUNC_USE_TAG(tag,arg0,...)
                これらのマクロを用いて関数を使用することができます。
                arg0にはT型のオブジェクトを指定してください。
                タグを用いる場合、「VEILER_VAR_FUNC_USE_TAG」を用いて第1引数にtagを指定してください。
                「...」の部分には 1. の項で実装を書いた型(この場合TかD)のオブジェクトを好きなだけ書いてください。
                また、このマクロ自体をさらにdefineすることで可変長引数関数に名前をつけることができます。
                
                ex.)
                #define add(a,...) VEILER_VAR_FUNC(a,__VA_ARGS__)
                
                T a(3,2),b(4,5);
                D c(6.0,8.9);
                T d = add(a,b,c);
                //d.x == 13 , d.y == 15
        b. 詳細版
            例として、今回は独自の型T、Dを用いることにします。Tの実装は以下の通りです。
            
            struct T{int x;int y;};
            struct D{double x;double y;};
            
            0. 仕様決定
                生成する可変長引数関数について、
                ・第1引数の型はTとする
                ・タグはTagという名の型とする
            1. 実装定義
                VEILER_VAR_DET_IMPL_START(T,Tag)
                を書きます。これはクラスの序文です。これ以降はクラスの中身を書きます。
                ここで、typeはTの、tagはTagのtypedefとなります。クラスを書く上で用いる場合は
                これらを使うことができます。
                その後、
                ・変数定義
                ・コンストラクタの実装
                ・関数の実装
                等を行うことができます。
                コンストラクタの実装は、
                VEILER_VAR_DET_IMPL_CTOR()
                VEILER_VAR_DET_IMPL_CTOR_UNCONST()
                に続けて、
                ・初期化子リストがある場合「VEILER_VAR_DET_IMPL_CTOR(),c(0),d(30){}」などのように続けて記入
                ・初期化子リストを用いない場合はそのまま続けて{}を、そしてその中に初期化内容を記入
                します。
                引数に取る値(=可変長引数関数の第1引数)に変更を加える場合は「_UNCONST」を用いてください。
                また、関数の実装は、
                VEILER_VAR_DET_IMPL_IMPL(Type)
                VEILER_VAR_DET_IMPL_IMPL_UNCONST(Type)
                を用い、これに続いて{}を、そしてその中に初期化内容を記入します。
                Typeには引数に取る値の型を指定します。また、引数はargとして関数内で用いることができます。
                戻り値は必ず*thisにしてください。
                こちらも同様に引数に取る値(=可変長引数関数の第2引数以降)に変更を加える場合は「_UNCONST」を
                用いてください。
                クラスの実装が終了したら、最後に
                VEILER_VAR_DET_IMPL_END
                でクラスを閉じます。
                
                ex.)
                struct hoge{};
                VEILER_VAR_DET_IMPL_START(T,hoge)
                  VEILER_VAR_DET_IMPL_CTOR(),count(1){}
                  VEILER_VAR_DET_IMPL_IMPL(T){this->x+=arg.x;this->y+=arg.y;++count;return *this;}
                  VEILER_VAR_DET_IMPL_IMPL(D){this->x+=arg.x;this->y+=arg.y;++count;return *this;}
                  VEILER_VAR_DET_IMPL_IMPL(int){this->x+=arg;this->y+=arg;return *this;}
                  operator D(){return D(this->x/count,this->y/count);}
                private:
                  unsigned int count;
                VEILER_VAR_DET_IMPL_END
            2. 関数宣言/使用
                VEILER_VAR_FUNC_USE_TAG(Tag,arg0,...)
                を用いて関数を使用することができます。
                Tagにはタグを、arg0にはT型のオブジェクトを指定してください。
                「...」の部分には 1. の項で関数の実装を書いた型(この場合T、D、int)のオブジェクトを好きなだけ書いてください。
                また、このマクロ自体をさらにdefineすることで可変長引数関数に名前をつけることができます。
                
                ex.)
                #define avg(a,...) VEILER_VAR_FUNC_USE_TAG(hoge,a,__VA_ARGS__)
                
                T a(3,2),b(4,5);
                D c(6.0,8.9);
                D d = avg(a,b,c);
                T t = avg(a,b,c);  //名前詐欺
                //d.x == 4  , d.y == 5
                //t.x == 13 . t.y == 15

3. Development Environment -- 開発環境
    Terapad 1.0.8 + GCC(g++)version 4.7.0 20111210 (experimental)
                    Clang(clang++)version 3.1 (trunk)
    Microsoft Visual Studio 2010 Professional(10.0.30319.1)
    Ideone.com

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    当ライブラリを作成するにあたり、Ryosuke氏には根幹のアイデア
    (可変長引数マクロとoperator,()を用いることで
    可変長引数関数のような見た目で型安全に引数を処理できる)を
    提案していただきました。ここで感謝を申し上げます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2012/9/23  1.2 一部マクロを変更。説明を変更。
    
    2012/4/18  1.1 名前をVarに変更。
    
    2012/4/8   1.0 名前空間/マクロ名をveiler/VEILERに変更。DET_IMPL系のマクロを追加。公開。
    
    2011/10/15 0.9 defineマクロの擬似ループによる大量オーバーロードを避ける方法として
                   可変長引数マクロをoperator,()と共に用いる手法を提示され、
                   Ideone.comにてこれを用いたマクロを作成する。   : http://ideone.com/4JjZo



Copyright (C) 2012 I
  Distributed under the Veiler Source License 1.0

*/

#include"./var/var.hpp"

