project('FinalProject', 'c',
            default_options : 'c_std=c89',
            version: '0.0.1',
            license: 'GPL3')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m') # Math library

inc = include_directories('.')
datadir = join_paths(get_option('datadir'), 'doc')

projlib_sources = [
    'bike.c',
    'bike_history.c',
    'campus.c',
    'file.c',
    'loan.c',
    'menu.c',
    'stats.c',
    'user.c',
    'util.c'
]

projlib = static_library('projlib', projlib_sources,
                         include_directories: inc,
                         install: false)

executable('finalproject', 'main.c',
           link_with: projlib,
           dependencies: m_dep)
