if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap <silent> [e <Plug>JumpDiffCharPrevEnd
nmap <silent> [b <Plug>JumpDiffCharPrevStart
nmap <silent> \p <Plug>PutDiffCharPair
nmap <silent> \g <Plug>GetDiffCharPair
nmap <silent> ]e <Plug>JumpDiffCharNextEnd
nmap <silent> ]b <Plug>JumpDiffCharNextStart
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
nnoremap <silent> <Plug>PutDiffCharPair :call diffchar#CopyDiffCharPair(1)
nnoremap <silent> <Plug>GetDiffCharPair :call diffchar#CopyDiffCharPair(0)
nnoremap <silent> <Plug>JumpDiffCharNextEnd :call diffchar#JumpDiffChar(1, 1)
nnoremap <silent> <Plug>JumpDiffCharPrevEnd :call diffchar#JumpDiffChar(0, 1)
nnoremap <silent> <Plug>JumpDiffCharNextStart :call diffchar#JumpDiffChar(1, 0)
nnoremap <silent> <Plug>JumpDiffCharPrevStart :call diffchar#JumpDiffChar(0, 0)
nnoremap <silent> <Plug>ToggleDiffCharCurrentLine :TDChar
nmap <silent> <F8> <Plug>ToggleDiffCharCurrentLine
nnoremap <silent> <Plug>ToggleDiffCharAllLines :%TDChar
nmap <silent> <F7> <Plug>ToggleDiffCharAllLines
nnoremap <silent> <Plug>(ale_hover) :ALEHover
nnoremap <silent> <Plug>(ale_find_references) :ALEFindReferences
nnoremap <silent> <Plug>(ale_go_to_definition_in_tab) :ALEGoToDefinitionInTab
nnoremap <silent> <Plug>(ale_go_to_definition) :ALEGoToDefinition
nnoremap <silent> <Plug>(ale_fix) :ALEFix
nnoremap <silent> <Plug>(ale_detail) :ALEDetail
nnoremap <silent> <Plug>(ale_lint) :ALELint
nnoremap <silent> <Plug>(ale_reset_buffer) :ALEResetBuffer
nnoremap <silent> <Plug>(ale_disable_buffer) :ALEDisableBuffer
nnoremap <silent> <Plug>(ale_enable_buffer) :ALEEnableBuffer
nnoremap <silent> <Plug>(ale_toggle_buffer) :ALEToggleBuffer
nnoremap <silent> <Plug>(ale_reset) :ALEReset
nnoremap <silent> <Plug>(ale_disable) :ALEDisable
nnoremap <silent> <Plug>(ale_enable) :ALEEnable
nnoremap <silent> <Plug>(ale_toggle) :ALEToggle
nnoremap <silent> <Plug>(ale_last) :ALELast
nnoremap <silent> <Plug>(ale_first) :ALEFirst
nnoremap <silent> <Plug>(ale_next_wrap) :ALENextWrap
nnoremap <silent> <Plug>(ale_next) :ALENext
nnoremap <silent> <Plug>(ale_previous_wrap) :ALEPreviousWrap
nnoremap <silent> <Plug>(ale_previous) :ALEPrevious
let &cpo=s:cpo_save
unlet s:cpo_save
set background=dark
set backspace=indent,eol,start
set backupdir=~/.cache/vim/backup//
set completeopt=menu,menuone
set diffexpr=diffchar#DiffCharExpr()
set directory=~/.cache/vim/swap//
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set grepprg=grep\ -nH\ $*
set helplang=en
set hlsearch
set laststatus=2
set ruler
set shiftwidth=2
set softtabstop=2
set splitright
set suffixes=.bak,~,.o,.info,.swp,.aux,.bbl,.blg,.brf,.cb,.dvi,.idx,.ilg,.ind,.inx,.jpg,.log,.out,.png,.toc
set tabstop=2
set undodir=~/.cache/vim/undo//
set window=42
" vim: set ft=vim :
