# fedfetch
Screenshot information tool for Fedora GNU/Linux, written in C, it is fork of archfetch.
![fedfetch](https://user-images.githubusercontent.com/18743742/86501689-32387d00-bd69-11ea-97e9-e4ce27a62875.png)
It contains ASCII characrers, so no Unicode required.
To dowload archfetch type in your terminal:
# git clone -b master --depth 1 https://github.com/DmitryHetman/fedfetch
GPU detection still doesn't work, you can just edit source code to 
substitute Intel HD with your GPU model:
# $ nano fedfetch.c
To run archfetch you must compile and install it:
# $ make
# (root) # make install
