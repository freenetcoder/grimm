CMC socials update request #1150205:

Delete current socials and grimm website. Only github links are real now. New links will be available after core upgrade. 


![alt text](https://i.ibb.co/vBHKqqV/logo200.png)
## Grimm. No one will ever know

### Current status

#### Mainnet online. First block "timestamp":1564997815 Aug. 5th, 2019 @ 9:36am (UTC) [mainnet branch](https://github.com/freenetcoder/grimm/tree/mainnet)

#### Testnet online [testnet branch](https://github.com/freenetcoder/grimm/tree/testnet)



- Financial freedom
No founders TAX. All mining rewards go directly to the miners. No ICO, STO, IEO, Premine. No transaction fees.
- FULLY Anonymous
Powered by Mimblewimble. Simple wallet.
- Grimm wallet with built-in GPU and CPU mining.
Easy-peasy, fresh and squeezy

We designed Grimm so that it doesn’t depend on any one person. We don’t control it. Not financially. Not physically. How can decentralized work if only founders or entities controls the system? They accumulate taxes, head the richest lists in their projects, or play an unfair game with an unlimited extra coin emission. Grimm is a classic fork of Beam with other consensus rules, open source, without founders rewards, ico, sto, ieo, premine and other commercial shit. Privacy and scalability - two most prominent problems challenges bedeviling the existing Blockchain ecosystem today. So we chose the Mimblewimble protocol, which has fully addressed these problems initially. We are not in favour of mix decentralized projects with a commercial semblance of decentralization. Grimm's got a different, absolutely decentralized way.

- [Read our Manifesto](https://github.com/freenetcoder/Manifesto/blob/master/Manifesto.md)


### EITHER HELP US, OR GET OUT OF OUR WAY

## Grimm spec.
- Protocol / MimbleWimble
- Language / C++
- Consensus / PoW
- PoW Algorithm / Equihash 150_5
- Mining / Grimm wallet with built-in GPU and CPU mining (MacOS, Windows, Linux)
- Emission / Deflationary
- Block Reward / 100 GRIMM, Rewards halving after 1 year and then halvings every 4 years 33 times in total. All mining
- rewards go directly to the miners.
- Max Supply / 262.8M
- Governance / Community
- Blocktime / 60 sec
- Block size / 2 Mb
- Speed / 34 tps
- Transaction fees / 0
- Smallest unit / CENTUM (0.00000001 GRIMM)

### GET STARTED


Get Grimm binaries here: https://grimmw.com or Rleases https://github.com/freenetcoder/grimm/releases

If you build from source you can use 'master' branch\

## How to build

### Windows
- Install Visual Studio >= 2017 with CMake support.
- Download and install Boost prebuilt binaries https://sourceforge.net/projects/boost/files/boost-binaries/1.68.0/boost_1_68_0-msvc-14.1-64.exe, also add BOOST_ROOT to the Environment Variables.
- Download and install OpenSSL prebuilt binaries https://slproweb.com/products/Win32OpenSSL.html (Win64 OpenSSL v1.1.0h for example) and add OPENSSL_ROOT_DIR to the Environment Variables.
- Download and install QT 5.11 https://download.qt.io/official_releases/qt/5.11/5.11.0/qt-opensource-windows-x86-5.11.0.exe.mirrorlist and add QT5_ROOT_DIR to the Environment Variables (usually it looks like .../5.11.0/msvc2017_64), also add QML_IMPORT_PATH (it should look like %QT5_ROOT_DIR%\qml). BTW disabling system antivirus on Windows makes QT installing process much faster.
Add .../qt511/5.11.1/msvc2017_64/bin and .../boost_1_68_0/lib64-msvc-14.1 to the System Path.
- Open project folder in Visual Studio, select your target (Release-x64 for example, if you downloaded 64bit Boost and OpenSSL) and select CMake -> Build All.
- Go to CMake -> Cache -> Open Cache Folder -> grimm (you'll find binaries in grimm/..., wallet/..., ui/..., explorer/... subfolders).

### Linux
### Ubuntu 18.04
- Install gcc7 boost ssl packages.
  sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  sudo apt update
  sudo apt install g++-7 libboost-all-dev libssl-dev -y
- Set it up so the symbolic links gcc, g++ point to the newer version:
  sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 \
                           --slave /usr/bin/g++ g++ /usr/bin/g++-7
  sudo update-alternatives --config gcc
  gcc --version
  g++ --version
- Install latest CMake
  wget "https://cmake.org/files/v3.12/cmake-3.12.0-Linux-x86_64.sh"
  sudo sh cmake-3.12.0-Linux-x86_64.sh --skip-license --prefix=/usr
- Add proper QT 5.11 repository depending on your system https://launchpad.net/~beineri (for example, choose Qt 5.10.1 for /opt Trusty if you have Ubuntu 14.04), install sudo apt-get install qt510declarative qt510svg packages and add export PATH=/opt/qt511/bin:$PATH.
Go to Grimm project folder and call cmake -DCMAKE_BUILD_TYPE=Release . && make -j4.
You'll find binaries in grimm/..., wallet/..., ui/..., explorer/... subfolders.

### Mac
Install Brew Package Manager.
Installed necessary packages using brew install openssl boost cmake command.
Download and install QT5.11 from the official website https://download.qt.io/official_releases/qt/5.11/5.11.0/qt-opensource-mac-x64-5.11.0.dmg.mirrorlist
Add export OPENSSL_ROOT_DIR="/usr/local/opt/openssl" and export PATH=/Users/<username>/Qt5.11.0/5.11.0/clang_64/bin:$PATH to the Environment Variables.
Go to Grimm project folder and call cmake -DCMAKE_BUILD_TYPE=Release . && make -j4.
You'll find binaries in grimm/..., wallet/..., ui/..., explorer/... subfolders.
If you don't want to build UI don't install QT5 and add -DGRIMM_NO_QT_UI_WALLET=On command line parameter when you are calling cmake.
