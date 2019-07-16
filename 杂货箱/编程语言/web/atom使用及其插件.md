---
title:  atom常用快捷键及插件
date: 2017-12-13 11:29:55
tags:
    - ide
    - 工具
categories:
    - 工具

---
>开发中提高效率的好用插件，不定时更新....
<!-- more -->


# 常用快捷键

1. ctrl+p 文件查询
2. ctrl+shift+p 系统查询
3. ctrl+o 打开文件
4. ctrl+shift+o 打开文件夹
5. ctrl+w 关闭当前编辑文档
6. ctrl+shift+w 关闭编辑器
7. ctrl+shift+c 复制当前文档路径
8. ctrl+shift+l 语法选择
9. ctrl+up 行向上移动
10. ctrl+down 行向下移动
11. ctrl+tab 在打开的文档间进行切换
12. ctrl+r 进行函数查找跳转页面
13. ctrl+shift+k 删除当前行
14. ctrl+delete 删除当前单词



# 次要快捷键
1. ctrl+shift+u 选择编码
2. ctrl+g 跳转到某行
3. ctrl+home 光标移到文件首部
4. ctrl+end 光标移到文件尾部
5. ctrl+shift+home 光标移到文件首部，并选择
6. ctrl+shift+end 光标移到文件尾部，并选择


# 必装实用插件
## minimap
类似sublim text右边的代码缩略图。

## file-types和file-types-icon
file-types用来区分文件类型的，file-types-icon用来给不同的问题类型添加不同的图标

## autocomplete-paths
文件路径自动提示。

## atom-beautify
整理代码格式

## activate-power-mode
跳舞的代码

## autocomplete-plus
提供可能的候选项补全

## regex-railroad-diagram package
正则表达式铁路图

![](img/atom/regex-railroad-diagram package.png)
* 如果光标位于文本上，并且语言标记为正则表达式，则铁路图会自动打开。它改变，而你改变的文字。
* 如果你选择了一些文本或者你的光标在别的地方（没有识别正则表达式），你可以点击ctrl-r ctrl-r打开铁路图视图。然后，您可以编辑正则表达式并按Enter键将其插入光标位置或替换当前选择。按esc取消视图
# 前端插件

## emmet
经典的前端插件，用法不用自行搜索

## atom-ternjs
该插件能对一个对象中拥有的对外提供的属性和方法都能通过suggest的形式提示出来，能对一个对象对外提供的接口有一个选择过程，可以理解为js代码自动提示。

## open-in-browser
在浏览器中打开本页面，要配置浏览器的位置，快捷键ctrl+shift+q

## jquery-snippets
jquery补全工具

## color-picker
取色器

## Pigments
Pigments能够根据颜色代码直接显示该颜色。

# markdown插件

## 基本支持
在atom中已经内置markdown插件可以进行预览，使用功能搜索后输入markdown preview toggle (快捷键ctrl+shift+m)

## markdown-preview-plus
对原本的markdown进行了增强，可以实时预览，使用该插件要禁用atom原来的markdown-preview

## markdown-scroll-sync
同步滚动与光标同步

## language-markdown
一般的Markdown编辑器提供了代码着色等基本功能，language-markdown除了能给代码着色，还提供了快捷的代码片段生成等功能。

## markdown-image-paste
图片功能支持的好坏直接决定了我是否选择使用一个Markdown编辑器。也有不少编辑器和在线的图床绑定，但是这种方式受限于网络。虽然Markdown支持插入本地图片，但是每次插入新图片都是一堆重复操作：截图－命名－保存－插入。markdown-image-paste将这些操作一步完成：
1. 使用截图工具将图片复制到系统剪切板。
2. 在Markdown新起一行输入文件名。
3. Ctrl + V 会自动把图片保存到Markdown文件相同目录下(因此要求Markdown文件应该先保存)，并命名为刚输入的文件名，同时在刚才输入文件名行处生成img标签。

## markdown-table-editor
类似于emmet快捷的表格创建

![](img/atom/markdown-table-editor.gif)

# Python
