# branchless
C++ header only library provided branchless code blocks
======================

Example
------------------------
    int array[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int index = bsearch_branchless_tiny<8>::op(3, array);

Licensing
------------------------

Apache License, Version 2.0

Requirements
------------------------

Tested on Linux and Mac OS X.

Acknowledgement
------------------------

This work is based on Sean Eron Anderson "Bit Twiddling Hacks" https://graphics.stanford.edu/~seander/bithacks.html
