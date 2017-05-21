" 个人用户的在$HOMN/.vimrc
" 基本设置
"set nocompatible	" 不兼容，在和vi有出入时，用vim的方式处理，/etc/vimrc已经设置了
set number		" 在左边显示行号
set ruler		" 下方状态栏显示光标行列坐标
"set tabstop=4		" 接下来三个是设置tab间距用的，默认是8
"set softtabstop=4
"set shiftwidth=4
syntax on		" 语法高亮

set autowrite		" 切换文件时自动保存未修改的
"set list		" 显示tab或结尾时用，代码里经常含有很多空白的冗余字符
"set listchars=tab:->,trail:-
set showcmd		"  显示命令，也就是连续多键组成的命令，没按完时显示已经按了那些键

"set foldmarker=#ifdef,#endif " 折叠代码块用的，vi不能识别，设置了影响vi
"set foldmethod=marker

map \\ I//<Esc>         " 键映射，连按两下\键，会将代码行注释
map <C-H> <C-W>h        " 多窗口时，窗口间游走的快捷键
map <C-H> <C-W>j
map <C-K> <C-W>k
map <C-L> <C-W>l
map <Left> :bprevious<CR> " 打开上次打开过的文件
map <Right> :bnext<CR>
"map <Space> :b#<cr>

" 工程项目设置，相互隔离，防止tags跳到别的工程下
if $PWD =~ 'Project_Trunk'
    set path+=~/Project_Trunk /Include   " 头文件目录，也就是按gf时能进入到的#include "xx /xx.h"文件
    set tag=~/Project_Trunk/tags
elseif $PWD =~ 'Project_Branch'
    set tag=~/Project_Branch/tags
else
    "set tag=~/xxx
endif

" Taglist插件设置
let Tlist_Ctags_Cmd = '/usr/bin/ctags'
let Tlist_Exit_OnlyWindow = 1 
let Tlist_Show_One_File = 1 
"let Tlist_Auto_Open = 1

