/*
/////////////////////////////////////////////////////////////////////////////////
/////////                             Fool                              /////////
/////////                          Functional                           /////////
/////////    Language : C++11                                           /////////
/////////    Compiler : GCC(g++) version 4.9.0 20140407 (experimental)  /////////
/////////               Clang(clang++) version 3.5.0 (trunk)            /////////
/////////     Version : 1.0                                             /////////
/////////      Status : Untested                                        /////////
/////////        List : <Fool/Invoke>                                   /////////
/////////                      veiler::invoke(...) (function)           /////////
/////////      Author : I                                               /////////
/////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリは関数オブジェクトに関するユーティリティライブラリです。

2. Usage -- 使い方
    このファイルをインクルードすると、Foolのすべての機能が使えるようになります。
    個別に使いたい機能がある場合は、それぞれ対応するファイルをインクルードしてください。
    
    
    
    A. Invoke
        C++標準規格の[func.require]で定義されているINVOKE仮想操作の実装です。
    
    
    

3. Development Environment -- 開発環境
    Vim v7.4
     +
    GCC(g++)version 4.9.0 20140407 (experimental)
    Clang(clang++)version 3.5.0 (trunk)

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはBoost Software License 1.0にて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
   2014/4/7   1.0 Invoke追加。ようやくドキュメント書いた。



Copyright (C) 2012-2017 I
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include "./fool/invoke.hpp"

