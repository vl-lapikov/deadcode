PHP_ARG_ENABLE(deadcode, whether to enable Deadcode support,
[ --enable-deadcode   Enable Deadcode support])
if test "$PHP_DEADCODE" = "yes"; then
  AC_DEFINE(HAVE_DEADCODE, 1, [Whether you have Deadcode])
  PHP_NEW_EXTENSION(deadcode, deadcode.c tools.c, $ext_shared)
fi
