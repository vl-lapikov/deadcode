deadcode
========

Now only php 5.5.14 was tested

To build extension:
```
phpize
./configure --enable-deadcode
make
```

Add extension to php.ini:
```
extension=/path-to-your-extension/modules/deadcode.so
```

Run some test script using:
```
php any.php
```

To debug:
```
gdb /usr/local/bin/php
...
(gdb) break <function_name>
(gdb) run test.php
```


