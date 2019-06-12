if [ ! -d "build" ]; then mkdir "build"; fi
g++ -D__Unix -fPIC -m32 -shared -o build/ukkonen32.so UkkonenSuffixTree/UkkonenExport.cpp UkkonenSuffixTree/SuffixTree.cpp UkkonenSuffixTree/SuffixTreeNode.cpp
g++ -D__Unix -fPIC -shared -o build/ukkonen64.so UkkonenSuffixTree/UkkonenExport.cpp UkkonenSuffixTree/SuffixTree.cpp UkkonenSuffixTree/SuffixTreeNode.cpp
