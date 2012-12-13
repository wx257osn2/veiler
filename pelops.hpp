/*
///////////////////////////////////////////////////////////////////////////////////////////
/////////                                 Pelops                                  /////////
/////////                              PreProcessor                               /////////
/////////    Language : C++11                                                     /////////
/////////    Compiler : GCC(g++) version 4.8.0 20120429 (experimental)            /////////
/////////     Version : 1.3                                                       /////////
/////////      Status : Untested                                                  /////////
/////////        List : <Pelops/Dibella>                                          /////////
/////////                      VEILER_PELOPS_DIBELLA(...) (macro)                 /////////
/////////               <Pelops/Lupe>                                             /////////
/////////                      VEILER_PELOPS_LUPE(macro,args) (macro)             /////////
/////////                      <Pelops/Lupe/LupeGem>                              /////////
/////////                             VEILER_PELOPS_LUPEGEM_USING(args) (macro)   /////////
/////////               <Pelops/Ullr>                                             /////////
/////////                      VEILER_PELOPS_ULLR_COMMA (macro)                   /////////
/////////                      VEILER_PELOPS_ULLR_CAT(a,b) (macro)                /////////
/////////      Author : I                                                         /////////
///////////////////////////////////////////////////////////////////////////////////////////

1. What's this library? -- このライブラリについて
    このライブラリ群は純プリプロセッサライブラリです。
    <del>ゴミコード片</del>

2. Usage -- 使い方
    このファイルをインクルードすると、Pelopsのすべての機能が使えるようになります。
    個別に使いたい機能がある場合は、それぞれ対応するファイルをインクルードしてください。
    
    
    
    A. Dibella  -  Delay
        内部に含むシンボルのマクロ展開を遅延させます。
        また、std::pair<int,int>のようなコンマを含むシンボルを
        VEILER_PELOPS_DIBELLA(std::pair<int,int>)のようにすることで一つのシンボルとして
        扱うことができます。
        
        使うにはveiler/pelops/dibella.hppをインクルードします。
        予めVEILER_PELOPS_DIBELLA_LIMIT_1000をdefineしておくと遅延を増加できます。
    
    B. Lupe  -  Loop
        無限引数マクロの実行部分です。可変長引数マクロがあるのでほとんど無用の長物ですが
        書式などの都合であったほうが便利なこともあると思うので用意しました。
        
        使うにはveiler/pelops/lupe.hppをインクルードします。
        
            a. LupeGem/Using
                Lupeを用いた無限引数マクロの一例です。using宣言を行います。
                
                使うにはveiler/pelops/lupegem/using.hppをインクルードします。
    
    C. Ullr  -  Utility
        プリプロセッサを使う時に便利なマクロが入っています。
        
        使うにはveiler/pelops/ullr.hppをインクルードします。
    
    
    
    使い方はLupe,LupeGem/Usingあたりを参照すればおおよそつかめると思います。

3. Development Environment -- 開発環境
    Notepad++ v5.8.7-6 / Vim v7.3 / TeraPad v1.08
     +
    GCC(g++)version 4.8.0 20120429 (experimental)
    Clang(clang++)version 3.2

4. License and Exemption from responsibility -- ライセンスと免責
    当ライブラリはVeiler Source Licenseにて公開されます。
    また、(c)(R)等を明記していませんが社名、製品名などは
    各社の登録商標または商標です。

5. 更新情報
    2012/9/23   1.3 typeの名称を変更(type -> Dibella)
                    Lupeを追加
                    Ullrを追加
                    説明文の変更
    
    2012/6/14   1.2 typeの実装が(ry
                    C++わから(ry
    
    2012/6/10   1.1 typeの実装がわけわからんことになっていたので修正。
                    C++わからん。
    
    2012/5/15   1.0 Veiler.Pelops作成。typeを追加。



Copyright (C) 2012 I
  Distributed under the Veiler Source License 1.0.

*/

#include "./pelops/dibella.hpp"
#include "./pelops/lupe.hpp"
#include "./pelops/ullr.hpp"

