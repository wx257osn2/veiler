/*
///////////////////////////////////////////////////////////////////////////////////////////
/////////                                 Istasha                                 /////////
/////////                           Auto Instantiation                            /////////
/////////    Language : C++11                                                     /////////
/////////    Compiler : GCC(g++) version 4.8.0 20120923 (experimental)            /////////
/////////               Clang(clang++) version 3.2 (trunk)                        /////////
/////////     Version : 1.0                                                       /////////
/////////      Status : Untested                                                  /////////
/////////        List : VEILER_ISTASHA_CREATE(class_name,derived_name) (macro)    /////////
/////////               VEILER_ISTASHA_DEFAULT_CONSTRUCTOR(class_name,            /////////
/////////                                                  derived_name) (macro)  /////////
/////////               veiler::istasha<T> (template class)                       /////////
/////////      Author : I                                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリは宣言と同時に自動的に実体化及び処理が行われるクラスを作るための
    ライブラリです。

2. Usage -- 使い方
    このファイルをインクルードして使います。
    実装詳細はistasha/istasha.hppを参照してください。
    
    
    A. Principle -- 原理
        CRTPを用いてistashaに型を与え、istasha側でその型の静的オブジェクトを生成します。
        コンストラクタを書き換えれば好きな処理を加えることも可能です。
    
    B. Reference -- リファレンス
        a. veiler::istasha<T>
            自動で実体化したいクラスを宣言する時に、veiler::istashaを継承します。
            ex.)  struct hoge : public veiler::istasha<hoge>{ここに実装};
        b. VEILER_ISTASHA_CREATE(class_name,derived_name)
            CRTPを用いて宣言と同時に自動で実体化するクラスを作るための
            補助クラスを好きな名前で定義できます。
            class_nameにはクラスの名前を、derived_nameにはクラスの内部で用いる
            継承先のインスタンスの名前を渡してください。
            VEILER_ISTASHA_CREATEで定義したクラスはデフォルトコンストラクタを
            自分で定義するか、後述のVEILER_ISTASHA_DEFAULT_CONSTRUCTORを用いて
            コンストラクタを生成してください。
            ex.)  VEILER_ISTASHA_CREATE(fuga,Derived)
                  template<typename T>fuga<T>::fuga(){Derived;}//Derivedを使った処理を書きます。
                  struct hoge : public fuga<hoge>{ここに実装};
        c. VEILER_ISTASHA_DEFAULT_CONSTRUCTOR(class_name,derived_name)
            VEILER_ISTASHA_CREATEで定義したクラスにveiler::istashaと同じコンストラクタを定義します。
            つまり挙動をveiler::istashaと同じ物にします。
            ex.)  VEILER_ISTASHA_CREATE(fuga,Derived)
                  VEILER_ISTASHA_DEFAULT_CONSTRUCTOR(fuga,Derived)
                  struct hoge : public fuga<hoge>{ここに実装}; //fuga<hoge> は veiler::istasha<hoge>と同じ

3. Development Environment -- 開発環境
    Vim 7.3 + GCC(g++)version 4.8.0 20120923 (experimental)
              Clang(clang++)version 3.2 (trunk)
    Ideone.com

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2012/12/13  1.0 作成。公開。



Copyright (C) 2012 I
  Distributed under the Veiler Source License 1.0

*/

#include"./istasha/istasha.hpp"

