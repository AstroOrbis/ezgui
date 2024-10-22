shebang := if os_family() == 'windows' {
	'powershell'
} else {
	'sh'
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
	rm -rf build-demo

cloc:
	cloc . --exclude-dir="build" --exclude-dir="vcpkg_installed"