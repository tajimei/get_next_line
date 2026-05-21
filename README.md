*This project has been created as part of the 42 curriculum by mtajima.*

# get_next_line
---

## Description

`get_next_line` is a project that implements a function which reads one line at a time from a file descriptor.

### Goal

- Read text one line at a time through a file descriptor
- Understand and apply the concept of **static variables** in C programming
- Handle memory management correctly using `malloc` and `free`

### Overview

By repeatedly calling `get_next_line(int fd)`, you can retrieve one line at a time from a file or standard input. It returns `NULL` when there is nothing left to read or when an error occurs.

---

## Instructions

### Compilation

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c
```

You can change `BUFFER_SIZE` to any value. If omitted, the default value (42) is used.

```bash
# Without specifying BUFFER_SIZE
cc -Wall -Wextra -Werror main.c get_next_line.c get_next_line_utils.c
```

### Usage Example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    // Reading from a file
    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);

    // Reading from standard input
    while ((line = get_next_line(0)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    return (0);
}
```

### Files

| File | Role |
|---|---|
| `get_next_line.c` | Main function implementation |
| `get_next_line_utils.c` | Helper functions implementation |
| `get_next_line.h` | Prototypes and macro definitions |

---

## Algorithm

### Chosen Algorithm: Static Buffer Accumulation

#### Overview

This implementation reads `BUFFER_SIZE` bytes at a time using `read()`, and accumulates the data into a static variable called `stash`.

#### Flow

```
get_next_line is called
        ↓
① Check if stash (leftover data) contains \n
        ↓ if not
② Call read() and append to stash → repeat until \n is found
        ↓
③ Extract up to \n and return it (= current line)
        ↓
④ Save everything after \n into stash (= carry over to next call)
```

#### Function Breakdown

| Function | Role |
|---|---|
| `read_to_buffer` | Calls `read()` and accumulates data into stash |
| `extract_line` | Extracts and returns everything up to `\n` from stash |
| `update_stash` | Saves everything after `\n` as the new stash |
| `get_next_line` | Main function that orchestrates the above three |

#### Role of the Static Variable

```
1st call:
  stash = NULL
  read → "Hello\nWorld\n"
  stash = "Hello\nWorld\n"
  returns "Hello\n"
  stash = "World\n"  ← persists after function returns

2nd call:
  stash = "World\n" is still there
  returns "World\n" without calling read
  stash = NULL

3rd call:
  stash = NULL → read → bytes_read == 0 → returns NULL
```

#### Why This Approach

- **Simple and readable**: Splitting into 3 functions makes each responsibility clear
- **Memory efficient**: Reads only what is needed, no unnecessary reads
- **Works with any BUFFER_SIZE**: Handles values as small as 1 or as large as 10000000
- **Works with both files and stdin**: Takes fd as a parameter for flexibility

---

## Resources

### References

- [man page: read(2)](https://man7.org/linux/man-pages/man2/read.2.html) - Specification of the read() system call
- [man page: open(2)](https://man7.org/linux/man-pages/man2/open.2.html) - How to obtain a file descriptor
- [C static variables](https://en.cppreference.com/w/c/language/storage_duration) - Storage duration of static variables in C
- [File descriptors in C](https://www.gnu.org/software/libc/manual/html_node/File-Descriptors.html) - GNU documentation on file descriptors

### AI Usage

The following describes how AI was used in this project:

| Purpose | Details |
|---|---|
| Understanding concepts | Clarifying how static variables work, understanding `ssize_t` |
| README creation | Assistance in writing this file |

The implementation itself was designed and written independently without relying on AI.

---
*以下日本語版*

# get_next_line
---

## Description

`get_next_line` は、ファイルディスクリプタから1行ずつ読み取る関数の実装プロジェクトです。

### 目的

- ファイルディスクリプタを通じてテキストを1行単位で読み取る
- Cプログラミングにおける **static変数** の概念を理解・活用する
- メモリ管理（malloc / free）を正しく扱う

### 概要

`get_next_line(int fd)` を繰り返し呼び出すことで、ファイルまたは標準入力から1行ずつ文字列を返します。読み取るものがなくなった場合、またはエラーが発生した場合は `NULL` を返します。

---

## Instructions

### コンパイル方法

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c
```

`BUFFER_SIZE` は任意の値に変更可能です。省略した場合はデフォルト値（42）が使用されます。

```bash
# BUFFER_SIZE を指定しない場合
cc -Wall -Wextra -Werror main.c get_next_line.c get_next_line_utils.c
```

### 使用例

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    // ファイルから読み取る場合
    fd = open("test.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);

    // 標準入力から読み取る場合
    while ((line = get_next_line(0)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    return (0);
}
```

### 提出ファイル

| ファイル | 役割 |
|---|---|
| `get_next_line.c` | メイン関数の実装 |
| `get_next_line_utils.c` | ヘルパー関数の実装 |
| `get_next_line.h` | プロトタイプ宣言・マクロ定義 |

---

## Algorithm

### 選択したアルゴリズム：static バッファ蓄積方式

#### 概要

`read()` を呼び出すたびに `BUFFER_SIZE` バイトずつデータを読み込み、static変数（`stash`）に蓄積していく方式を採用しました。

#### 処理の流れ

```
get_next_line が呼ばれる
        ↓
① stash（読み残し）に \n があるか確認
        ↓ ない場合
② read() して stash に追加 → \n が見つかるまで繰り返す
        ↓
③ \n までを切り出して return（= 今回の行）
        ↓
④ \n より後ろを stash に保存（= 次回への持ち越し）
```

#### 関数の役割分担

| 関数 | 役割 |
|---|---|
| `read_to_buffer` | `read()` を呼び出して stash にデータを蓄積する |
| `extract_line` | stash から `\n` までを切り出して返す |
| `update_stash` | `\n` より後ろを新しい stash として保存する |
| `get_next_line` | 上記3つを統括するメイン関数 |

#### static変数の役割

```
1回目の呼び出し:
  stash = NULL
  read → "Hello\nWorld\n" を読む
  stash = "Hello\nWorld\n"
  "Hello\n" を返す
  stash = "World\n" として保存 ← 関数終了後も残る

2回目の呼び出し:
  stash = "World\n" がまだ残っている
  read せずに "World\n" を返せる
  stash = NULL に更新

3回目の呼び出し:
  stash = NULL → read → bytes_read == 0 → NULL を返す
```

#### この方式を選んだ理由

- **シンプルで理解しやすい構造**：3つの関数に役割を分けることで、各処理が明確になる
- **メモリ効率が良い**：必要な分だけ読み込み、余分な読み込みをしない
- **あらゆる BUFFER_SIZE に対応**：1でも9999でも正しく動作する
- **ファイルと標準入力の両方に対応**：fd を引数で受け取るため汎用性が高い

---

## Resources

### 参考資料

- [man page: read(2)](https://man7.org/linux/man-pages/man2/read.2.html) - read() システムコールの仕様
- [man page: open(2)](https://man7.org/linux/man-pages/man2/open.2.html) - ファイルディスクリプタの取得方法
- [C static variables](https://en.cppreference.com/w/c/language/storage_duration) - static変数のストレージ期間について
- [File descriptors in C](https://www.gnu.org/software/libc/manual/html_node/File-Descriptors.html) - GNUによるファイルディスクリプタの解説

### AIの使用について

本プロジェクトにおけるAIの使用は以下の通りです：

| 用途 | 使用箇所 |
|---|---|
| 概念の理解 | static変数の動作確認、ssize_t の意味の確認 |
| READMEの作成 | 本ファイルの作成補助 |

実装そのものはAIに依存せず、自分で設計・実装しました。