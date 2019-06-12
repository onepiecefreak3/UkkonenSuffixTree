IF NOT EXIST "build" mkdir "build"
call g++ -D__Win -fPIC --shared -o build\\ukkonen32.dll UkkonenSuffixTree\\UkkonenExport.cpp UkkonenSuffixTree\\SuffixTree.cpp UkkonenSuffixTree\\SuffixTreeNode.cpp
call g64++ -D__Win -m64 -fPIC --shared -o build\\ukkonen64.dll UkkonenSuffixTree\\UkkonenExport.cpp UkkonenSuffixTree\\SuffixTree.cpp UkkonenSuffixTree\\SuffixTreeNode.cpp
