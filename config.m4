PHP_ARG_ENABLE(inquisition, whether to enable Inquisition support,
[ --enable-inquisition   Enable Inquisition support])
if test "$PHP_INQUISITION" = "yes"; then
  AC_DEFINE(HAVE_INQUISITION, 1, [Whether you have Inquisition])
  PHP_NEW_EXTENSION(inquisition, inquisition.c, $ext_shared)
fi
