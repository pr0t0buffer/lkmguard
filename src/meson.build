include_dirs = include_directories('include')
sources = files('lkmguard_main.c', 
		'integrity/integrity.c',
		'integrity/hooks.c',
		'keylogger/keylogger.c'
		,'Makefile')

if meson.version().version_compare('<0.40.0')
	lkmguard = custom_target('lkmguard',
		output: ['lkmguard.ko'],
		input: sources,
		command: [meson.source_root()+'/src/build.sh',meson.source_root()+'/src',meson.build_root()],
		install: true,
		install_dir: 'bin',
		build_always: true
		)
else
	lkmguard = custom_target('lkmguard',
		output: ['lkmguard.ko'],
		input: sources,
		command: [meson.source_root()+'/src/build.sh',meson.source_root()+'/src',meson.build_root()],
		install: true,
		install_dir: 'bin',
		build_by_default: true
		)
endif
