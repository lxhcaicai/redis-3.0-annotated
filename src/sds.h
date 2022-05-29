/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __SDS_H
#define __SDS_H

/*
 * 最大预分配长度
 */
#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

/*
 * 类型别名，用于指向 sdshdr 的 buf 属性
 */
typedef char *sds;

/*
 * 保存字符串对象的结构
 */
struct sdshdr {
    
    // buf 中已占用空间的长度
    int len;

    // buf 中剩余可用空间的长度
    int free;

    // 数据空间
    char buf[];
};

/*
 * 返回 sds 实际保存的字符串的长度
 *
 * T = O(1)
 */
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

/*
 * 返回 sds 可用空间的长度
 *
 * T = O(1)
 */
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}

/*
 * 根据给定的初始化字符串init和字符串长度initlen 创建一个新的sds
 * */
sds sdsnewlen(const void *init, size_t initlen);

/*
 * 根据给定的字符串init, 创建一个包含同样字符串的sds
 * */
sds sdsnew(const char *init);

/*
 * 创建一个长度为0的sds 空字符串
 * */
sds sdsempty(void);

/*
 * 返回 sds 实际保存的字符串的长度
 * */
size_t sdslen(const sds s);

/*
 * 复制给定 sds 的副本
 *
 *返回值
 * sds ：创建成功返回输入 sds 的副本
 * 创建失败返回 NULL
 * */
sds sdsdup(const sds s);

/*
 * 释放给定的 sds
 *
 * 复杂度
 *  T = O(N)
 *
 *  释放一个sds字符串。如果's'为NULL，则不执行任何操作。
 */
void sdsfree(sds s);

/*
 * 返回 sds 可用空间的长度
 *
 * T = O(1)
 */
size_t sdsavail(const sds s);


/*
 * 将 sds 扩充至指定长度，未使用的空间以 0 字节填充。
 * 如果指定长度小于当前长度则不进行操作
 *
 * 返回值
 *  sds ：扩充成功返回新 sds ，失败返回 NULL
 *
 * 复杂度：
 *  T = O(N)
 */
sds sdsgrowzero(sds s, size_t len);

/*
 * 将长度为 len 的字符串 t 追加到 sds 的字符串末尾
 *
 * 返回值
 *  sds ：追加成功返回新 sds ，失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 *  调用之后，传递的sds字符串不再有效，并且所有的引用必须用调用返回的新指针替换。
 *
 */
sds sdscatlen(sds s, const void *t, size_t len);

/*
 * 将给定字符串 t 追加到 sds 的末尾
 *
 * 返回值
 *  sds ：追加成功返回新 sds ，失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds sdscat(sds s, const char *t);

/*
 * 将另一个 sds 追加到一个 sds 的末尾
 *
 * 返回值
 *  sds ：追加成功返回新 sds ，失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds sdscatsds(sds s, const sds t);

/*
 * 将字符串 t 的前 len 个字符复制到 sds s 当中，
 * 并在字符串的最后添加终结符。
 *
 * 如果 sds 的长度少于 len 个字符，那么扩展 sds
 *
 * 复杂度
 *  T = O(N)
 *
 * 返回值
 *  sds ：复制成功返回新的 sds ，否则返回 NULL
 */
sds sdscpylen(sds s, const char *t, size_t len);

/*
 * 将字符串复制到 sds 当中，
 * 覆盖原有的字符。
 *
 * 如果 sds 的长度少于字符串的长度，那么扩展 sds 。
 *
 * 复杂度
 *  T = O(N)
 *
 * 返回值
 *  sds ：复制成功返回新的 sds ，否则返回 NULL
 */
sds sdscpy(sds s, const char *t);

/*
 * 打印函数，被 sdscatprintf 所调用
 *
 * T = O(N^2)
 */
sds sdscatvprintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

/*
 * 这个函数类似于sdscatprintf，但要快得多
 * 不依赖于由libc实现的sprintf()族函数
 *  通常都很慢。此外，直接将sds字符串处理为
 *   连接新数据提供了性能改进。
 * */
sds sdscatfmt(sds s, char const *fmt, ...);

/*
 * 对 sds 左右两端进行修剪，清除其中 cset 指定的所有字符
 * */
sds sdstrim(sds s, const char *cset);

/*
 * 按索引对截取 sds 字符串的其中一段
 * start 和 end 都是闭区间（包含在内）
 *
 * 索引从 0 开始，最大为 sdslen(s) - 1
 * 索引可以是负数， sdslen(s) - 1 == -1
 *
 * 复杂度
 *  T = O(N)
 */
void sdsrange(sds s, int start, int end);

/*
 * 设置sds字符串长度为strlen()得到的长度
 * */
void sdsupdatelen(sds s);

/*
 * 在不释放 SDS 的字符串空间的情况下，
 * 重置 SDS 所保存的字符串为空字符串。
 *
 * 复杂度
 *  T = O(1)
 */
void sdsclear(sds s);

