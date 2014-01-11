/*
////////////////////////////////////////////////////////////////////////////////////////////////
/////////                                    Hastur                                    /////////
/////////                                  Has Member                                  /////////
/////////    Language : C++11                                                          /////////
/////////    Compiler : GCC(g++) version 4.9.0 20131221 (experimental)                 /////////
/////////               Clang(clang++) version 3.5 (trunk)                             /////////
/////////     Version : 1.3                                                            /////////
/////////      Status : Untested                                                       /////////
/////////        List : VEILER_HASTUR_TAG_CREATE(name,...) (macro)                     /////////
/////////               veiler::hastur<has> (template class)                           /////////
/////////                      veiler::hastur<has>::type<T>  (template class)          /////////
/////////                      veiler::hastur<has>::value<T> (template class)          /////////
/////////                      veiler::hastur<has>::func<T,Args...>  (template class)  /////////
/////////      Author : I                                                              /////////
////////////////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリはあるクラスに特定の名称のメンバが存在しているかを
    確認するためのメタ関数ライブラリです。

2. Usage -- 使い方
    このファイルをインクルードして使います。
    実装詳細はhastur/hastur.hppを参照してください。
    
    
    A. Reference -- リファレンス
        a. VEILER_HASTUR_TAG_CREATE(name)
            veiler::hasturのtemplate引数に渡すメタ関数を生成します。
            これは
             struct hoge{～}
            の形に展開されるので、typedefで束縛してhasturに渡してください。
            nameには調べたいメンバの名前を指定します。
            ex.)  typedef VEILER_HASTUR_TAG_CREATE(hoge) has_hoge_mf;
        b. veiler::hastur<has>
            第一引数hasに渡したメタ関数で指定した名称のメンバを
            持っているかどうかを表すメタ関数をメンバに持ちます。
                1. veiler::hastur<has>::type<T>  -  型
                    Tがhasで指定した名称の型を持っているかどうかを表します。
                    持っていればstd::true_type、持っていなければstd::false_typeを
                    継承します。
                2. veiler::hastur<has>::value<T> -  変数
                    Tがhasで指定した名称の変数を持っているかどうかを表します。
                    持っていればstd::true_type、持っていなければstd::false_typeを
                    継承します。
                3. veiler::hastur<has>::func<T,Args...>  -  関数
                    Tがhasで指定した名称、Args...以降で指定した型の引数で動作する
                    関数を持っているかどうかを表します。引数が存在しない関数の場合は
                    Tのみ書きます。
                    持っていればstd::true_type、持っていなければstd::false_typeを
                    継承します。
            ex.)  struct foo{typedef int hoge;};
                  veiler::hastur<has_hoge_mf>::type::value<foo>(); // is true

3. Development Environment -- 開発環境
    Vim v7.4
     +
    GCC(g++)version 4.9.0 20131221 (experimental)
    Clang(clang++)version 3.5 (trunk)

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2014/1/11   1.3 VEILER_HASTUR_TAG_CREATEによって生成されるメタ関数の
                    インターフェースを変更
    2013/12/26  1.2 オーバーロードにVeiler.Prometheusを使用するよう変更
    2013/4/7    1.1 veiler::hastur<has>::func<T>のインターフェイスを
                    veiler::hastur<has>::func<T,Args...>に変更し、
                    引数を持つ関数の検出に対応。
                    (互換性は維持されます)
    2013/4/3    1.0 作成。公開。



Copyright (C) 2014 I
  Distributed under the Veiler Source License 1.0

*/

#include"./hastur/hastur.hpp"

