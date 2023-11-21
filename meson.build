project('ONScripter-EN', 'cpp',
  version: '20230628',
  default_options: ['cpp_std=c++98',
                      'c_std=c99',
                      'warning_level=3',
                      'buildtype=release',
                      'b_ndebug=if-release'],
  meson_version: '>= 0.49.0')

cxx = meson.get_compiler('cpp')
cpu_family = target_machine.cpu_family()
target_system = target_machine.system()
target_endian = target_machine.endian()

if target_system == 'linux'
    add_project_arguments('-DLINUX', language : ['cpp','c'])
endif
add_project_arguments('-DUSE_OGG_VORBIS', language : ['cpp','c'])

has_format_security = cxx.has_argument('-Wformat-security')
if has_format_security
    add_project_arguments('-Wformat-security', language : ['cpp','c'])
endif

has_pointer_arith = cxx.has_argument('-Wpointer-arith')
if has_pointer_arith
    add_project_arguments('-Wpointer-arith', language : ['cpp','c'])
endif

has_omit_fp = cxx.has_argument('-fomit-frame-pointer')
if has_omit_fp
    add_project_arguments('-fomit-frame-pointer', language : ['cpp','c'])
endif

has_cxx_pipe = cxx.has_argument('-pipe')
if has_cxx_pipe
    add_project_arguments('-pipe', language : ['cpp','c'])
endif

onscripter_pie = false
hardened_args = []
if get_option('hardened').enabled()
    hardened_args = ['-Wl,-z,relro', '-Wl,-z,now']
    add_project_arguments('-fstack-protector', language: ['cpp','c'])
    onscripter_pie = true
if get_option('optimization') != '0'
    add_project_arguments('-D_FORTIFY_SOURCE=2', language: ['cpp','c'])
endif
endif

# smpeg needs to be detected manually
smpeg_config = find_program('smpeg-config')
if smpeg_config.found()
    smpeg_cflags = run_command(smpeg_config, '--cflags', check: true)
    smpeg_cflags_out = smpeg_cflags.stdout().strip().split()
    add_project_arguments([smpeg_cflags_out], language: ['cpp','c'])
    smpeg_libs = run_command(smpeg_config, '--libs', check: true)
    smpeg_libs_out = smpeg_libs.stdout().strip().split()
    add_project_link_arguments([smpeg_libs_out], language: ['cpp','c'])
endif

# TODO need to detect 'libbz2' before adding to linker arguments
add_project_link_arguments(['-L/usr/lib/x86_64-linux-gnu','-lbz2'], language: ['cpp','c'])

embed_sources = files('embed.cpp')

onscripter_sources = files(
'AnimationInfo.cpp',
'DirectReader.cpp',
'DirPaths.cpp',
'DirtyRect.cpp',
'Encoding.cpp',
'FontInfo.cpp',
'graphics_routines.cpp',
'Layer.cpp',
'Linux_messagebox.cpp',
'NsaReader.cpp',
'ONScripterLabel_animation.cpp',
'ONScripterLabel_command.cpp',
'ONScripterLabel.cpp',
'ONScripterLabel_effect_breakup.cpp',
'ONScripterLabel_effect_cascade.cpp',
'ONScripterLabel_effect.cpp',
'ONScripterLabel_effect_trig.cpp',
'ONScripterLabel_event.cpp',
'ONScripterLabel_file2.cpp',
'ONScripterLabel_file.cpp',
'ONScripterLabel_image.cpp',
'ONScripterLabel_rmenu.cpp',
'ONScripterLabel_sound.cpp',
'ONScripterLabel_text.cpp',
'resize_image.cpp',
'SarReader.cpp',
'ScriptHandler.cpp',
'ScriptParser_command.cpp',
'ScriptParser.cpp',
'sjis2utf16.cpp',
)

sdl1_dep = dependency('sdl', version: '>=1.2.15')
sdlmixer1_dep = dependency('SDL_mixer', version: '>=1.2.12')
sdlimage1_dep = dependency('SDL_image', version: '>=1.2.12')
sdlttf1_dep = dependency('SDL_ttf', version: '>=2.0.11')
vorbisfile_dep = dependency('vorbisfile', version: '>=1.3.7')
x11_dep = dependency('x11')

graphics_mmx_sources = files('graphics_mmx.cpp')
graphics_mmx_lib = []
if cpu_family in ['x86','x86_64']
  graphics_mmx_lib = static_library('ons-en_graphics_mmx',
                                  graphics_mmx_sources,
                                  cpp_args: ['-mmmx', '-DUSE_X86_GFX'])
endif

graphics_sse2_sources = files('graphics_sse2.cpp')
graphics_sse2_lib = []
if cpu_family in ['x86','x86_64']
  graphics_sse2_lib = static_library('ons-en_graphics_sse2',
                                   graphics_sse2_sources,
                                   cpp_args: ['-msse2', '-DUSE_X86_GFX'],
                                   dependencies: [sdl1_dep])
endif

graphics_altivec_sources = files('graphics_altivec.cpp')
graphics_altivec_lib = []
if cpu_family in ['ppc','ppc64'] and target_endian == 'big'
  graphics_altivec_lib = static_library('graphics_altivec',
                                   graphics_altivec_sources,
                                   cpp_args: ['-maltivec', '-DUSE_PPC_GFX'])
endif

embed = executable('embed',
           embed_sources,
           install: false,
           link_args: [],
           dependencies: [])

resources = custom_target('gen-resources',
                              input: 'ons-en.png',
                              output: 'resources.cpp',
                              command: [embed, '-o', '@OUTPUT@', '@INPUT@', 'icon.png'])

executable('onscripter-en',
           'onscripter.cpp', onscripter_sources, resources,
           install: true,
           link_args: [hardened_args],
           link_with: [graphics_mmx_lib, graphics_sse2_lib, graphics_altivec_lib],
           pie: onscripter_pie,
           dependencies: [sdl1_dep, sdlmixer1_dep, sdlimage1_dep, sdlttf1_dep, vorbisfile_dep, x11_dep])
