#!/usr/bin/env python

################################################################################

hdr_utils               = 'utils.h'
hdr_lib                 = 'lib.h'
hdr_iolib               = 'iolib.h'
hdr_os_interface        = 'os-interface.h'

src_algs                = 'algs.cpp'
src_basic_ops           = 'basic-ops.cpp'
src_bin_rel_obj         = 'bin-rel-obj.cpp'
src_debug               = 'debug.cpp'
src_instrs              = 'instrs.cpp'
src_inter_utils         = 'inter-utils.cpp'
src_iolib               = 'iolib.cpp'
src_mem_alloc           = 'mem-alloc.cpp'
src_mem_core            = 'mem-core.cpp'
src_mem                 = 'mem.cpp'
src_mem_utils           = 'mem-utils.cpp'
src_os_interface_linux  = 'os-interface-linux.cpp'
src_parsing             = 'parsing.cpp'
src_printing            = 'printing.cpp'
src_sorting             = 'sorting.cpp'
src_tern_rel_obj        = 'tern-rel-obj.cpp'
src_utils               = 'utils.cpp'

hdr_table_utils         = 'table-utils.h'
src_unary_table         = 'unary-table.cpp'
src_binary_table        = 'binary-table.cpp'
src_ternary_table       = 'ternary-table.cpp'
src_hashing             = 'hashing.cpp'
src_mem_copying         = 'mem-copying.cpp'
src_value_store         = 'value-store.cpp'

hdr_cell_lang           = 'interface/cell-lang.h'
hdr_lib_cpp             = 'interface/lib-cpp.h'
src_value               = 'interface/value.cpp'
src_conversion          = 'interface/conversion.cpp'
src_lib_cpp             = 'interface/lib-cpp.cpp'

################################################################################

std_headers = [
  hdr_utils,
  hdr_lib,
  hdr_iolib,
  hdr_os_interface
]

std_sources = [
  src_algs,
  src_basic_ops,
  src_bin_rel_obj,
  src_debug,
  src_instrs,
  src_inter_utils,
  src_iolib,
  src_mem_alloc,
  src_mem_core,
  src_mem,
  src_mem_utils,
  src_os_interface_linux,
  src_parsing,
  src_printing,
  src_sorting,
  src_tern_rel_obj,
  src_utils
]

table_headers = [hdr_table_utils]

table_sources = [
  src_unary_table,
  src_binary_table,
  src_ternary_table,
  src_hashing,
  src_mem_copying,
  src_value_store,
]

interface_headers = [
  hdr_cell_lang,
  hdr_lib_cpp
]

interface_sources = [
  src_value,
  src_conversion,
  src_lib_cpp
]

################################################################################

num_of_tabs = 0

def escape(ch):
  if ch == ord('\\'):
    return '\\\\'
  elif ch == ord('"'):
    return '\\"'
  elif ch >= ord(' ') or ch <= ord('~'):
    return chr(ch)
  elif ch == ord('\t'):
    global num_of_tabs
    num_of_tabs += 1
    return '\\t'
  else:
    print 'Invalid character: ' + ch
    exit(1);


def convert_file(file_name):
  res = []
  f = open(file_name)
  # i = 0
  for l in f:
    l = l.rstrip()
    # if i > 10:
    #   break
    # i = i + 1
    if l.startswith('#include'):
      pass
    else:
      el = ''.join([escape(ord(ch)) for ch in l])
      res.append('"' + el + '"')
  return res


# def to_code(bytes):
#   count = len(bytes)
#   ls = []
#   l = ' '
#   for i, b in enumerate(bytes):
#     l += ' ' + str(b) + (',' if i < count-1 else '')
#     if len(l) > 80:
#       ls.append(l)
#       l = ' '
#   if l:
#     ls.append(l)
#   return ls


def convert_files(directory, file_names):
  ls = []
  for i, f in enumerate(file_names):
    if i > 0:
      ls.extend(['""', '""'])
    ls.extend(convert_file(directory + '/' + f))
  last_line = len(ls) - 1
  fls = []
  for i, l in enumerate(ls):
    if i != last_line:
      l += ','
    fls.append('  ' + l)
  return fls


def data_array_def(array_name, directory, file_names):
  data = convert_files(directory, file_names)
  # code = to_code(data)
  return ['String* ' + array_name + ' = ('] + data + [');']

################################################################################

from sys import argv, exit

if len(argv) != 4:
  print 'Usage: ' + argv[0] + ' <input directory> <output file> <empty output file>'
  exit(0)

_, input_dir, out_fname, empty_out_fname = argv

file_data = [
  data_array_def('standard_headers', input_dir, std_headers),
  data_array_def('standard_sources', input_dir, std_sources),
  data_array_def('table_headers', input_dir, table_headers),
  data_array_def('table_sources', input_dir, table_sources),
  data_array_def('interface_headers', input_dir, interface_headers),
  data_array_def('interface_sources', input_dir, interface_sources),
  data_array_def('cell_lang_header', input_dir, [hdr_cell_lang])
]

out_file = open(out_fname, 'w')
for i, f in enumerate(file_data):
  if i > 0:
    out_file.write('\n\n')
  for l in f:
    out_file.write(l + '\n');

empty_file_data = [
  data_array_def('standard_headers', input_dir, []),
  data_array_def('standard_sources', input_dir, []),
  data_array_def('table_headers', input_dir, []),
  data_array_def('table_sources', input_dir, []),
  data_array_def('interface_headers', input_dir, []),
  data_array_def('interface_sources', input_dir, []),
  data_array_def('cell_lang_header', input_dir, [hdr_cell_lang])
]

empty_out_file = open(empty_out_fname, 'w')
for i, f in enumerate(empty_file_data):
  if i > 0:
    empty_out_file.write('\n\n')
  for l in f:
    empty_out_file.write(l + '\n')
