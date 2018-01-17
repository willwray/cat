project('cat', 'cpp', default_options : 'cpp_std=c++1z')

test('test concatenating chars',
  executable('test_cat', 'test_cat.cpp')
)

cat_dep = declare_dependency(
  include_directories : include_directories('.')
)
