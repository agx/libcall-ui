call_ui_enum_headers = files(['cui-call.h',
                             ])
call_ui_enum_sources = gnome.mkenums_simple('cui-enums',
                                            sources : call_ui_enum_headers)

call_ui_resources = gnome.compile_resources(
   'cui-resources',
   'call-ui.gresources.xml',
   extra_args: '--manual-register',
   c_name: 'cui',
)

call_ui_deps = [
  libcallaudio_dep,
  gtk_dep,
  libhandy_dep,
  cc.find_library('m', required: false),
  cc.find_library('rt', required: false),
]

call_ui_sources = files( 
 'cui-call.c',
 'cui-call.h',
 'cui-call-display.c',
 'cui-call-display.h',
 'cui-encryption-indicator.c',
 'cui-encryption-indicator-priv.h',
 'cui-main.c',
 'cui-main.h',
) + [
  call_ui_resources,
  call_ui_enum_sources,
]

call_ui_c_args = [
  '-DG_LOG_DOMAIN="Cui"',
]

call_ui_lib = static_library('call-ui',
  call_ui_sources,			     
  c_args: call_ui_c_args,
  include_directories: [ root_inc, src_inc ],
  dependencies: call_ui_deps)

libcall_ui_dep = declare_dependency(
  include_directories: include_directories('.'),
  dependencies: call_ui_deps,
  link_with: call_ui_lib)
