# Strings in Meson are declared with single quotes.
project(
  'particle', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20',
  ],
)

# Meson code is strongly typed so no object is ever converted to another
# under the covers.
message('hello, world!')

# Create simple executable.
particle = executable(
  'particle', 'bin/particle.cpp',
)

# Create library and specify include directory.
db = library(
  'db', 'lib/database.cpp',
  include_directories: 'lib',
)

# Create executable and link with library.
service = executable(
  'service', 'bin/service.cpp',
  link_with: db,
)
