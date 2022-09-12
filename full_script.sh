git clone https://github.com/zeFresk/tmp-quantum-circuit-EASEA.git
cd tmp-quantum-circuit-EASEA
git checkout remote
git submodule init
git submodule sync
git submodule update
export CXX=g++-10
./compile.sh
cp * /tmp/
