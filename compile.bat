call g++ -D__Win -fPIC --shared -o ukkonen32.dll UkkonenExport.cpp SuffixTree.cpp SuffixTreeNode.cpp
call g64++ -D__Win -m64 -fPIC --shared -o ukkonen64.dll UkkonenExport.cpp SuffixTree.cpp SuffixTreeNode.cpp
