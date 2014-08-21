PHP_ARG_ENABLE(deadcode, whether to enable Inquisition support,
[ --enable-deadcode   Enable Inquisition support])
if test "$PHP_DEADCODE" = "yes"; then
  AC_DEFINE(HAVE_DEADCODE, 1, [Whether you have Inquisition])
  PHP_NEW_EXTENSION(deadcode, deadcode.c, tool.c, $ext_shared)
fi
