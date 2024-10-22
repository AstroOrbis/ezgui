shebang := if os_family() == 'windows' {
	'powershell'
} else {
	'/bin/bash'
}

executable := if os_family() == 'windows' {
	'Debug/demo.exe'
} else {
	'demo'
}

demo:
	#!{{shebang}}
	cmake -B build -S . -DBUILD_DEMO=ON
	cmake --build build
	./build/{{executable}}

build:
	#!{{shebang}}
	cmake -B build -S .
	cmake --build build

clean:
	rm -rf build

cloc:
	cloc . --exclude-dir="build" --exclude-dir="vcpkg_installed"