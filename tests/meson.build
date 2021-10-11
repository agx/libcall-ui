if get_option('tests')

test_env = [
  'G_DEBUG=gc-friendly,fatal-warnings',
  'MALLOC_CHECK_=2',
  'NO_AT_BRIDGE=1',
]

display_test_sources = [
  'test-display.c',
  'dummy-call.c', 'dummy-call.h'
]

t = executable('display', display_test_sources,
               call_ui_resources,
               pie: true,
               link_with : call_ui_lib,
               dependencies : libcall_ui_dep,
               include_directories : [src_inc, tests_inc]
)

test ('display', t, env: test_env)

endif