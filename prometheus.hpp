/*
////////////////////////////////////////////////////////////////////////////////////
/////////                            Prometheus                            /////////
/////////                         Overload Priority                        /////////
/////////    Language : C++11                                              /////////
/////////    Compiler : GCC(g++) version 4.9.0 20131221 (experimental)     /////////
/////////               Clang(clang++) version 3.5 (trunk)                 /////////
/////////     Version : 1.0                                                /////////
/////////      Status : Untested                                           /////////
/////////        List : veiler::prometheus<N> (template alias)             /////////
/////////               veiler::prometheus_fire<N>() (template function)   /////////
/////////      Author : I                                                  /////////
////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリはSFINAEだけでは解決しきれない曖昧なオーバーロードを解決するべく
    オーバーロードに優先順位を与えるためのクラスを提供します．
    元ネタはRiSK(sscrisk)さんの
    overload_priority.hpp(https://gist.github.com/sscrisk/1222173)です．

2. Usage -- 使い方
    このファイルをインクルードして使います．
    実装詳細はprometheus/prometheus.hppを参照してください．
    
    
    A. Reference -- リファレンス
        a. veiler::prometheus<N>
            関数の宣言時に引数にこれを指定することでオーバーロードに優先順位を
            与えることが出来ます．複数の同名関数の，同じ箇所の引数に対し
            これを指定して下さい．
            Nには0以上の整数を指定します．大きいほど優先順位は高くなります．
            ex.)  int    f(int,    veiler::prometheus<1>);
                  double f(double, veiler::prometheus<0>);
        b. veiler::prometheus_fire<N>()
            関数を呼び出すときに引数にこれを指定することで優先順位を考慮した
            オーバーロード解決を行うことが出来ます．
            Nには任意の0以上の整数を指定します．N以下で最も大きい値の
            veiler::prometheus<N>を持つ関数から優先的にオーバーロードの候補に
            なります．
            ex.)  f(t, veiler::prometheus_fire<1>());

3. Development Environment -- 開発環境
    Vim v7.4
     +
    GCC(g++)version 4.9.0 20131221 (experimental)
    Clang(clang++)version 3.5 (trunk)

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはBoost Software License 1.0にて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2013/12/26    1.0 作成。公開。



Copyright (C) 2012-2017 I
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

*/

#include "./prometheus/prometheus.hpp"

