inherit eutils

DESCRIPTION="CT++"
HOMEPAGE="http://ctpp.havoc.ru"

SRC_URI="http://ctpp.havoc.ru/download/${P}.tar.gz"

LICENSE="BSD"
KEYWORDS="x86"

DEPEND="dev-util/cmake"

src_compile() {
	cmake . || die
	emake || die
}

src_install() {
	dobin ctpp2c ctpp2json ctpp2vm ctpp2c ctpp2-config

	dolib.so *.so*

	insinto /usr/include/ctpp2
	doins include/*

	dodoc CHANGES
}
