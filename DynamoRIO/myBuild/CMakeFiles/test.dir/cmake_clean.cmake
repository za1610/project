FILE(REMOVE_RECURSE
  "CMakeFiles/test.dir/test.c.o"
  "bin/libtest.pdb"
  "bin/libtest.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/test.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