/*
 * 对比两个 sds ， strcmp 的 sds 版本
 *
 * 返回值
 *  int ：相等返回 0 ，s1 较大返回正数， s2 较大返回负数
 *
 * T = O(N)
 */
int sdscmp(const sds s1, const sds s2);



/*
 * 使用分隔符 sep 对 s 进行分割，返回一个 sds 字符串的数组。
 * *count 会被设置为返回数组元素的数量。
 *
 * 如果出现内存不足、字符串长度为 0 或分隔符长度为 0
 * 的情况，返回 NULL
 *
 * 注意分隔符可以的是包含多个字符的字符串
 *
 * 这个函数接受 len 参数，因此它是二进制安全的。
 * （文档中提到的 sdssplit() 已废弃）
 *
 * T = O(N^2)
 */
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);

/*
 * 释放 tokens 数组中 count 个 sds
 *
 * T = O(N^2)
 * */
void sdsfreesplitres(sds *tokens, int count);

/*
 * 将 sds 字符串中的所有字符转换为小写
 *
 * T = O(N)
 */
void sdstolower(sds s);

/*
 * 将 sds 字符串中的所有字符转换为大写
 *
 * T = O(N)
 */
void sdstoupper(sds s);


/*
 * 根据输入的 long long 值 value ，创建一个 SDS
 * */
sds sdsfromlonglong(long long value);

/*
 * 将长度为 len 的字符串 p 以带引号（quoted）的格式
 * 追加到给定 sds 的末尾
 *
 * T = O(N)
 */
sds sdscatrepr(sds s, const char *p, size_t len);

/*
 * 将一行文本分割成多个参数，每个参数可以有以下的类编程语言 REPL 格式：
 *
 * 参数的个数会保存在 *argc 中，函数返回一个 sds 数组。
 *
 * 调用者应该使用 sdsfreesplitres() 来释放函数返回的 sds 数组。

 *
 * sdscatrepr() 可以将一个字符串转换为一个带引号（quoted）的字符串，
 * 这个带引号的字符串可以被 sdssplitargs() 分析。
 *
 * 即使输入出现空字符串， NULL ，或者输入带有未对应的括号，
 * 函数都会将已成功处理的字符串先返回。
 *
 * 这个函数主要用于 config.c 中对配置文件进行分析。
 * 例子：
 *  sds *arr = sdssplitargs("timeout 10086\r\nport 123321\r\n");
 * 会得出
 *  arr[0] = "timeout"
 *  arr[1] = "10086"
 *  arr[2] = "port"
 *  arr[3] = "123321"
 *
 * T = O(N^2)
 */
sds *sdssplitargs(const char *line, int *argc);

/*
 * 将字符串 s 中，
 * 所有在 from 中出现的字符，替换成 to 中的字符
 *
 * 比如调用 sdsmapchars(mystring, "ho", "01", 2)
 * 就会将 "hello" 转换为 "0ell1"
 *
 * 因为无须对 sds 进行大小调整，
 * 所以返回的 sds 输入的 sds 一样
 *
 * T = O(N^2)
 */
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);

/*
 * 使用指定的分隔符(也是一个C字符串)连接一个C字符串数组。
 * */
sds sdsjoin(char **argv, int argc, char *sep);



// 向用户公开的低级API
/* Low level functions exposed to the user API */

/*
 * 对 sds 中 buf 的长度进行扩展，确保在函数执行之后，
 * buf 至少会有 addlen + 1 长度的空余空间
 * （额外的 1 字节是为 \0 准备的）
 *
 * 返回值
 *  sds ：扩展成功返回扩展后的 sds
 *        扩展失败返回 NULL
 *
 * 复杂度
 *  T = O(N)
 */
sds sdsMakeRoomFor(sds s, size_t addlen);


/*
 *
 * 根据 incr 参数，增加 sds 的长度，缩减空余空间，
 * 并将 \0 放到新字符串的尾端
 *
 *
 * 这个函数是在调用 sdsMakeRoomFor() 对字符串进行扩展，
 * 然后用户在字符串尾部写入了某些内容之后，
 * 用来正确更新 free 和 len 属性的。
 *
 * 如果 incr 参数为负数，那么对字符串进行右截断操作。
 *
 * 以下是 sdsIncrLen 的用例：
 *
 * oldlen = sdslen(s);
 * s = sdsMakeRoomFor(s, BUFFER_SIZE);
 * nread = read(fd, s+oldlen, BUFFER_SIZE);
 * ... check for nread <= 0 and handle it ...
 * sdsIncrLen(s, nread);
 *
 * 复杂度
 *  T = O(1)
 */
void sdsIncrLen(sds s, int incr);

/*
 * 回收 sds 中的空闲空间，
 * 回收不会对 sds 中保存的字符串内容做任何修改。
 *
 * 返回值
 *  sds ：内存调整后的 sds
 *
 * 复杂度
 *  T = O(N)
 */
sds sdsRemoveFreeSpace(sds s);

/*
 * 返回给定 sds 分配的内存字节数
 *
 * 复杂度
 *  T = O(1)
 */
size_t sdsAllocSize(sds s);

#endif
