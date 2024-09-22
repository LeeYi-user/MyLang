# MyLang

本程式修改自[老師的專案1](https://gitlab.com/cccnqu111/co/-/tree/master/06)和[老師的專案2](https://github.com/ccc111b/cpu2os/tree/master/02-%E8%BB%9F%E9%AB%94/02-%E7%B7%A8%E8%AD%AF%E5%99%A8/01-diy/05-compiler-hack)。

## 基本介紹

我根據上課內容，把上學期的 `HackCPU` 彙編器，與這學期的 `EBNF` 編譯器結合起來。  
其程式碼主要都取自老師的專案，除了把他們結合起來外，我僅僅修改了以下部分:

1. 把原本的終端機列印 (printf) 改成檔案寫入 (fprintf)。
2. 更改執行檔的目標檔案參數格式 (一律要加副檔名)。
3. 修復從中間碼轉換到 HackCPU 組合語言的錯誤。
4. 讓 Makefile 能同時在 Windows/Linux 上執行。

以下則是該專案的運作流程，基本上可以分為兩個部份:

* compiling

    1. 根據 `argv` 參數來讀取指定的高階語言檔案 (*.my)。
    2. 用 `lexer.c` 的 `lex()` 函數把高階語言 (MyLang) 轉換成 `token`。
    3. 用 `compiler.c` 的 `parse()` 函數把 token 轉換成中間碼。
    4. 用 `ir.c` 的 `irToHack()` 函數把中間碼轉換成組合語言。

* assembling & running

    1. 用 `asm.cpp` 把組合語言轉換成機器碼。
    2. 用 `vm.c` 把機器碼丟入虛擬機中執行。

而在 compiling 的第三步驟中，我們使用到了 EBNF 語法來將其轉換。該語法如下所示:

```
PROG = STMTS
BLOCK = { STMTS }
STMTS = STMT*
STMT = WHILE | BLOCK | ASSIGN
WHILE = while (E) STMT
ASSIGN = id '=' E;
E = F (op E)*
F = (E) | Number | Id
```

## 使用方式

以執行 `sum.my` 為例，只要在終端機中依序輸入以下指令即可:

```
make
./compiler test/sum.my
./asm test/sum.asm
./vm test/sum.bin
```

結束後可使用 `make clean` 來清除剛生成的檔案。

## 授權聲明

[LICENSE](https://github.com/LeeYi-user/MyLang/blob/main/LICENSE)
