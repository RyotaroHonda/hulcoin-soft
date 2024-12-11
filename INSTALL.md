## Install hul-common-lib
### Build example

```
mkdir hulcoin-soft
cd  hulcoin-soft
mkdir build install

git clone https://github.com/spadi-alliance/hulcoin-soft.git hulcoin-soft.src.git 

cmake \
   -S ./hulcoin-soft.src.git \
   -B ./build \
   -DCMAKE_INSTALL_PREFIX=./install \
   -DCMAKE_PREFIX_PATH=<your hul_core_lib install dir>/install
cd build
make
make install
```

### Practical usage

Please DO NOT rewrite the conf.txt in the example-register directory. Copy it to other directory and use it.
