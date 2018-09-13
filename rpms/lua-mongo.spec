%define luaver 5.1
%define lualibdir %{_libdir}/lua/%{luaver}
%define luapkgdir %{_datadir}/lua/%{luaver}

%ifarch x86_64
%global with_tests   1%{!?_without_tests:1}
%else
%global with_tests   0%{?_with_tests:1}
%endif

Name:           lua-mongo
Version:        1.2.0
Release:        2%{?dist}
BuildArch:      x86_64
Summary:        Mongodb connectivity for the Lua programming language
Group:          Development/Libraries
License:        MIT
URL:            https://github.com/neoxic/lua-mongo/
Source:         https://github.com/neoxic/lua-mongo/archive/v%{version}.tar.gz
Patch0:         lua-mongo-1.2.0-el6.patch

BuildRequires:  gcc
BuildRequires:  cmake3
BuildRequires:  pkgconfig
BuildRequires:  lua >= %{luaver}, lua-devel >= %{luaver}, lua-static >= %{luaver}
BuildRequires:  mongo-c-driver-devel
BuildRequires:  libbson-devel
BuildRequires:  cyrus-sasl-devel
BuildRequires:  openssl-devel
BuildRequires:  snappy-devel
%if %{with_tests}
BuildRequires:  mongodb-org-server
BuildRequires:  openssl
%endif

Requires:  lua >= %{luaver}
Requires:  mongo-c-driver

%description
lua-mongo is a binding to the MongoDB C Driver for Lua.
Unified API for MongoDB commands, CRUD operations and GridFS in the MongoDB C Driver.
Support for custom data transformation handlers when converting to/from BSON documents.
Transparent conversion from Lua/JSON to BSON for convenience.
Automatic conversion of Lua numbers to/from BSON Int32, Int64 and Double types depending 
on their capacity without precision loss (when Lua allows it). Manual conversion is also available.

%package doc
Summary:        Documentation for lua-mongo
Group:          Documentation
Requires:       lua >= %{luaver}

%description doc
Documentation for lua-mongo

%prep
%setup -q -n lua-mongo-%{version}
%patch0 -p1

%build
%cmake3 \
    -DUSE_LUA_VERSION= .

make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{luapkgdir}
mkdir -p $RPM_BUILD_ROOT%{lualibdir}
cp -p *.so $RPM_BUILD_ROOT%{lualibdir}
#make install PREFIX=$RPM_BUILD_ROOT%{_prefix} LUA_LIBDIR=$RPM_BUILD_ROOT%{lualibdir} LUA_DIR=$RPM_BUILD_ROOT%{luapkgdir}

%check
%if %{with_tests}
: Run a server
mkdir lua-mongo-test
mongod \
  --journal \
  --ipv6 \
  --unixSocketPrefix /tmp \
  --logpath     $PWD/server.log \
  --pidfilepath $PWD/server.pid \
  --dbpath      $PWD/lua-mongo-test \
  --fork

: Run the test suite
ret=0

make test || ret=1

: Cleanup
[ -s server.pid ] && kill $(cat server.pid)

exit $ret
%endif

%files
%doc README.md
%{lualibdir}/*.so

%files doc
%license LICENSE
%doc doc/*

%changelog
* Thu Sep 14 2018 iglov <root@avalon.land> 1.2.0-2
- Added tests.

* Thu Sep 13 2018 iglov <root@avalon.land> 1.2.0-1
- Ported to centos 6 by iglov@avalon.land.
