# 🖨 ft_printf — C 標準ライブラリの `printf` を可変長引数から再実装

![language](https://img.shields.io/badge/language-C-blue.svg)
![norm](https://img.shields.io/badge/42-norminette-success.svg)
![flags](https://img.shields.io/badge/build-Wall%20Wextra%20Werror-brightgreen.svg)
![platform](https://img.shields.io/badge/platform-macOS%20(arm64)-lightgrey.svg)

> **TL;DR (EN):** A from-scratch reimplementation of C's `printf` as a [42](https://42tokyo.jp/)
> project. It parses the format string in a single left-to-right pass and writes each
> conversion straight to fd 1 with `write(2)` — no output buffer, no `malloc` for the
> result, no libc `printf`. Supports `%c %s %p %d %i %u %x %X %o %%` plus the bonus
> flags `-0#+ `, minimum field width, precision and `*`. 15 source files / 1,165 lines,
> driven by a single 15-field state struct to stay inside 42's Norm (25 lines and
> 4 parameters per function). Checked against libc: **33/33** mandatory cases match
> exactly, and the bonus sweep matches on **7,572 of 8,640** combinations — the
> remaining gaps are listed below rather than hidden.

42 のカリキュラム課題 **ft_printf** です。可変長引数（`<stdarg.h>`）と `write(2)` だけを使って
`printf` の主要機能を作り直し、静的ライブラリ `libftprintf.a` として提供します。
libc の `printf` は呼ばず、出力バッファも持たず、書いたバイト数を数えて戻り値にします。

**対応する変換指定子**: `%c` `%s`（`NULL` は `(null)`） `%p`（`NULL` は `0x0`） `%d` `%i` `%u`
`%x` `%X` `%o` `%%`。
**ボーナス**: 最小フィールド幅、精度（`.`）、フラグ `- 0 # +` と空白、`*` による幅・精度の可変指定
（負の幅を渡すと libc と同じく左寄せに切り替わります）。フラグの一致状況は
[既知の不一致](#既知の不一致)を参照してください。

---

## ビルド

```bash
make            # libftprintf.a を生成（同梱 libft も一緒にビルドされる）
make bonus      # re のエイリアス。ボーナスは同じ .a に含まれる
make norm       # norminette を srcs / include / libft に対して実行
make clean      # オブジェクトファイルを削除
make fclean     # .a も含めて削除
make re         # fclean + all
```

`CFLAGS = -Wall -Werror -Wextra -Iinclude`、警告ゼロでコンパイルされます。

## リンクして使う

```c
#include "ft_printf.h"

int	main(void)
{
	int	len;

	ft_printf("%s / %c / %d / %i\n", "42Tokyo", 'K', 42, -42);
	ft_printf("%u | %x | %X | %o | %p\n", 4294967295u, 48879, 48879, 511,
		(void *)0x7ffee1234567);
	ft_printf("[%10s][%-10s][%.3s]\n", "right", "left", "truncate");
	ft_printf("[%8d][%-8d][%.5d]\n", 42, 42, 42);
	len = ft_printf("100%% done\n");
	ft_printf("last call returned %d\n", len);
	return (0);
}
```

```console
$ gcc -Wall -Wextra -Werror -Iinclude main.c libftprintf.a -o demo
$ ./demo
42Tokyo / K / 42 / -42
4294967295 | beef | BEEF | 777 | 0x7ffee1234567
[     right][left      ][tru]
[      42][42      ][00042]
100% done
last call returned 10
```

---

## 実装のポイント

### 状態を 1 つの構造体に集約する

42 の Norm は「1 関数 25 行以内・引数 4 個以内・ローカル変数 5 個以内」を課します。
書式解析の途中状態（残りの書式文字列・フラグ・幅・精度・桁数・出力済み文字数）をそのまま
引数で回すと即座に上限を超えるため、15 フィールドの状態構造体 `t_tab`（`include/ft_printf.h`）
にまとめ、全ての関数がこれ 1 つを受け取る形にしました。`va_list` も構造体のメンバなので、
`va_arg` を呼ぶ処理を自由に関数分割できます。

初期化は 2 段に分けています。`ft_initialize` は呼び出し全体で 1 回だけ行うもの
（`len` のゼロ初期化、フラグ文字列 `"#0- +"` の設定）、`ft_initialize_again` は変換指定子
1 個ごとに戻すもの（幅・精度・フラグ・桁数）で、リセットの範囲を関数名で表現しています。

### フラグをビットマスクではなく文字列インデックスで持つ

同梱 libft の `ft_strchr` は標準と違い **見つかった位置の `int` インデックス** を返すよう
改変してあります。フラグ集合 `"#0- +"` に対して `ft_strchr` を呼べば戻り値がそのまま
フラグ ID になり、`if` の連鎖を書かずに済みます。複数フラグが並んだ場合は
`ft_process_flag_priority`（`srcs/ft_printf_flag.c`）が優先順位を解決し、`-` を最優先で残します。

### 基数変換の再帰を「桁数計算」と「出力」で共有する

数値出力の中核は 4 行の再帰関数 1 つです。

```c
int	ft_putnbr_write_len(unsigned long nb, int base, char mode, char *putlist)
{
	int	len;

	len = 0;
	if (nb >= (unsigned long)base)
		len = ft_putnbr_write_len(nb / base, base, mode, putlist);
	if (mode == 'w')
		write(1, &putlist[nb % base], 1);
	return (len + 1);
}
```

- 上位桁から順に出力するため、復帰時に 1 桁書く形の再帰にしています。深さ・計算量とも
  O(log_base n)（32bit 値なら 10 進で最大 10 段、16 進で 8 段）。
- `mode` が `'l'` なら書かずに桁数だけ返し、`'w'` なら実際に書きます。パディング量を決めるには
  先に桁数が必要ですが、同じ再帰を使い回すことで桁数計算用のコードを別に持たずに済みます。
- 数字テーブル `putlist` を引数にしたので、`%x`（`0123456789abcdef`）と
  `%X`（`0123456789ABCDEF`）が同一関数を共有します。

### パディングは算術で決め、左寄せ／右寄せは経路ごと分ける

文字列を組み立ててから幅を合わせる作り方はしていません。`ft_set_diff` が精度による 0 埋め幅
`max(0, 精度 - 桁数)` を求め、`ft_diff_one_or_more` が
`field_width - (桁数 + 0埋め幅 + 符号やプレフィックスの幅)` を `while` の条件にして空白または
`0` を直接 `write` します。差が負なら条件が最初から偽になるのでクランプ処理が要りません。

`-` フラグの有無ではパディングを先に出すか後に出すかで処理順そのものが変わるため、
`ft_flag_hyphen_*` / `ft_flag_nonhyphen_*` と関数を分け、1 つの関数に順序違いを `if` で
混ぜないようにしました。`%s` の `NULL`（`srcs/ft_print_s_null.c`）と `%p` の `NULL`
（`srcs/ft_print_unsigned_long_utils.c` の `handle_null_pointer_*` 5 関数）も同じ理由で
専用経路に切り出しています。書式に `%` が 1 つも無い場合は 1 文字ずつのループを回さず、
`ft_putstr` + `ft_strlen` で一括出力します（`ft_printf_main`）。

### 同梱 libft への改変

`libft/`（46 ファイル / 1,377 行）は自作 libft の同梱ですが、ft_printf の都合で 3 点を変えています。

- `ft_strchr` は `char *` ではなく `int` のインデックスを返す（フラグ ID として使うため）
- `ft_atoi` は `int *len` を第 2 引数に取り、読み取った桁数だけ呼び出し側のカーソルを進める
  （パーサが「数値を読み終わった位置」を必要とするため）
- `ft_putstr` / `ft_putnstr` / `ft_strnchr` を追加

そのため `libftprintf.a` は libft のオブジェクトも含んでおり、これ 1 つでリンクできます。

---

## 検証

libc の `snprintf` と **出力文字列・戻り値の両方** を突き合わせる比較プログラムを書いて確認しました
（テストコードはリポジトリに含めていません）。

| 項目 | 結果 |
| --- | --- |
| 必須の変換指定子 | 33 ケース中 33 件が libc と完全一致。`INT_MAX` / `INT_MIN` / `0` / `-1` / `UINT_MAX` / `NULL` 文字列 / `NULL` ポインタ / 空文字列 / 変換を含まない書式を含む |
| ボーナスの総当たり（フラグ 10 種 × 幅 4 種 × 精度 4 種 × 変換 6 種 × 値 9 種） | 8,640 ケース中 7,572 件（87.6%）が一致 |
| norminette | `srcs/*.c` 15 個、`include/*.h` 2 個、同梱 libft の 46 個すべてが `OK!` |
| メモリリーク | `leaks -atExit` で 0 leaks / 0 bytes（`ft_printf` を 8,673 回呼ぶバイナリで測定） |
| コンパイル警告 | `-Wall -Wextra -Werror` で 0 件 |

検証環境は macOS（Darwin arm64 / Apple clang 21）です。Linux では確認していません。

### 既知の不一致

ボーナス部分には未修正の不一致が残っています。上記 8,640 ケースのうち一致しなかった 1,068 件は
以下の 4 系統に集中します。

| 症状 | 例 | ft_printf | libc |
| --- | --- | --- | --- |
| `0` フラグが効かず空白埋めになる | `%05d`, 42 | `   42` | `00042` |
| `+` と空白フラグが入れ替わる | `%+d` / `% d`, 42 | ` 42` / `+42` | `+42` / ` 42` |
| `#` を `-` と併用するとプレフィックスが落ちる | `%#-8x`, 255 | `ff      ` | `0xff    ` |
| 値 0・精度 0・幅指定ありでパディングが 1 文字不足 | `%5.0d`, 0 | 空白 4 個 | 空白 5 個 |

一方、`-` フラグ・精度・最小フィールド幅・`*` による可変指定、および `%s` / `%c` / `%p` に
幅と精度を組み合わせたケースは libc と一致します。`#` も `%#x` / `%#o` のように単体（右寄せ）で
使う限り正しく `0xff` / `010` を出力します。

---

## ディレクトリ構成

```
printf/
├── Makefile
├── include/ft_printf.h      # t_tab（15 フィールド）と 32 個のプロトタイプ
├── srcs/                    # 15 ファイル / 1,165 行 / 51 関数
│   ├── ft_printf.c          # エントリポイントと指定子ディスパッチ
│   ├── ft_printf_flag.c     # フラグ・幅・精度のパーサ
│   ├── ft_printf_utils.c    # '*' 引数取得、パディング計算、基数変換
│   ├── ft_initialize.c      # 2 段階の初期化
│   ├── ft_print_{c,percent,s,s_null}.c
│   ├── ft_print_int{,_flag}.c / ft_print_unsigned{,_flag}.c
│   └── ft_print_unsigned_long*.c   # %p 専用（NULL の 5 ケースを含む）
└── libft/                   # 同梱の自作 libft（46 ファイル / 1,377 行）
```

---

## 学んだこと

- **可変長引数の実体**。`va_list` は型情報を持たないので、`%d` `%u` `%p` で `va_arg` に渡す型を
  書式から自分で決めるしかありません。`%p` だけ `unsigned long` で受ける必要がある理由が、
  規格の文面ではなく実装として腹落ちしました。
- **戻り値は「書けたバイト数」である**という仕様。出力バッファを持たず `write` の呼び出しごとに
  `len` を加算する設計にしたぶん、パディングやプレフィックスを含めた全経路で数え漏らしが
  ないことを自分で保証する必要がありました。
- **フォーマット仕様の組み合わせ爆発**。フラグ × 幅 × 精度 × 変換 × 値の相互作用は手で確かめ
  られる量ではありません。libc をリファレンスにした総当たり比較を書いて初めて `%05d` が
  効いていないことに気づきました。「動いているように見える」と「仕様どおり」は別物だと
  数字で突きつけられた課題です。
- **制約が設計を決める**こと。Norm の 25 行・引数 4 個という制限がなければ、状態構造体で
  引き回す設計にも、左寄せ／右寄せを関数ごと分ける判断にも至らなかったはずです。

---

42 Tokyo / rank01 · [Kizuna42](https://github.com/Kizuna42)
