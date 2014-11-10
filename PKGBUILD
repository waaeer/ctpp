# Maintainer: Valentin V. Bartenev <i@vbart.ru>

pkgname=ctpp2
pkgver=2.8.0
pkgrel=1
pkgdesc="Template engine completely written in C++"
arch=('i686' 'x86_64')
url="http://ctpp.havoc.ru/en/"
optdepends=('gettext: gettext support')
makedepends=('cmake' 'make' 'patch')
source=(http://ctpp.havoc.ru/download/ctpp2-$pkgver.tar.gz)
license=('custom:BSD-like')
#md5sums=('')

build() {
  cd $srcdir/$pkgname-$pkgver
  mkdir build
  cd build
  cmake .. || return 1
  make || return 1
  make DESTDIR=$pkgdir install || return 1
  install -Dm 644 ../LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
} 

# Enjoy! ;)
