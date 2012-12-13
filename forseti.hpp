/*
///////////////////////////////////////////////////////////////////////////////////////////
/////////                                Forseti                                  /////////
/////////                                fstream                                  /////////
/////////    Language : C++11                                                     /////////
/////////    Compiler : GCC(g++) version 4.8.0 20120923 (experimental)            /////////
/////////               Microsoft C/C++ Optimizing Compiler(cl)                   /////////
/////////                                                 Version 16.00.40219.01  /////////
/////////               Clang(clang++) version 3.2 (trunk)                        /////////
/////////     Version : 1.0                                                       /////////
/////////      Status : Untested                                                  /////////
/////////        List : <Forseti/Olympus>                                         /////////
/////////                      veiler::forseti::r (std::ios::openmode)            /////////
/////////                      veiler::forseti::w (std::ios::openmode)            /////////
/////////                      veiler::forseti::a (std::ios::openmode)            /////////
/////////                      veiler::forseti::rb (std::ios::openmode)           /////////
/////////                      veiler::forseti::wb (std::ios::openmode)           /////////
/////////                      veiler::forseti::ab (std::ios::openmode)           /////////
/////////                      veiler::forseti::rp (std::ios::openmode)           /////////
/////////                      veiler::forseti::wp (std::ios::openmode)           /////////
/////////                      veiler::forseti::ap (std::ios::openmode)           /////////
/////////                      veiler::forseti::rpb (std::ios::openmode)          /////////
/////////                      veiler::forseti::wpb (std::ios::openmode)          /////////
/////////                      veiler::forseti::apb (std::ios::openmode)          /////////
/////////                      veiler::forseti::rbp (std::ios::openmode)          /////////
/////////                      veiler::forseti::wbp (std::ios::openmode)          /////////
/////////                      veiler::forseti::abp (std::ios::openmode)          /////////
/////////      Author : I                                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリ群はfstreamの拡張ライブラリです。
    <del>微妙</del>

2. Usage -- 使い方
    このファイルをインクルードすると、Forsetiすべての機能が使えるようになります。
    個別に使いたい機能がある場合は、それぞれ対応するファイルをインクルードしてください。
    
    
    
    A. Olympus  -  Openmode
        veiler::forseti::*はC言語のfopenの第二引数に渡す文字列に対応します。
        例えば、rはin、abならout|app|binary、wpbはw+b、即ちin|out|trunc|binaryなどです。
        
        使うにはveiler/ullr/forseti/olympus.hppをインクルードします。

3. Development Environment -- 開発環境
    Terapad 1.0.8 + GCC(g++)version 4.8.0 20120923 (experimental)
                    Clang(clang++)version 3.2 (trunk)
    Microsoft Visual Studio 2010 Professional(10.0.30319.1)

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2012/10/2   1.0 Olympus : ICL(I C++ Library : 以前作った非公開のライブラリ)
                              からクロール。名称を変更。微修正。公開。



Copyright (C) 2012 I
  Distributed under the Veiler Source License 1.0.

*/

#include"./forseti/olympus.hpp"
