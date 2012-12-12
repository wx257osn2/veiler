/*
///////////////////////////////////////////////////////////////////////////////////////////
/////////                                  Refil                                  /////////
/////////                             Reference Holder                            /////////
/////////    Language : C++11                                                     /////////
/////////    Compiler : GCC(g++) version 4.8.0 20120923 (experimental)            /////////
/////////               Clang(clang++) version 3.2 (trunk)                        /////////
/////////     Version : 1.0                                                       /////////
/////////      Status : Untested                                                  /////////
/////////        List : veiler::refil<T>(template class)                          /////////
/////////      Author : I                                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリは型の参照を保持するクラスveiler::refilを提供します。

2. Usage -- 使い方
    このファイルをインクルードして使います。
    実装詳細はrefil/refil.hppを参照してください。
    
    
    
    A. Usage -- 使い方
        veiler::refil<T>のインスタンスはT型のインスタンスの参照で初期化します。
        veiler::refil<T>のインスタンスはT&/const T&に暗黙変換でき、これで初期化時に渡した
        Tのインスタンスにアクセスでき、またこれ自体はコピーすることが可能なので
        参照をコピーしたい時に用います。
        簡単な話、ぬるぽのない、デリファレンスしなくてもいいポインタみたいなものです。
        
        ここで重要な事なのですが、ポインタ同様元のオブジェクトの寿命に注意して
        取り扱わないと不正なメモリアドレスを踏んでSEGVします。
        それなりに気をつけて使ってください。

3. Development Environment -- 開発環境
    Vim v7.3 + GCC(g++)version 4.8.0 20120923 (experimental)
               Clang(clang++)version 3.2 (trunk)

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2012/9/23  1.0 公開。



Copyright (C) 2012 I
  Distributed under the Veiler Source License 1.0.

*/

#include"./refil/refil.hpp"

