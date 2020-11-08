project('anzu', 'c', version: '0.1')

deps = [
	dependency('sdl2'),
	dependency('SDL2_image'),
	dependency('x11')
]

sources = [
	'src/main.c',
	'src/app.c',
	'src/config.c',
	'src/save.c',
	'src/clipboard.c'
]

executable(
	'anzu',
	sources,
	dependencies: deps,
	install: true
)
