/*
///////////////////////////////////////////////////////////////////////////////////////////
/////////                                  Ullr                                   /////////
/////////                                Utility                                  /////////
/////////    Language : C++11                                                     /////////
/////////    Compiler : GCC(g++) version 4.8.0 20120923 (experimental)            /////////
/////////               Microsoft C/C++ Optimizing Compiler(cl)                   /////////
/////////                                                 Version 16.00.40219.01  /////////
/////////               Clang(clang++) version 3.2 (trunk)                        /////////
/////////     Version : 1.1                                                       /////////
/////////      Status : Untested                                                  /////////
/////////        List : <Ullr/Eridian>                                            /////////
/////////                      veiler::ullr::endian (enum class)                  /////////
/////////                             veiler::ullr::endian::big                   /////////
/////////                             veiler::ullr::endian::little                /////////
/////////                      veiler::ullr::eridian() (Functor)                  /////////
/////////               <Ullr/Areion>                                             /////////
/////////                      veiler::ullr::areion<T>(...) (template Function)   /////////
/////////      Author : I                                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリ群は小さなユーティリティライブラリです。
    <del>いらない</del>

2. Usage -- 使い方
    このファイルをインクルードすると、Ullrすべての機能が使えるようになります。
    個別に使いたい機能がある場合は、それぞれ対応するファイルをインクルードしてください。
    
    
    
    A. Fall  -  Using Namespace
        veiler::ullr以下をveiler以下として扱います。
        
        使うにはプリプロセッサトークンVEILER_ULLR_NAMESPACE_FALLを定義した後
        veiler/ullr/fall.hppをインクルードします。
    
    B. Eridian  -  Endian
        エンディアンを判定します。
        veiler::ullr::eridian()がveiler::ullr::endian型の値(big/little)を返します。
        
        使うにはveiler/ullr/eridian.hppをインクルードします。
    
    C. Areion  -  Make Array
        std::arrayを生成します。
        veiler::ullr::areion<int>(1,2,3)のように、要素数を指定しません(引数の数で
        自動で要素数が決定します。この場合はstd::array<int,3>{1,2,3}が返ります)。
        
        使うにはveiler/ullr/areion.hppをインクルードします。

3. Development Environment -- 開発環境
    Vim v7.3 / TeraPad v1.08
     +
    GCC(g++)version 4.8.0 20120429 (experimental)
    Clang(clang++)version 3.2
    Microsoft Visual Studio 2010 Professional(10.0.30319.1)

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2012/12/12  1.1 Areion  : 追加。
                    Fall    : 追加。
    
    2012/10/2   1.0 Eridian : ICL(I C++ Library : 以前作った非公開のライブラリ)
                              からクロール。名称を変更。微修正。公開。



Copyright (C) 2012 I
  Distributed under the Veiler Source License 1.0.

*/

#include"./ullr/fall.hpp"
#include"./ullr/eridian.hpp"
#include"./ullr/areion.hpp"
