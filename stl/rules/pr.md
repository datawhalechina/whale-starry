## PR Standard

提交PR请遵循以下规范，否则不合入主干!!!

pr模版示例：[helloworld pr](https://github.com/datawhalechina/whale-starry/pull/1)

PR title: [Team_Task]: Your Custom Title.

for example: [fight_flow_1]: vector reading and adding test.

### What

- brief: 描述本次pr的内容简介
- author: 作者

### Directory Specification

示例：提交的代码放在自己的团队目录里面，例如fight_flow, 团队命名为小写下划线，第一天由各个Team Leader提交各个组的信息，并创建目录，随后正式学习的时候，大家提pr往自己的team里面提交。

```
└── stl -> root dir
    ├── README.md 
    ├── example
    │   ├── README.md
    │   └── week1.cpp
    ├── fight_flow  -> your team name should follow lowercase underscore style(小写下划线)
    │   ├── lisi -> team member name1, please use english name
    │   │   └── learning.md -> your document file
    │   │   └── src -> your source code
    │   └── zhangsan -> team member name2
    │       └── vector.md -> your document file
    │       └── src -> your source code
```

### What changes are included in this PR?

在这个pr包含什么?

### Are these changes tested?

编写对应的issue测试文件与源代码逻辑。

### other information

其他信息。